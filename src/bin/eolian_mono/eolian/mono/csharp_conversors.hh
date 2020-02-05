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

#ifndef EOLIAN_MONO_CSHARP_CONVERSORS_HH
#define EOLIAN_MONO_CSHARP_CONVERSORS_HH

#include "csharp_definitions.hh"

namespace eolian_mono::csharp_definitions {
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

    // stolen from direction_modifier for setters
    if (param.direction == attributes::parameter_direction::inout) {
        modifiers |= CSharp_Modifiers::REF;
    } else if (param.direction == attributes::parameter_direction::in) {
        if (param.type.c_type == "Eina_Slice" || param.type.c_type == "Eina_Rw_Slice") {
            modifiers |= CSharp_Modifiers::REF;
        } else {
            modifiers |= CSharp_Modifiers::OUT;
        }
    } else if (param.direction == attributes::parameter_direction::in && param.type.is_ptr) {
        auto regular = efl::eina::get<attributes::regular_type_def>(&param.type.original_type);
        if (helpers::need_struct_conversion(regular))
           modifiers |= CSharp_Modifiers::REF; // Don't add ref on Marshal if it is ptr
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

auto native_call_from(attributes::function_def const& f, std::string const& name) -> CSharp_Function {
    auto decl = Decl{
        CSharp_Modifiers::NONE,
        f.return_type,
        name,
    };
    auto params = std::vector<Decl>{};

    if (f.parameters.size() == 0) {
        // ¯\_(ツ)_/¯
    } else {
        for (auto& param: f.parameters) {
            params.push_back(to_decl(param));
        }
    }

    return CSharp_Function{decl, params};
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
        auto eo_getter = *eolian_property.getter;
        auto managed_method_name = name_helpers::managed_method_name(eo_getter);
        auto native_call = is_interface 
                           ? optional<CSharp_Function>{} 
                           : native_call_from(eo_getter, managed_method_name);

        getter = CSharp_Property::Getter{
            get_scope ? to_modifiers(*get_scope) : CSharp_Modifiers::NONE,
            native_call,
        };
    }
    if (eolian_property.setter) {
        auto eo_setter = *eolian_property.setter;
        auto managed_method_name = name_helpers::managed_method_name(eo_setter);
        auto native_call = is_interface
                           ? optional<CSharp_Function>{} 
                           : native_call_from(eo_setter, managed_method_name);

        setter = CSharp_Property::Setter{
            set_scope ? to_modifiers(*set_scope) : CSharp_Modifiers::NONE,
            native_call,
        };
    }

    if (getter && setter)
        return {CSharp_Property{decl, *getter, *setter}};
    if (getter)
        return {CSharp_Property{decl, *getter}};
    return {};
}

}}

#endif
