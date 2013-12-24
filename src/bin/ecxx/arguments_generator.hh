
#ifndef EFL_ECXX_COMMON_GENERATOR_HH
#define EFL_ECXX_COMMON_GENERATOR_HH

#include <string>
#include <vector>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "eo_type_def.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct argument_generator
  : karma::grammar<OutputIterator, std::string(int)>
{
   argument_generator();
   karma::rule<OutputIterator, std::string(int)> start;
};

template <typename OutputIterator>
struct arguments_declaration_generator
  : karma::grammar<OutputIterator, std::vector<std::string>(), karma::locals<int> >
{
   arguments_declaration_generator();
   karma::rule<OutputIterator, std::vector<std::string>(), karma::locals<int> > start;
   karma::rule<OutputIterator, std::string(int)> argument_declaration;
};

template <typename OutputIterator>
struct arguments_prepended_declaration_generator
  : karma::grammar<OutputIterator, std::vector<std::string>()>
{
   arguments_prepended_declaration_generator();
   karma::rule<OutputIterator, std::vector<std::string>()> start;
};

template <typename OutputIterator>
struct arguments_types_generator
  : karma::grammar<OutputIterator, std::vector<std::string>()>
{
   arguments_types_generator();
   karma::rule<OutputIterator, std::vector<std::string>()> start;
   karma::rule<OutputIterator, std::string()> argument_type;
};

template <typename OutputIterator>
struct arguments_prepended_types_generator
  : karma::grammar<OutputIterator, std::vector<std::string>()>
{
   arguments_prepended_types_generator();
   karma::rule<OutputIterator, std::vector<std::string>()> start;
};

template <typename OutputIterator>
struct arguments_list_generator
  : karma::grammar<OutputIterator, std::vector<std::string>(), karma::locals<int> >
{
   arguments_list_generator();
   karma::rule<OutputIterator, std::vector<std::string>(), karma::locals<int> > start;
};

} } }

#endif // EFL_ECXX_COMMON_GENERATOR_HH



