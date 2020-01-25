/*
 * Copyright 2019 by its authors. See AUTHORS.
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
#ifndef EOLIAN_CXX_optional_HH
#define EOLIAN_CXX_optional_HH

#include "grammar/generator.hpp"

namespace efl { namespace eolian { namespace grammar {

template <typename Generator>
struct optional_generator
{
   optional_generator(Generator g)
     : generator(g) {}
  
  template <typename OutputIterator, typename Attribute, typename Context>
  bool generate(OutputIterator sink, Attribute const& attribute, Context const& context) const
   {
      if (attribute)
        return generator.generate(sink, attribute.get(), context);
      return false;
   }

   Generator generator;
};

template <typename Generator>
struct is_eager_generator<optional_generator<Generator> > : std::true_type {};
template <typename Generator>
struct is_generator<optional_generator<Generator> > : std::true_type {};

namespace type_traits {
template  <typename G>
struct attributes_needed<optional_generator<G> > : std::integral_constant<int, 1> {};
}
      
template <typename Generator>
typename std::enable_if<grammar::is_generator<Generator>::value, optional_generator<Generator>>::type
operator!(Generator g)
{
   return optional_generator<Generator>{g};
}

} } }

#endif

