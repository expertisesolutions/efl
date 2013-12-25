
#ifndef EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH

#include "eo_class_detail_generator.hh"
#include "arguments_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
eo_class_detail_generator<OutputIterator>::eo_class_detail_generator()
  : eo_class_detail_generator::base_type(start)
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

   class_constructor_argument = "args.get<" << karma::lit(_r1) << ">()";

   class_constructor_arguments_loop = -(class_constructor_argument(_a++) % ", ");

   class_constructor = "void call_constructor(tag<"
    << string[_1 = _r1] << ">" << eol
    << tab(5) << ", Eo* eo, Eo_Class const* cls EINA_UNUSED," << eol
    << tab(5) << "args_class<" << string[_1 = _r1] << ","
    << "boost::fusion::vector<int> > const& args)" << eol
    << "{" << eol
    << tab(1) << "eo2_do(eo, ::" << string[_1 = at_c<0>(_val)] << "("
    << class_constructor_arguments_loop[_1 = at_c<1>(_val)]
    << "));" << eol
    << "}" << eol << eol;

   class_constructors_loop = (*class_constructor(_r1));

   get_eo_class = "inline Eo_Class const* get_eo_class(tag<"
    << string[_1 = at_c<1>(_val)] << ">)" << eol
    << "{" << eol
    << tab(1) << "return EO3_GET_CLASS(" << string[_1 = at_c<2>(_val)]
    << ");" << eol
    << "}" << eol << eol;

   extension_inheritance_function = tab(2)
    << string[_1 = at_c<3>(_val)] << " "
    << string[_1 = at_c<1>(_val)] << "("
    << arguments_declaration[_1 = at_c<4>(_val)]
    << ")" << eol
    << tab(2) << "{" << eol
    << tab(3) << "eo2_do(static_cast<T*>(this)->_eo_ptr(), ::"
    << string[_1 = at_c<1>(_val)]
    << "(" << arguments_list[_1 = at_c<4>(_val)] << "));" << eol
    << tab(2) << "}" << eol
    << eol;

   extension_inheritance_functions_loop = (*extension_inheritance_function);

   extension_inheritance = "template<>"
    << eol
    << "struct extension_inheritance< ::" << string[_1 = at_c<1>(_val)] << ">" << eol
    << "{" << eol
    << tab(1) << "template <typename T>" << eol
    << tab(1) << "struct type" << eol
    << tab(1) << "{" << eol
    << tab(2) << "operator " << string[_1 = at_c<1>(_val)] << "() const" << eol
    << tab(2) << "{" << eol
    << tab(3) << "return " << string[_1 = at_c<1>(_val)] << eol
    << tab(4) << "(eo_ref(static_cast<T const*>(this)->_eo_ptr()));" << eol
    << tab(2) << "}" << eol
    << eol
    << extension_inheritance_functions_loop[_1 = at_c<6>(_val)]
    << tab(1) << "};" << eol
    << "};" << eol
    << eol;

   start = operations[_1 = _val] << eol
    << "namespace efl { namespace eo { namespace detail {" << eol << eol
    << class_constructors_loop(at_c<1>(_val))[_1 = at_c<5>(_val)]
    << extension_inheritance[_1 = _val]
    << get_eo_class[_1 = _val]
    << "} } }" << eol;
};

} } }

#endif // EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_DEFINITION_HH
