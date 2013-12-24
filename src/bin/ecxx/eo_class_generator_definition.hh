
#ifndef EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH

#include "eo_class_generator.hh"

#include "arguments_generator.hh"
#include "eo_class_detail_generator.hh"
#include "eo_class_event_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
eo_class_generator<OutputIterator>::eo_class_generator()
  : eo_class_generator::base_type(start)
{
  using phoenix::at_c;
  using karma::_a; using karma::_b; using karma::_c; using karma::_d;
  using karma::_r1; using karma::_r2;
  using karma::_1;
  using karma::_val;
  using karma::eol;
  using karma::eps;
  using karma::string;

  tab = karma::repeat(2*_r1)[karma::space];

  class_extension = tab(2)
    << "efl::eo::detail::extension_inheritance<"
    << string[_1 = _val] << ">::type<"
    << string[_1 = _r1] << ">";

  class_extensions_loop = *(class_extension(_r1) % (',' << eol));

  class_inheritance =
    ( (eps(at_c<3>(_val) != "") << string[_1 = at_c<3>(_val)]) | "efl::eo::base" )
    << (eps(phoenix::size(at_c<4>(_val)) > 0)
      << ',' << eol << class_extensions_loop(at_c<1>(_val))[_1 = at_c<4>(_val)]
      | "")
    << eol;

  constructor_eo = tab(1)
    << "explicit " << string[_1 = at_c<1>(_val)] << "(Eo* eo)" << eol
    << tab(2) << ": " << string[_1 = at_c<3>(_val)] << "(eo)" << eol
    << tab(1) << "{}" << eol << eol;

  copy_constructor = tab(1)
    << string[_1 = at_c<1>(_val)]
    << "(" << string[_1 = at_c<1>(_val)] << " const& other)" << eol
    << tab(2) << ": " << string[_1 = at_c<3>(_val)]
    << "(eo_ref(other._eo_ptr()))" << eol
    << tab(1) << "{}" << eol << eol;

  constructor = tab(1)
    << string[_1 = _r1]
    << '(' << arguments_declaration[_1 = at_c<1>(_val)] << ')' << eol
    << tab(2) << ": " << string[_1 = _r2]
    << "(_c1(" << arguments_list[_1 = at_c<1>(_val)] << "))" << eol
    << tab(1) << "{}" << eol << eol;

  constructors_loop = *(constructor(_r1, _r2));

  destructor = tab(1)
    << '~' << string[_1 = _val] << "() {}" << eol << eol;

  function = eps[_a = at_c<3>(_val)]                    // _a = return type
    << eps[_b = !(_a == "" || _a == "void") ]           // _b = return type != "void"
    << tab(1)
    << ( (eps(at_c<0>(_val) == eo_function::class_) << "static ") | "" )
    << ( (eps(_b) << string[_1 = _a] ) | "void" )
    << karma::space << string[_1 = at_c<1>(_val)]
    << "(" << arguments_declaration[_1 = at_c<4>(_val)] << ")" << eol
    << tab(1) << "{" << eol
    << ( (eps(_b) << (tab(2) << string[_1 = _a]
          << " r = (" << string[_1 = _a] << ") 0;" << eol)) | "" )
    << tab(2) << "eo2_do(_eo_ptr(), "
    << ( (eps(_b) << "r = ") | "" )
    << "::" << string[_1 = at_c<1>(_val)]
    << "(" << arguments_list[_1 = at_c<4>(_val)] << "));" << eol
    << ( (eps(_b) << (tab(2) << "return r;" << eol)) | "" )
    << tab(1) << "}" << eol << eol;

  functions_loop = *(function);

  eo_class_getter_definition = tab(1)
    << "static Eo_Class const* _eo_class()" << eol
    << tab(1) << "{" << eol
    << tab(2) << "return(EO3_GET_CLASS("<< string[_1 = _val] << "));" << eol
    << tab(1) << "}" << eol << eol;

  eo_class_constructor = tab(1)
    << "static Eo* _c1("
    << arguments_declaration[_1 = at_c<1>(_val)]
    << ')' << eol
    << tab(1) << "{" << eol
    << tab(2) << "return eo2_add_custom(EO3_GET_CLASS("
    << string[_1 = _r1] << "), NULL, ::" << string[_1 = at_c<0>(_val)]
    << "(" << arguments_list[_1 = at_c<1>(_val)] << "));" << eol
    << tab(1) << "}" << eol << eol;

  eo_class_constructors_loop = *(eo_class_constructor(_r1));

  start = eps[_a = at_c<0>(_val)]                       // _a = 'type'
    << eps[_b = at_c<1>(_val)]                          // _b = 'name'
    << eps[_c = at_c<3>(_val)]                          // _c = 'parent'
    << "struct " << string[_1 = _b] << eol
    << tab(2) << ": " << class_inheritance[_1 = _val]
    << '{' << eol
    << constructor_eo[_1 = _val]
    << constructors_loop(_b, _c)[_1 = at_c<5>(_val)]    // 5 = constructors
    << copy_constructor[_1 = _val]
    << destructor[_1 = _b]
    << functions_loop[_1 = at_c<6>(_val)]               // 6 = functions
    << eo_class_getter_definition[_1 = at_c<2>(_val)]   // 2 = eo_name
    << events[_1 = at_c<7>(_val)]                       // 7 = events
    << "private:" << eol
    << eo_class_constructors_loop(at_c<2>(_val))[_1 = at_c<5>(_val)]
      << "};" << eol << eol;
//<< detail[_1 = _val];
}

} } }

#endif // EFL_ECXX_EO_CLASS_GENERATOR_DEFINITION_HH
