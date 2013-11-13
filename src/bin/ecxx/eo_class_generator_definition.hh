
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH

#include <string>

#include "eo_class_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

namespace detail {
void next_arg_decl(int& i) { static int c = 0; i = c++; } // XXX
void next_arg(int& i) { static int c = 0; i = c++; } // XXX
}

template <typename OutputIterator>
eo_class_generator<OutputIterator>::eo_class_generator() 
  : eo_class_generator::base_type(start)
{
  using namespace boost::spirit;
  using namespace boost::spirit::ascii;

  using phoenix::at_c;
  using karma::_a; using karma::_b; using karma::_c; using karma::_d;
  using karma::_r1; using karma::_r2;
  using karma::_1;
  using karma::_val;
  using karma::eol;
  using karma::eps;
  using karma::space;
  using karma::string;

  indent = karma::repeat(2*_r1)[space];

  constructor_eo = indent(1)
    << "explicit " << string[_1 = at_c<1>(_val)] << "(Eo* eo)" << eol
    << indent(2) << ": " << string[_1 = at_c<3>(_val)] << "(eo)" << eol
    << indent(1) << "{}" << eol;

  copy_constructor = indent(1)
    << string[_1 = at_c<1>(_val)] 
    << "(" << string[_1 = at_c<1>(_val)] << " const& other)" << eol
    << indent(2) << ": " << string[_1 = at_c<3>(_val)] 
    << "(eo_ref(other._eo_ptr()))" << eol
    << indent(1) << "{}" << eol;

  constructor_arg = 'a' << int_[&detail::next_arg];

  constructor_args_loop = *(constructor_arg % ", ");
 
  constructor_arg_decl = string[_1 = _val] 
    << space << 'a' << int_[&detail::next_arg_decl];

  constructor_args_decl_loop = *(constructor_arg_decl % ", ");

  constructor = indent(1)
    << string[_1 = _r1]
    << '(' << constructor_args_decl_loop[_1 = at_c<1>(_val)] << ')' << eol
    << indent(2) << ": " << string[_1 = _r2] 
    << "(_c1(" << constructor_args_loop[_1 = at_c<1>(_val)] << "))" << eol
    << indent(1) << "{}" << eol;

  constructors_loop = *(constructor(_r1, _r2) << eol);

  destructor = indent(1)
    << '~' << string[_1 = _val] << "() {}" << eol;

  start = eps[_a = at_c<0>(_val)]                       // _a = 'type'
    << eps[_b = at_c<1>(_val)]                          // _b = 'name'
    << eps[_c = at_c<2>(_val)]                          // _c = 'eo_name'
    << eps[_d = at_c<3>(_val)]                          // _d = 'parent'
    << "struct " << string[_1 = _b] << " {" << eol
    << constructor_eo[_1 = _val] << eol
    << constructors_loop(_b, _d)[_1 = at_c<5>(_val)]
    << copy_constructor[_1 = _val] << eol
    << destructor[_1 = _b] << eol
    ;
}

} } }


#endif // EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
