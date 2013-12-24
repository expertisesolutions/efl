
#ifndef EFL_ECXX_EO_CLASS_OPERATIONS_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_OPERATIONS_GENERATOR_DEFINITION_HH

#include "eo_class_operations_generator.hh"
#include "arguments_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
eo_class_operations_generator<OutputIterator>::eo_class_operations_generator()
  : eo_class_operations_generator::base_type(start)
{
   using phoenix::at_c;
   using karma::_1;
   using karma::_a;
   using karma::_b;
   using karma::_r1;
   using karma::_val;
   using karma::eol;
   using karma::eps;
   using karma::string;

   tab = karma::repeat(2*_r1)[karma::space];

   eo_operation_wrapper = "template <typename T>" << eol
     << string[_1 = at_c<3>(_val)] << " " << string[_1 = at_c<1>(_val)]
     << "_wrapper(Eo* objid EINA_UNUSED, "
     << "efl::eo::detail::Inherit_Private_Data* self"
     << arguments_prepended_declaration[_1 = at_c<4>(_val)] << ")" << eol
     << "{" << eol
     << tab(1) << "return static_cast<T*>(self->this_)->"
     << string[_1 = at_c<1>(_val)]
     << "(" << arguments_list[_1 = at_c<4>(_val)] << ");" << eol
     << "}" << eol << eol;

  eo_operations_wrappers_loop = *(eo_operation_wrapper);

  virtual_operation = eps[_a = at_c<3>(_val)]           // _a = return type
    << eps[_b = !(_a == "" || _a == "void") ]           // _b = return type != "void"
    << tab(2) << "virtual " << string[_1 = at_c<3>(_val)] << karma::space
    << string[_1 = at_c<1>(_val)] << "("
    << arguments_declaration[_1 = at_c<4>(_val)] << ")" << eol
    << tab(2) << "{" << eol
    << ( (eps(_b) << (tab(3) << string[_1 = _a] << " r = ("
          << string[_1 = _a] << ") 0;" << eol)) | "" )
    << tab(3) << "eo2_do_super(static_cast<T*>(this)->_eo_ptr()" << eol
    << tab(4) << ", static_cast<T*>(this)->_eo_class()" << eol
    << tab(4) << ", " << ( (eps(_b) << "r = ") | string[_1 = ""] )
    << "::" << string[_1 = at_c<1>(_val)]
    << "(" << arguments_list[_1 = at_c<4>(_val)] << "));" << eol
    << ( (eps(_b) << ( tab(3) << "return r;" << eol)) | string[_1 = ""] )
    << tab(2) << "}" << eol << eol;

  virtual_operations_loop = *(virtual_operation);

  class_operations = "template<>" << eol
    << "struct operations<" << string[_1 = at_c<1>(_val)] << ">" << eol
    << "{" << eol
    << tab(1) << "template <typename T>" << eol
    << tab(1) << "struct type" << eol
    << tab(1) << "{" << eol
    << virtual_operations_loop[_1 = at_c<6>(_val)]
    << tab(1) << "};" << eol
    << "};" << eol << eol;

  operation_description_class_size = "template<>"
    << eol << "struct operation_description_class_size<eo3_simple>" << eol
    << "{" << eol
    << tab(1) << "static const int value = "
    << karma::lit(phoenix::size(at_c<6>(_val)))
    << ";" << eol
    << "};" << eol << eol;

  operation_description = tab(1)
    << "ops[" << karma::lit(_r1) << "].func = reinterpret_cast<void*>(& ::"
    << string[_1 = at_c<1>(_val)] << "_wrapper<T>);" << eol
    << tab(1) << "ops[" << karma::lit(_r1) << "].api_func = reinterpret_cast<void*>(& ::"
    << string[_1 = at_c<1>(_val)] << ");" << eol
    << tab(1) << "ops[" << karma::lit(_r1) << "].op = EO2_OP_OVERRIDE;" << eol
    << tab(1) << "ops[" << karma::lit(_r1) << "].op_type = EO_OP_TYPE_REGULAR;" << eol  // XXX
    << tab(1) << "ops[" << karma::lit(_r1) << "].doc = NULL;" << eol                    // XXX
    << eol;

  operations_descriptions_loop = (*operation_description(_a++));

  initialize_operation_description = "template <typename T>"
    << eol << "void initialize_operation_description(efl::eo::detail::tag<"
    << string[_1 = at_c<1>(_val)] << ">" << eol
    << tab(7) << ", Eo2_Op_Description* ops)" << eol
    << "{" << eol << operations_descriptions_loop[_1 = at_c<6>(_val)]
    << "}" << eol << eol;

  start = eo_operations_wrappers_loop[_1 = at_c<6>(_val)]
    << "namespace efl { namespace eo { namespace detail {" << eol << eol
    << class_operations[_1 = _val]
    << operation_description_class_size[_1 = _val]
    << initialize_operation_description[_1 = _val]
    << "} } }" << eol;
};

} } }

#endif // EFL_ECXX_EO_CLASS_OPERATIONS_GENERATOR_DEFINITION_HH
