
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_HH

#include <string>

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
namespace spirit = boost::spirit;

template <typename OutputIterator>
struct eo_class_generator
  : karma::grammar<OutputIterator, eo_class()
  , karma::locals<efl::ecxx::eo_class::eo_class_type, 
      std::string, std::string, std::string> >
{
  eo_class_generator();
  karma::rule<OutputIterator, eo_class()
    , karma::locals<efl::ecxx::eo_class::eo_class_type, 
        std::string, std::string, std::string > >
  start;
  karma::rule<OutputIterator, void(unsigned int)> indent;
  karma::rule<OutputIterator, std::vector<std::string>()> arguments_decl_loop;
  karma::rule<OutputIterator, std::vector<std::string>()> arguments_loop;
  karma::rule<OutputIterator, std::string()> argument_decl;
  karma::rule<OutputIterator, std::string()> argument;
  karma::rule<OutputIterator, eo_class()> constructor_eo;
  karma::rule<OutputIterator, 
    std::vector<eo_constructor>(std::string, std::string)> 
  constructors_loop;
  karma::rule<OutputIterator, eo_constructor(std::string, std::string)> constructor;
  karma::rule<OutputIterator, eo_class()> copy_constructor;
  karma::rule<OutputIterator, std::string()> destructor;
  karma::rule<OutputIterator, std::vector<eo_function>()> functions_loop;
  karma::rule<OutputIterator, eo_function(), karma::locals<std::string, bool> > function;
};

} } } 

#endif // EFL_ECXX_EO_CLASS_GENERATOR_HH
