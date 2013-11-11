
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH

#include <string>

#include "eo_class_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
eo_class_generator<OutputIterator>::eo_class_generator() 
  : eo_class_generator::base_type(start)
{
  using namespace boost::spirit;
  using namespace boost::spirit::ascii;

  using phoenix::at_c;
  using karma::_a;
  using karma::_1;
  using karma::_val;
  
  start = 
    ( 
      karma::eps(at_c<0>(_val) == eo_class::regular_)
      << regular_class
    ) | (
      karma::eps(at_c<0>(_val) == eo_class::regular_noninst_)
      << regular_noninst_class
    ) | (
      karma::eps(at_c<0>(_val) == eo_class::interface_)
      << interface_class
    ) | (
      karma::eps(at_c<0>(_val) == eo_class::mixin_)
      << mixin_class
    );

  regular_class = "";//karma::eps[_a = at_c<1>(_val)] ;
  
    //<< "struct " << karma::string[_1 = _a];
  
    // << " : efl::eo::" << 
    // ( 
    //   (karma::eps(at_c<2> == "") << "base") 
    //   | at_c<2>;
    // );

  regular_noninst_class = karma::string("regular-noninst-class-XXX");
  
  interface_class = karma::string("interface-class-XXX");
  
  mixin_class = karma::string("mixin-class-XXX");
  
}

} } }


#endif // EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
