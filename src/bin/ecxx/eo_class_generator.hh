
#ifndef EFL_ECXX_EO_GENERATOR_HH
#define EFL_ECXX_EO_GENERATOR_HH

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
  eo_class_generator() : eo_class_generator::base_type(start)
  {
    using namespace boost::spirit;
    using namespace boost::spirit::ascii;
    using phoenix::at_c;

    //eo_class_definition_generator<OutputIterator> 
  }

  karma::rule<OutputIterator, eo_class()> start;
};

template <typename OutputIterator>
struct eo_class_definition_generator
{
};

template <typename OutputIterator>
struct eo_class_constructor_generator
{
};

template <typename OutputIterator>
struct eo_class_destructor_generator
{
};

template <typename OutputIterator>
struct eo_class_funcs_generator
{
};


  
} } } 

#endif // EFL_ECXX_EO_GENERATOR_HH
