
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
  : karma::grammar
  <
    OutputIterator, eo_class(), 
    karma::locals<eo_class::eo_class_type, std::string, std::string> 
  >
{
  eo_class_generator();
  karma::rule<OutputIterator, eo_class(), karma::locals
    <eo_class::eo_class_type, std::string, std::string> > start;
  karma::rule<OutputIterator, void(unsigned int)> indent;
  karma::rule<OutputIterator, std::vector<std::string>()> arguments_declaration_loop;
  karma::rule<OutputIterator, std::vector<std::string>()> arguments_loop;
  karma::rule<OutputIterator, std::string()> argument_declaration;
  karma::rule<OutputIterator, std::string()> argument;
  karma::rule<OutputIterator, eo_class()> class_inheritance;
  karma::rule<OutputIterator, std::vector<std::string>(std::string)> class_extensions_loop;
  karma::rule<OutputIterator, std::string(std::string)> class_extension;
  karma::rule<OutputIterator, eo_class()> constructor_eo;
  karma::rule<OutputIterator, 
    std::vector<eo_constructor>(std::string, std::string)> constructors_loop;
  karma::rule<OutputIterator, eo_constructor(std::string, std::string)> constructor;
  karma::rule<OutputIterator, eo_class()> copy_constructor;
  karma::rule<OutputIterator, std::string()> destructor;
  karma::rule<OutputIterator, std::vector<eo_function>()> functions_loop;
  karma::rule<OutputIterator, eo_function(), karma::locals<std::string, bool> > function;
  karma::rule<OutputIterator, std::string()> eo_class_getter_definition;
  karma::rule<OutputIterator, 
    std::vector<eo_constructor>(std::string)> eo_class_constructors_loop;
  karma::rule<OutputIterator, eo_constructor(std::string)> eo_class_constructor;
};

} } } 

#endif // EFL_ECXX_EO_CLASS_GENERATOR_HH
