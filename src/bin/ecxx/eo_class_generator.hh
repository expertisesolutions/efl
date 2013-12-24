
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_HH

#include <string>
#include <vector>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "eo_type_def.hh"
#include "arguments_generator.hh"
#include "eo_class_event_generator.hh"
#include "eo_class_detail_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct eo_class_generator
  : karma::grammar
  <
    OutputIterator, eo_class(),
    karma::locals<eo_class::eo_class_type, std::string, std::string>
  >
{
   eo_class_generator();
   karma::rule<OutputIterator, eo_class(),
               karma::locals<eo_class::eo_class_type, std::string, std::string> > start;
   karma::rule<OutputIterator, void(unsigned int)> tab;
   karma::rule<OutputIterator, std::string(std::string)> class_extension;
   karma::rule<OutputIterator, std::vector<std::string>(std::string)> class_extensions_loop;
   karma::rule<OutputIterator, eo_class()> class_inheritance;
   karma::rule<OutputIterator, eo_class()> constructor_eo;
   karma::rule<OutputIterator, eo_class()> copy_constructor;
   karma::rule<OutputIterator, eo_constructor(std::string, std::string)> constructor;
   karma::rule<OutputIterator, std::vector<eo_constructor>(std::string, std::string)> constructors_loop;
   karma::rule<OutputIterator, std::string()> destructor;
   karma::rule<OutputIterator, eo_function(), karma::locals<std::string, bool> > function;
   karma::rule<OutputIterator, std::vector<eo_function>()> functions_loop;
   karma::rule<OutputIterator, std::string()> eo_class_getter_definition;
   karma::rule<OutputIterator, eo_constructor(std::string)> eo_class_constructor;
   karma::rule<OutputIterator, std::vector<eo_constructor>(std::string)> eo_class_constructors_loop;

   // sub-grammars
   efl::ecxx::grammar::arguments_declaration_generator<OutputIterator> arguments_declaration;
   efl::ecxx::grammar::arguments_list_generator<OutputIterator> arguments_list;
   efl::ecxx::grammar::eo_class_event_generator<OutputIterator> events;
   efl::ecxx::grammar::eo_class_detail_generator<OutputIterator> detail;
};

} } }

#endif // EFL_ECXX_EO_CLASS_GENERATOR_HH
