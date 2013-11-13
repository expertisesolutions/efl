
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
  
  class_extension = indent(2) 
    << "efl::eo::detail::extension_inheritance<"
    << string[_1 = _val] << ">::type<" 
    << string[_1 = _r1] << ">";
  
  class_extensions_loop = *(class_extension(_r1) % (',' << eol));

  class_inheritance = 
    ( (eps(at_c<3>(_val) != "") << string[_1 = at_c<3>(_val)]) | "efl::eo::base" ) 
    << ',' << eol 
    << class_extensions_loop(at_c<1>(_val))[_1 = at_c<4>(_val)] << eol;

  argument = 'a' << int_[&detail::next_arg];
  arguments_loop = *(argument % ", ");
  argument_declaration = string[_1 = _val]
    << space << 'a' << int_[&detail::next_arg_decl];
  arguments_declaration_loop = *(argument_declaration % ", ");

  constructor_eo = indent(1)
    << "explicit " << string[_1 = at_c<1>(_val)] << "(Eo* eo)" << eol
    << indent(2) << ": " << string[_1 = at_c<3>(_val)] << "(eo)" << eol
    << indent(1) << "{}" << eol << eol;

  copy_constructor = indent(1)
    << string[_1 = at_c<1>(_val)]
    << "(" << string[_1 = at_c<1>(_val)] << " const& other)" << eol
    << indent(2) << ": " << string[_1 = at_c<3>(_val)]
    << "(eo_ref(other._eo_ptr()))" << eol
    << indent(1) << "{}" << eol << eol;

  constructor = indent(1)
    << string[_1 = _r1]
    << '(' << arguments_declaration_loop[_1 = at_c<1>(_val)] << ')' << eol
    << indent(2) << ": " << string[_1 = _r2]
    << "(_c1(" << arguments_loop[_1 = at_c<1>(_val)] << "))" << eol
    << indent(1) << "{}" << eol << eol;

  constructors_loop = *(constructor(_r1, _r2));

  destructor = indent(1)
    << '~' << string[_1 = _val] << "() {}" << eol << eol;

  function = eps[_a = at_c<3>(_val)]                    // _a = return type
    << eps[_b = !(_a == "" || _a == "void") ]           // _b = return type != "void"
    << indent(1)
    << ( (eps(at_c<0>(_val) == eo_function::class_) << "static ") | "" )
    << ( (eps(_b) << string[_1 = _a] ) | "void" )
    << space << string[_1 = at_c<1>(_val)]
    << "(" << arguments_declaration_loop[_1 = at_c<4>(_val)] << ")" << eol
    << indent(1) << "{" << eol
    << ( (eps(_b) << (indent(2) << string[_1 = _a] << " r;" << eol)) | "" )
    << indent(2) << "eo2_do(_eo_ptr(), "
    << ( (eps(_b) << "r = ") | "" )
    << "::" << string[_1 = at_c<1>(_val)]
    << "(" << arguments_loop[_1 = at_c<4>(_val)] << "));" << eol
    << ( (eps(_b) << (indent(2) << "return r;" << eol)) | "" )
    << indent(1) << "}" << eol << eol;

  functions_loop = *(function);

  eo_class_getter_definition = indent(1)
    << "static Eo_Class const* _eo_class()" << eol
    << indent(1) << "{" << eol
    << indent(2)
    << "return(EO3_GET_CLASS(" << string[_1 = _val] << "));" << eol
    << indent(1) << "}" << eol << eol;

  eo_class_constructor = indent(1)
    << "static Eo* _c1("
    << arguments_declaration_loop[_1 = at_c<1>(_val)]
    << ')' << eol
    << indent(1) << "{" << eol
    << indent(2) << "return eo2_add_custom(EO3_GET_CLASS("
    << string[_1 = _r1] << "), NULL, ::" << string[_1 = at_c<0>(_val)]
    << "(" << arguments_loop[_1 = at_c<1>(_val)] << "));" << eol
    << indent(1) << "}" << eol << eol;

  eo_class_constructors_loop = *(eo_class_constructor(_r1));

  start = eps[_a = at_c<0>(_val)]                       // _a = 'type'
    << eps[_b = at_c<1>(_val)]                          // _b = 'name'
    << eps[_c = at_c<3>(_val)]                          // _c = 'parent'
    << "struct " << string[_1 = _b] << eol
    << indent(2) << ": " << class_inheritance[_1 = _val]
    << '{' << eol 
    << constructor_eo[_1 = _val]
    << constructors_loop(_b, _c)[_1 = at_c<5>(_val)]    // 5 = constructors
    << copy_constructor[_1 = _val]
    << destructor[_1 = _b]
    << functions_loop[_1 = at_c<6>(_val)]               // 6 = functions
    << eo_class_getter_definition[_1 = at_c<2>(_val)]   // 2 = eo_name
    << "private:" << eol
    << eo_class_constructors_loop(at_c<2>(_val))[_1 = at_c<5>(_val)]
    << "};" << eol;
}

} } }


#endif // EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
