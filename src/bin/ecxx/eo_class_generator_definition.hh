
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

  argument = 'a' << int_[&detail::next_arg];
  arguments_loop = *(argument % ", ");
  argument_decl = string[_1 = _val] << space << 'a' << int_[&detail::next_arg_decl];
  arguments_decl_loop = *(argument_decl % ", ");

  constructor = indent(1)
    << string[_1 = _r1]
    << '(' << arguments_decl_loop[_1 = at_c<1>(_val)] << ')' << eol
    << indent(2) << ": " << string[_1 = _r2] 
    << "(_c1(" << arguments_loop[_1 = at_c<1>(_val)] << "))" << eol
    << indent(1) << "{}" << eol;

  constructors_loop = *(constructor(_r1, _r2) << eol);

  destructor = indent(1)
    << '~' << string[_1 = _val] << "() {}" << eol;

  function = eps[_a = at_c<3>(_val)]                    // _a = return type
    << eps[_b = !(_a == "" || _a == "void") ]           // _b = return type != "void"
    << indent(1)
    << ( (eps(at_c<0>(_val) == eo_function::class_) << "static ") | "" )
    << ( (eps(_b) << string[_1 = _a] ) | "void" )
    << space << string[_1 = at_c<1>(_val)]
    << "(" << arguments_decl_loop[_1 = at_c<4>(_val)] << ")" << eol
    << indent(1) << "{" << eol
    << ( (eps(_b) << (indent(2) << string[_1 = _a] << " r;" << eol)) | "" )
    << indent(2) << "eo2_do(_eo_ptr(), "
    << ( (eps(_b) << "r = ") | "" )
    << "::" << string[_1 = at_c<1>(_val)]
    << "(" << arguments_loop[_1 = at_c<4>(_val)] << "));" << eol
    << ( (eps(_b) << (indent(2) << "return r;" << eol)) | "" )
    << indent(1) << "}" << eol;

  functions_loop = *(function << eol);

  start = eps[_a = at_c<0>(_val)]                       // _a = 'type'
    << eps[_b = at_c<1>(_val)]                          // _b = 'name'
    << eps[_c = at_c<2>(_val)]                          // _c = 'eo_name'
    << eps[_d = at_c<3>(_val)]                          // _d = 'parent'
    << "struct " << string[_1 = _b] << " {" << eol
    << constructor_eo[_1 = _val] << eol
    << constructors_loop(_b, _d)[_1 = at_c<5>(_val)]    // 5 = constructors
    << copy_constructor[_1 = _val] << eol
    << destructor[_1 = _b] << eol
    << functions_loop[_1 = at_c<6>(_val)]               // 6 = functions
    ;
}

} } }


#endif // EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
