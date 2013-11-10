
#ifndef EFL_ECXX_EO_GENERATOR_HH
#define EFL_ECXX_EO_GENERATOR_HH

#include <boost/spirit/home/karma.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "eo_type_def.hh"

namespace efl { namespace ecxx {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
struct eo_class_generator
  : karma::grammar<OutputIterator, eo_class()>
{
  eo_class_generator() : eo_class_generator::base_type(ini)
  {
    using namespace boost::spirit;
    using namespace boost::spirit::ascii;
    using phoenix::at_c;

    ini = "" << string[_1 = at_c<1>(_val)];
  }

  karma::rule<OutputIterator, eo_class()> ini;
};

} } 

#endif // EFL_ECXX_EO_GENERATOR_HH
