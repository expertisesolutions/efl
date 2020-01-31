/*
 * Copyright 2020 by its authors. See AUTHORS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * ABOUT THIS FILE
 * ---------------
 *
 * This file contains definitions suited for a straight-forward C# code
 * generation. Every structural conversion necessary from Eolian should come
 * here instead of middle of generation code. Types 
 *
 * ---------------
 */
#ifndef EOLIAN_MONO_CSHARP_DEFINITIONS_HH
#define EOLIAN_MONO_CSHARP_DEFINITIONS_HH

#include <string>
#include <variant>
#include <vector>
#include <type_traits>

#include <Eina.hh>

#include "grammar/generator.hpp"
#include "grammar/klass_def.hpp"

#include "grammar/alternative.hpp"
#include "grammar/attribute_conditional.hpp"
#include "grammar/eps.hpp"
#include "grammar/indentation.hpp"
#include "grammar/optional.hpp"
#include "type.hh"

namespace eolian_mono {

namespace attributes = efl::eolian::grammar::attributes;

template<typename T>
auto or_default(efl::eina::optional<T> const& optional, T default_value) -> T {
    if (optional)
        return *optional;
    return default_value;
}

// Temporary namespace (just to prevent accidental name clashes while in
// first development)
namespace csharp_definitions {

namespace attributes = efl::eolian::grammar::attributes;

namespace utils {

/**
 * Extracts value from enum.
 */
template <typename Enum>
constexpr auto value_of(Enum e) -> typename std::underlying_type<Enum>::type
{
    return static_cast<typename std::underlying_type<Enum>::type>(e);
}

}

using efl::eina::optional;

using SingleType = attributes::type_def;
using TupleType = std::vector<SingleType>;

// TODO: Remember possible return types :P
struct Type {
    TupleType types;

    Type(SingleType type):
        types{type}
    {}

    Type(TupleType type):
        types{type}
    {}
};


enum class CSharp_Modifiers {
    NONE,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    INTERNAL,

    CONST,
    STATIC,
    READ_ONLY,

    OUT,
    REF,
    DYN,
};

auto operator|(CSharp_Modifiers lhs, CSharp_Modifiers rhs) -> CSharp_Modifiers {
    return static_cast<CSharp_Modifiers>(utils::value_of(lhs) | utils::value_of(rhs));
}

auto operator&(CSharp_Modifiers lhs, CSharp_Modifiers rhs) -> CSharp_Modifiers {
    return static_cast<CSharp_Modifiers>(utils::value_of(lhs) & utils::value_of(rhs));
}

auto operator|=(CSharp_Modifiers& lhs, CSharp_Modifiers rhs) -> CSharp_Modifiers& {
    lhs = lhs | rhs;
    return lhs;
}

auto has_any(CSharp_Modifiers mod) -> bool {
    return mod != CSharp_Modifiers::NONE;
}

// Removes every modifier that is not scope.
auto scope(CSharp_Modifiers mod) -> CSharp_Modifiers {
    return mod & (CSharp_Modifiers::PUBLIC 
                  | CSharp_Modifiers::PRIVATE 
                  | CSharp_Modifiers::PROTECTED 
                  | CSharp_Modifiers::INTERNAL);
}


struct Decl {
    CSharp_Modifiers modifiers;
    Type type;
    std::string name;
};

struct CSharp_Function {
    Decl decl;

    std::vector<Type> parameters;
};

struct CSharp_Property {
    struct Getter {
        CSharp_Modifiers modifiers;
        optional<CSharp_Function> native_call;

        Getter(CSharp_Modifiers mods = CSharp_Modifiers::NONE
               , optional<CSharp_Function> native_call = {}):
            modifiers{mods},
            native_call{native_call}
        {}
    };

    struct Setter {
        CSharp_Modifiers modifiers;
        efl::eina::optional<CSharp_Function> native_call;

        Setter(CSharp_Modifiers mods = CSharp_Modifiers::NONE
               , optional<CSharp_Function> native_call = {}):
            modifiers{mods},
            native_call{native_call}
        {}
    };

    Decl decl;
    efl::eina::optional<Getter> getter = {};
    efl::eina::optional<Setter> setter = {};

    CSharp_Property(Decl decl, Getter getter):
        decl{decl},
        getter{getter}
    {}

