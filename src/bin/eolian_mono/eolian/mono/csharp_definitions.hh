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

#include <Eina.hh>

namespace eolian_mono {

// Temporary namespace (just to prevent accidental name clashes while in
// first development)
namespace csharp_conversion {

using TupleType = std::vector<std::string>;

// TODO: Remember possible return types :P
struct Type {
    TupleType types;

    auto is_tuple() const -> bool {
        return types.size() > 1;
    }
};

enum class CSharp_Modifiers {
    NONE,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    INTERNAL,

    CONST,
    STATIC,
};

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
        CSharp_Modifiers modifiers = CSharp_Modifiers::NONE;
        bool calls_native = false;
    };

    struct Setter {
        CSharp_Modifiers modifiers = CSharp_Modifiers::NONE;
        bool calls_native = false;
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

// Straight forward generators - to be moved (TODO).
struct CSharp_Code_Terminal {
    struct Generator {
        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, CSharp_Function const& function, Context const& context) const
        {
            return true;
        }
    };
} const CSharp_Code;

struct Assign_Terminal {
    struct Generator {
        Generator(CSharp_Variable& var, std::string expr):
            var{var},
            expr{std::move(expr)}
        {}

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, attributes::unused_type, Context const& context) const
        {
            if (!as_generator(
                   (!var.initialized ? "var " : "") << var.name << " = " << expr
                 ).generate(sink, attributes::unused, context))
              return false;

            var.initialized = true;

            return true;
        }

        CSharp_Variable& var;
        std::string expr;
    };

    auto operator()(CSharp_Variable var, std::string expr) const -> Generator {
        return {var, expr};
    }
} const Assign;

}

}

namespace efl { namespace eolian { namespace grammar {

template <>
struct is_generator<::eolian_mono::csharp_conversion::CSharp_Code_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_conversion::CSharp_Code_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_conversion::CSharp_Code_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_conversion::CSharp_Code_Terminal::Generator> : std::true_type {};


template <>
struct is_generator<::eolian_mono::csharp_conversion::Assign_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_conversion::Assign_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_conversion::Assign_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_conversion::Assign_Terminal::Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_conversion::CSharp_Code_Terminal> : std::integral_constant<int, 1> {};
template <>
struct attributes_needed<::eolian_mono::csharp_conversion::CSharp_Code_Terminal::Generator> : std::integral_constant<int, 1> {};

}}}}

#endif
