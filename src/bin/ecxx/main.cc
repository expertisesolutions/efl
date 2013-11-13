
#include <iostream>
#include <boost/spirit/home/karma.hpp>
#include <boost/assign/list_of.hpp>

#include "common_types.hh"
#include "eo_class_generator.hh"

namespace { // XXX

efl::ecxx::eo_class get_next()
{
  efl::ecxx::eo_constructor c1 =
    { "simple_constructor", boost::assign::list_of("int") };
  efl::ecxx::eo_constructor c2 =
    { "multi_constructor", boost::assign::list_of("int")("float") };
  efl::ecxx::eo_constructor c3 =
    { "default_constructor", std::vector<std::string>() };
  efl::ecxx::eo_function f1 =
    { efl::ecxx::eo_function::regular_, "simple_inc", "_inc", "void"
    , std::vector<std::string>() };
  efl::ecxx::eo_function f2 =
    { efl::ecxx::eo_function::regular_, "simple_set", "_set", "void"
    , boost::assign::list_of("int") };
  efl::ecxx::eo_function f3 =
    { efl::ecxx::eo_function::regular_, "simple_get", "_get", "int"
    , std::vector<std::string>() };
  efl::ecxx::eo_function f4 =
    { efl::ecxx::eo_function::class_, "simple_print", "_print", "void"
    , std::vector<std::string>() };
  efl::ecxx::eo_function f5 =
    { efl::ecxx::eo_function::class_, "variate_print", "_print", "void"
    , boost::assign::list_of("std::string")("int")("double") };

  efl::ecxx::eo_class k =
    {
      efl::ecxx::eo_class::regular_,
      "eo3_simple", "EO3_SIMPLE_CLASS",
      "efl::eo::base", std::vector<std::string>(),
      boost::assign::list_of(c1)(c2)(c3),
      boost::assign::list_of(f1)(f2)(f3)(f4)(f5),
      std::vector<efl::ecxx::eo_event>()
    };
  return k;
}

}

int main(int argc, char **argv)
{
  namespace karma = boost::spirit::karma;
  typedef efl::ecxx::grammar::eo_class_generator
    <efl::ecxx::output_iterator_type> eo_class_generator;

  std::string s;
  efl::ecxx::output_iterator_type o(s);
  efl::ecxx::grammar::eo_class_generator<efl::ecxx::output_iterator_type> g;
  g.name("grammar::eo_class_generator");

  karma::generate(o, g, ::get_next());

  std::cout << s << std::endl;

  return 0;
}