    CSharp_Property(Decl decl, Setter setter):
        decl{decl},
        setter{setter}
    {}

    CSharp_Property(Decl decl, Getter getter, Setter setter):
        decl{decl},
        getter{getter},
        setter{setter}
    {}
};

struct CSharp_Klass {
    std::string name;
    std::vector<CSharp_Function> constructors;

    std::vector<Decl> attributes;
    std::vector<CSharp_Property> properties;
    std::vector<CSharp_Function> functions;

};

struct CSharp_Variable {
    CSharp_Variable(std::string name):
        name{std::move(name)}
    {}

    std::string name;
    bool initialized = false;
};


/**
 *
 * Conversors for current Eo implementations to correct CSharpish structures
 * -------------------------------------------------------------------------
 *
 */
namespace conversors {

auto to_decl(attributes::parameter_def const& param) -> Decl {
    auto modifiers = CSharp_Modifiers::NONE;

    if (param.direction == efl::eolian::grammar::attributes::parameter_direction::out) {
        modifiers |= CSharp_Modifiers::OUT;
    }

    return {modifiers, param.type, param.param_name};
}


auto to_modifiers(attributes::member_scope scope) -> CSharp_Modifiers {
    using attributes::member_scope;
    switch (scope) {
        case member_scope::scope_public:
            return CSharp_Modifiers::PUBLIC;
        case member_scope::scope_private:
            return CSharp_Modifiers::PRIVATE;
        case member_scope::scope_protected:
            return CSharp_Modifiers::PROTECTED;
        default:
            return CSharp_Modifiers::NONE;
    }
}


auto will_generate_property(attributes::property_def const& eolian_property
                            , bool is_interface
                            , bool is_concrete
                            , bool is_static
                            , optional<attributes::member_scope> get_scope
                            , optional<attributes::member_scope> set_scope) -> bool {
    using attributes::member_scope;

    // Cannot generate properties without getter.
    if (!eolian_property.getter) {
        return false; 
    }

    // Do not generate if no accessor is public
    if (is_interface
        && (get_scope && *get_scope != member_scope::scope_public)
        && (set_scope && *set_scope != member_scope::scope_public))
        return false;

    // ???
    if ((is_concrete || is_interface) && is_static)
        return false;

    return true;
}

auto extract_parameters(attributes::property_def const& eolian_property) -> std::vector<Decl>
{
    using attributes::parameter_direction;

    auto parameters = std::vector<Decl>{};

    if (eolian_property.setter) {
        auto setter_params = eolian_property.setter->parameters;
        std::transform(std::begin(setter_params), std::end(setter_params),
                       std::back_inserter(parameters),
                       [] (attributes::parameter_def p) -> Decl
                       {
                           return to_decl(p);
                       });
    } else if (eolian_property.getter) {
        auto getter_params = eolian_property.getter->parameters;

        if (getter_params.size() == 0) {
            parameters.push_back(to_decl({parameter_direction::in
                                          , eolian_property.getter->return_type
                                          , "propertyResult"
                                          , {}
                                          , eolian_property.getter->unit}));
        } else {
            std::transform(std::begin(getter_params), std::end(getter_params),
                           std::back_inserter(parameters),
                           [] (attributes::parameter_def p) -> Decl
                           {
                               p.direction = parameter_direction::in;
                               return to_decl(p);
                           });
        }
    }

    return parameters;
}

template <typename Context>
auto to_property(attributes::property_def const& eolian_property, Context const& context) -> efl::eina::optional<CSharp_Property> {
    using efl::eina::optional;
    using efl::eolian::grammar::context_find_tag;
    using attributes::member_scope;

    auto is_interface = context_find_tag<class_context>(context).current_wrapper_kind == class_context::interface;
    auto is_concrete = context_find_tag<class_context>(context).current_wrapper_kind == class_context::concrete;
    auto is_static = (eolian_property.getter.is_engaged() && eolian_property.getter->is_static)
                     || (eolian_property.setter.is_engaged() && eolian_property.setter->is_static);

    auto scope = optional<member_scope>{member_scope::scope_public};
    auto get_scope = eolian_property.getter ? optional<member_scope>{eolian_property.getter->scope} : optional<member_scope>{};
    auto set_scope = eolian_property.setter ? optional<member_scope>{eolian_property.setter->scope} : optional<member_scope>{};

    if (!will_generate_property(eolian_property, is_interface, is_concrete, is_static, get_scope, set_scope))
        return {};

    // No explicit scope for interfaces in MCS!
    if (is_interface) {
        scope = {};
        get_scope = {};
        set_scope = {};
    // Same scope for get/set? Then that shall be the property's scope.
    // Also, no need to specify get/set scope in this case.
    } else if (get_scope && get_scope == set_scope) {
        scope = get_scope;
        get_scope = {};
        set_scope = {};
    // No setter, but property has the same scope of get? No need to specify get, then.
    } else if (!eolian_property.setter || get_scope == scope) {
        get_scope = {};
    // No getter, but property has the same scope of set? No need to specify set, then.
    } else if (!eolian_property.getter || set_scope == scope) {
        set_scope = {};
    }

    auto parameters = extract_parameters(eolian_property);

    auto modifiers = CSharp_Modifiers::NONE;

    if (scope) {
        modifiers |= to_modifiers(*scope);
    }

    if (is_static) {
        modifiers |= CSharp_Modifiers::STATIC;
    }

    auto return_type = Type{TupleType{}};
    for (auto& param: parameters) {
        return_type.types.push_back(param.type.types[0]);
    }

    auto getter = efl::eina::optional<CSharp_Property::Getter>{};
    auto setter = efl::eina::optional<CSharp_Property::Setter>{};

    auto decl = Decl{
        modifiers,
        return_type,
        name_helpers::property_managed_name(eolian_property),
    };

    if (eolian_property.getter) {
        getter = CSharp_Property::Getter{
            get_scope ? to_modifiers(*get_scope) : CSharp_Modifiers::NONE,
            {},
        };
    }
    if (eolian_property.setter) {
        setter = CSharp_Property::Setter{
            set_scope ? to_modifiers(*set_scope) : CSharp_Modifiers::NONE,
            {},
        };
    }

    if (getter && setter)
        return {CSharp_Property{decl, *getter, *setter}};
    if (getter)
        return {CSharp_Property{decl, *getter}};
    return {};
}

}

/**
 *
 * Straight forward generators - to be moved (TODO).
 * -------------------------------------------------
 *
 */

using namespace efl::eolian::grammar;

// Function calls
struct Call_Terminal
{
    struct Generator {
        Generator() = default;
        Generator(std::vector<std::string> args):
            args{args}
        {}

