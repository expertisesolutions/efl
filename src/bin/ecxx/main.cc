
#include <iostream>
#include <boost/spirit/home/karma.hpp>
#include <boost/assign/list_of.hpp>

#include "common_types.hh"
#include "eo_class_generator.hh"
#include "eo_class_detail_generator.hh"

namespace {

efl::ecxx::eo_class class1()
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
    { efl::ecxx::eo_function::class_, "some_other_func", "_other", "void"
    , boost::assign::list_of("std::string")("int")("double") };
  efl::ecxx::eo_event e1 =
    { "simple_set_event", boost::assign::list_of("int"), false };
  efl::ecxx::eo_event e2 =
    { "simple_print_event", std::vector<std::string>(), true };

  efl::ecxx::eo_class class1 =
    {
      efl::ecxx::eo_class::regular_,
      "eo3_inherit_interface", "EO3_INHERIT_INTERFACE_CLASS",
      "eo3_simple",
      boost::assign::list_of("eo3_interface")("eo3_simple_interface"),
      boost::assign::list_of(c1)(c2)(c3),
      boost::assign::list_of(f1)(f2)(f3)(f4)(f5),
      boost::assign::list_of(e1)(e2)
    };
  return class1;
}

}

int main(int argc, char **argv)
{
  namespace karma = boost::spirit::karma;
  typedef efl::ecxx::grammar::eo_class_generator
    <efl::ecxx::output_iterator_type> eo_class_generator;

  std::string s;
  efl::ecxx::output_iterator_type o(s);
  efl::ecxx::grammar::eo_class_generator<efl::ecxx::output_iterator_type> g_class;
  efl::ecxx::grammar::eo_class_detail_generator<efl::ecxx::output_iterator_type> g_detail;

  karma::generate(o, g_class, ::class1());
  karma::generate(o, g_detail, ::class1());

  std::cout << s << std::endl;

  return 0;
}
