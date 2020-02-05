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
    NONE      = 0,
    PUBLIC    = 1 << 0,
    PRIVATE   = 1 << 1,
    PROTECTED = 1 << 2,
    INTERNAL  = 1 << 3,

    CONST     = 1 << 4,
    STATIC    = 1 << 5,
    READ_ONLY = 1 << 6,

    OUT       = 1 << 7,
    REF       = 1 << 8,
    DYN       = 1 << 9,
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

auto contains(CSharp_Modifiers lhs, CSharp_Modifiers rhs) -> bool {
    return (lhs & rhs) != CSharp_Modifiers::NONE;
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

    std::vector<Decl> parameters;
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

}}

#endif
