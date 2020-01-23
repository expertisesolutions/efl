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

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace eolian_mono {

// Temporary namespace (just to prevent accidental name clashes while in
// first development)
namespace csharp_conversion {

using TupleType = std::vector<std::string>;

// TODO: Remember possible return types :P
using Type = std::variant<std::string, TupleType>; 

enum class CSharp_Modifiers {
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
    enum class Options {
        SETTER_ONLY,
        GETTER_ONLY,
        GETTER_AND_SETTER,
    };

    Decl decl;
    Options options = Options::GETTER_AND_SETTER;

    CSharp_Property() = default;
};

struct CSharp_Klass {
    std::string name;
    std::vector<CSharp_Property> properties;
    std::vector<Decl> attributes;
    std::vector<Decl> functions;
};

}

}

#endif