        template <typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, CSharp_Function const& function, Context const& context) const
        {
            return as_generator(
                        function.decl.name << "(" << (string % ", ") << ")"
                    ).generate(sink, args, context);
        }

        std::vector<std::string> args;
    };

    auto operator()() const -> Generator {
        return {};
    }

    auto operator()(std::vector<std::string> args) const -> Generator {
        return {args};
    }
} const Call;

auto as_generator(Call_Terminal) -> Call_Terminal::Generator {
    return {{}};
}

}}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Call_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Call_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Call_Terminal> : std::integral_constant<int, 1> {};
template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::integral_constant<int, 1> {};

}

}

namespace eolian_mono::csharp_definitions {

struct Type_Generator {
    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, Type const& _type, Context const& context) const
    {
        return as_generator(
                 attribute_conditional([](std::vector<SingleType> const& types) { return types.size() > 1; }) << (
                     "(" << (type(true) % ", ") << ")"
                 ) | *type(true) 
               ).generate(sink
                          , std::make_tuple(_type.types, _type.types)
                          , context);
    }
};

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Type_Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Type_Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Type_Generator> : std::integral_constant<int, 1> {};

}

}

namespace eolian_mono::csharp_definitions {

Type_Generator const Type_Gen = {};

auto split_as_str(CSharp_Modifiers modifiers) -> std::vector<std::string> {
    auto mods = std::vector<std::string>{};

    if (has_any(modifiers & CSharp_Modifiers::PUBLIC)) {
        mods.push_back("public");
    } else if (has_any(modifiers & CSharp_Modifiers::PRIVATE)) {
        mods.push_back("private");
    } else if (has_any(modifiers & CSharp_Modifiers::PROTECTED)) {
        mods.push_back("protected");
    } else if (has_any(modifiers & CSharp_Modifiers::INTERNAL)) {
        mods.push_back("internal");
    }

    if (has_any(modifiers & CSharp_Modifiers::STATIC)) {
        mods.push_back("static");
    }
    else if (has_any(modifiers & CSharp_Modifiers::CONST)) {
        mods.push_back("const");
    }

    /*
    if (has_any(modifiers & CSharp_Modifiers::READ_ONLY)) {
        mods.push_back("readonly");
    }
    */

    return mods;
}

// Every possible C# declaration (but not definitions)
struct Decl_Terminal {
    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, Decl const& decl, Context const& context) const
    {
        auto mods = split_as_str(decl.modifiers);

        if (!as_generator((string % " "))
             .generate(sink, mods, context))
            return false;

        if (!as_generator(" " << Type_Gen << " " << decl.name)
             .generate(sink, decl.type, context))
            return false;
        return true;
    }

    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, CSharp_Property::Getter const& getter, Context const& context) const
    {
        auto const& indent = current_indentation(context);
        if (!as_generator(
                "get" <<
                ((!("{\n"
                 << indent << "return " << Call() << ";\n"
                 << "}\n"
                 )) | lit(";"))
            ).generate(sink, getter.native_call, context))
            return false;
        return true;
    }

    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, CSharp_Property::Setter const& setter, Context const& context) const
    {
        auto const& indent = current_indentation(context);
        if (!as_generator(
                "set" <<
                ((!("{\n"
                 << indent << Call({"value"}) << ";\n"
                 << "}\n"
                 )) | lit(";"))
            ).generate(sink, setter.native_call, context))
            return false;
        return true;
    }
} const Decl {};

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Decl_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Decl_Terminal> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Decl_Terminal> : std::integral_constant<int, 1> {};

}

}

