
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_HH

#include <boost/spirit/home/karma.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "eo_type_def.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
struct eo_class_generator
  : karma::grammar<OutputIterator, eo_class()>
{
  eo_class_generator();
  karma::rule<OutputIterator, eo_class()> start;
  karma::rule<OutputIterator> regular_class;
  karma::rule<OutputIterator> regular_noninst_class;
  karma::rule<OutputIterator> interface_class;
  karma::rule<OutputIterator> mixin_class;
};

} } } 

#endif // EFL_ECXX_EO_CLASS_GENERATOR_HH
