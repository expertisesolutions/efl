
#ifndef EFL_ECXX_ARGUMENTS_GENERATOR_DEFINITION_HH
#define EFL_ECXX_ARGUMENTS_GENERATOR_DEFINITION_HH

#include "arguments_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
argument_generator<OutputIterator>::argument_generator()
  : argument_generator::base_type(start)
{
   start = 'a' << karma::lit(karma::_r1);
};

template <typename OutputIterator>
arguments_declaration_generator<OutputIterator>::arguments_declaration_generator()
  : arguments_declaration_generator::base_type(start)
{
   argument_declaration = karma::string[karma::_1 = karma::_val]
     << karma::space << 'a' << karma::lit(karma::_r1);
   start = *(argument_declaration(karma::_a++) % ", ");
};

template <typename OutputIterator>
arguments_prepended_declaration_generator<OutputIterator>::arguments_prepended_declaration_generator()
  : arguments_prepended_declaration_generator::base_type(start)
{
   efl::ecxx::grammar::arguments_declaration_generator<OutputIterator> arguments_declaration;
   start = karma::eps(karma::_val != std::vector<std::string>())
     << ", " << arguments_declaration[karma::_1 = karma::_val]
     | karma::string[karma::_1 = ""];
};

template <typename OutputIterator>
arguments_types_generator<OutputIterator>::arguments_types_generator()
  : arguments_types_generator::base_type(start)
{
   argument_type = karma::string[karma::_1 = karma::_val];
   start = *(argument_type % ", ");
};

template <typename OutputIterator>
arguments_prepended_types_generator<OutputIterator>::arguments_prepended_types_generator()
  : arguments_prepended_types_generator::base_type(start)
{
   efl::ecxx::grammar::arguments_types_generator<OutputIterator> arguments_type;
   start = karma::eps(karma::_val != std::vector<std::string>())
     << ", " << arguments_type[karma::_1 = karma::_val]
     | karma::string[karma::_1 = ""];
};

template <typename OutputIterator>
arguments_list_generator<OutputIterator>::arguments_list_generator()
  : arguments_list_generator::base_type(start)
{
   efl::ecxx::grammar::argument_generator<OutputIterator> argument;
   start = *(argument(karma::_a++) % ", ");
};

} } }

#endif // EFL_ECXX_ARGUMENTS_GENERATOR_DEFINITION_HH

