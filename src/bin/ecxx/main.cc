
#include <iostream>
#include <boost/spirit/home/karma.hpp>
#include <boost/assign/list_of.hpp>

#include "eo_class_generator.hh"

namespace { // XXX
  
efl::ecxx::eo_class get_next()
{
  efl::ecxx::eo_constructor c = 
    { "simple_constructor", boost::assign::list_of("int") };
  efl::ecxx::eo_function f = 
    { efl::ecxx::eo_function::regular_, "simple_inc", "_inc", "void", };
  efl::ecxx::eo_class k = 
    { efl::ecxx::eo_class::regular_, "eo3_simple", "EO3_BASE_CLASS", std::vector<std::string>(), boost::assign::list_of(c), boost::assign::list_of(f), std::vector<efl::ecxx::eo_event>() };
  return k;
}

}

int main(int argc, char **argv)
{
  namespace karma = boost::spirit::karma;
  
  typedef std::back_insert_iterator<std::string> out_iter_type;
  typedef efl::ecxx::grammar::eo_class_generator<out_iter_type> eo_class_generator;
  std::string s;
  out_iter_type o(s);
  eo_class_generator g;
  
  karma::generate(o, g, ::get_next());
  
  std::cout << s << std::endl;

  return 0;
}