// General C# code (TODO: Specify to Block?)
namespace eolian_mono::csharp_definitions {

using efl::eolian::grammar::scope_tab;

struct Definition_Terminal {
    static const Definition_Terminal Definition;

    struct Generator {
        constexpr Generator() = default;
        constexpr Generator(int indent_level):
            indent{indent_level}
        {}

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, csharp_definitions::CSharp_Function const& function, Context const& context) const
        {
            return true;
        }

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, csharp_definitions::CSharp_Property const& property, Context const& context) const
        {
            return as_generator(
                scope_tab(indent) << Decl << " {\n" <<
                (!(scope_tab(indent + 1) << Decl << "\n") | eps) << // property.getter
                (!(scope_tab(indent + 1) << Decl << "\n") | eps) << // property.setter
                scope_tab(indent) << "}\n")
                .generate(sink
                          , std::make_tuple(property.decl, property.getter, property.setter)
                          , context);
        }

        int indent = 0;
    };

    auto operator()(int indent_level) const -> Generator {
        return {indent_level};
    }
};

auto as_generator(Definition_Terminal) -> Definition_Terminal::Generator const {
    return {0};
}

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Definition_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Definition_Terminal> : std::true_type {};

}

// Assignment operator
namespace eolian_mono::csharp_definitions {

const Definition_Terminal Definition_Terminal::Definition = Definition_Terminal{};
Definition_Terminal const& Definition = Definition_Terminal::Definition;

struct Assign_Terminal {
    struct Generator {
        Generator(csharp_definitions::CSharp_Variable& var, std::string expr):
            var{var},
            expr{std::move(expr)}
        {}

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, attributes::unused_type, Context const& context) const
        {
            if (!as_generator(efl::eolian::grammar::lit("test") <<
                   (!var.initialized ? "var " : "") << var.name << " = " << expr
                 ).generate(sink, attributes::unused, context))
              return false;

            var.initialized = true;

            return true;
        }

        csharp_definitions::CSharp_Variable& var;
        std::string expr;
    };

    auto operator()(csharp_definitions::CSharp_Variable var, std::string expr) const -> Generator {
        return {var, expr};
    }
} const Assign;

}

namespace efl { namespace eolian { namespace grammar {

template <>
struct is_generator<::eolian_mono::csharp_definitions::Assign_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Assign_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Assign_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Assign_Terminal::Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Definition_Terminal> : std::integral_constant<int, 1> {};
template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::integral_constant<int, 1> {};

}

}}}

#endif
