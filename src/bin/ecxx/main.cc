
#include <iostream>
#include <boost/spirit/home/karma.hpp>

#include "eo_class_generator.hh"

efl::ecxx::eo_class __get_next()
{
  efl::ecxx::eo_function f = { efl::ecxx::eo_function::regular_, "simple_inc" };
  efl::ecxx::eo_class_members m; m.push_back(f);
  efl::ecxx::eo_class c = { efl::ecxx::eo_class::regular_, "eo3_simple", m};
  return c;
}

int main(int argc, char **argv)
{
  namespace karma = boost::spirit::karma;
  
  typedef std::back_insert_iterator<std::string> out_iter_type;
  typedef efl::ecxx::eo_class_generator<out_iter_type> eo_class_generator;
  std::string s;
  out_iter_type o(s);
  eo_class_generator g;
  
  karma::generate(o, g, __get_next());
  
  std::cout << s << std::endl;
  
  return 0;
}

