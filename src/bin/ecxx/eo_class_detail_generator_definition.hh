
#ifndef EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH

#include "eo_class_detail_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

namespace detail {
inline void next_arg_decl(int& i) { static int c = 0; i = c++; } // XXX
inline void next_arg(int& i) { static int c = 0; i = c++; } // XXX
}

template <typename OutputIterator>
eo_class_detail_generator<OutputIterator>::eo_class_detail_generator()
  : eo_class_detail_generator::base_type(start)
{
  using namespace boost::spirit;
  using namespace boost::spirit::ascii;

  using phoenix::at_c;
  using karma::_1;
  using karma::_val;
  using karma::eol;
  using karma::eps;
  using karma::string;

  tab = karma::repeat(2*_r1)[karma::space];
  
  argument = 'a' << int_[&detail::next_arg];
  arguments_loop = *(argument % ", ");
  argument_declaration = string[_1 = _val]
    << karma::space << 'a' << int_[&detail::next_arg_decl];
  arguments_declaration_loop = *(argument_declaration % ", ");

  prepended_arguments_declaration =
    eps(_val != std::vector<std::string>())
    << ", " << arguments_declaration_loop[_1 = _val]
    | string[_1 = ""];
      
  eo_operation_wrapper = "template <typename T>" << eol
    << "void " << string[_1 = at_c<1>(_val)]
    << "_wrapper(Eo* objid EINA_UNUSED, "
    << "efl::eo::detail::Inherit_Private_Data* self"
    << prepended_arguments_declaration[_1 = at_c<4>(_val)] << ")" << eol
    << "{" << eol
    << tab(1) << "return static_cast<T*>(self->this_)->"
    << string[_1 = at_c<1>(_val)]
    << "(" << arguments_loop[_1 = at_c<4>(_val)] << ");" << eol
    << "}" << eol << eol;
  
  eo_operations_wrappers_loop = *(eo_operation_wrapper);

  virtual_operation = eps[_a = at_c<3>(_val)]           // _a = return type
    << eps[_b = !(_a == "" || _a == "void") ]           // _b = return type != "void"
    << tab(2) << "virtual " << string[_1 = at_c<3>(_val)] << karma::space
    << string[_1 = at_c<1>(_val)] << "("
    << arguments_declaration_loop[_1 = at_c<4>(_val)] << ")" << eol
    << tab(2) << "{" << eol
    << ( (eps(_b) << (tab(3) << string[_1 = _a] << " r = ("
          << string[_1 = _a] << ") 0;" << eol)) | "" )
    << tab(3) << "eo2_do_super(static_cast<T*>(this)->_eo_ptr()" << eol
    << tab(4) << ", static_cast<T*>(this)->_eo_class()" << eol
    << tab(4) << ", " << ( (eps(_b) << "r = ") | string[_1 = ""] )
    << "::" << string[_1 = at_c<1>(_val)]
    << "(" << arguments_loop[_1 = at_c<4>(_val)] << "));" << eol
    << ( (eps(_b) << ( tab(3) << "return r;" << eol)) | string[_1 = ""] )
    << tab(2) << "}" << eol << eol;
  
  virtual_operations_loop = *(virtual_operation);
  
  class_operations = "template<>" << eol
    << "struct operations<" << string[_1 = at_c<1>(_val)] << ">" << eol
    << "{" << eol
    << tab(1) << "template <typename D>" << eol
    << tab(1) << "struct type" << eol
    << tab(1) << "{" << eol
    << virtual_operations_loop[_1 = at_c<6>(_val)]
    << tab(1) << "};" << eol
    << "};" << eol << eol;

  operation_description_class_size = string[_1 = ""]; // TODO

  operation_description = string[_1 = ""]; // TODO

  operations_descriptions_loop = (*operation_description);
  
  initialize_operation_description = "template <typename T>" << eol
    << "void initialize_operation_description(efl::eo::detail::tag<"
    << string[_1 = at_c<1>(_val)] << ">" << eol
    << tab(7) << ", Eo2_Op_Description* ops)" << eol
    << "{" << eol
    << operations_descriptions_loop[_1 = at_c<6>(_val)]
    << "}" << eol << eol;

  class_constructor = "void call_constructor(tag<"
    << string[_1 = at_c<1>(_val)] << ">" << eol
    << tab(5) << ", Eo* eo, Eo_Class const* cls EINA_UNUSED" << eol
    << tab(5) << "args_class<" << string[_1 = at_c<1>(_val)] << ","
    << "boost::fusion::vector<int> > const& args)" << eol
    << "{" << eol
    // TODO XXX YYY
    //<< tab(1) << "eo2_do(eo, ::" << 
    ;

  get_eo_class = "inline Eo_Class const* get_eo_class(tag<"
    << string[_1 = at_c<1>(_val)] << ">)" << eol
    << "{" << eol
    << tab(1) << "return EO3_GET_CLASS(" << string[_1 = at_c<2>(_val)] 
    << ");" << eol
    << "}" << eol << eol;

  start = eo_operations_wrappers_loop[_1 = at_c<6>(_val)]
    << "namespace efl { namespace ecxx { namespace detail {" << eol << eol
    << class_operations[_1 = _val]
    << operation_description_class_size[_1 = _val]
    << initialize_operation_description[_1 = _val]
    << class_constructor[_1 = _val]
    << get_eo_class[_1 = _val]
    << "} } }" << eol;
};

} } }

#endif // EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH
