
#ifndef EFL_ECXX_EO_CLASS_EVENT_GENERATOR_DEFINITION_HH
#define EFL_ECXX_EO_CLASS_EVENT_GENERATOR_DEFINITION_HH

#include "arguments_generator.hh"
#include "eo_class_event_generator.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
eo_class_event_generator<OutputIterator>::eo_class_event_generator()
  : eo_class_event_generator::base_type(start)
{
   using karma::_1;
   using karma::_a;
   using karma::_r1;
   using karma::_val;
   using phoenix::at_c;
   using karma::eol;
   using karma::eps;
   using karma::string;

   efl::ecxx::grammar::arguments_declaration_generator<OutputIterator> arguments_declaration;
   efl::ecxx::grammar::arguments_list_generator<OutputIterator> arguments_list;
   efl::ecxx::grammar::arguments_prepended_types_generator<OutputIterator> arguments_prepended_types;

   tab = karma::repeat(2*_r1)[karma::space];

   event_callback_add = tab(1) << "template <typename F>" << eol
    << tab(1) << "::efl::eo::callback_token " << string[_1 = at_c<0>(_val)]
    << "_callback_add(F function)" << eol
    << tab(1) << "{" << eol
    << tab(2) << "std::auto_ptr<F> f(new F(function));" << eol
    << tab(2) << "::efl::eo::callback_token t" << eol
    << tab(3) << "= { f.get()" << eol
    << tab(3) << ", reinterpret_cast<void*>(static_cast<Eina_Bool(*)(void*"
    << arguments_prepended_types[_1 = at_c<1>(_val)]
    << ")>(&::efl::eo::detail::callback_function_object1<F"
    << arguments_prepended_types[_1 = at_c<1>(_val)] << ">))" << eol
    << tab(3) << ", &::efl::eo::detail::free_callback<F>" << eol
    << tab(3) << "};" << eol
    << tab(2) << "eo2_do(_eo_ptr(), ::"
    << string[_1 = at_c<0>(_val)] << "_callback_add" << eol
    << tab(3) << "(f.release(), &::efl::eo::detail::callback_function_object1<F"
    << arguments_prepended_types[_1 = at_c<1>(_val)] << ">));" << eol
    << tab(2) << "return t;" << eol
    << tab(1) << "}" << eol << eol;

  event_callback_del = tab(1) << "void " << string[_1 = at_c<0>(_val)]
    << "_callback_del(::efl::eo::callback_token t)" << eol
    << tab(1) << "{" << eol
    << tab(2) << "typedef Eina_Bool(*function_type)(void*"
    << arguments_prepended_types[_1 = at_c<1>(_val)] << ");" << eol
    << tab(2) << "eo2_do(_eo_ptr(), ::" << string[_1 = at_c<0>(_val)]
    << "_callback_del(t.data, (function_type)t.function));" << eol
    << tab(2) << "t.free_function(t.data);" << eol
    << tab(1) << "}" << eol << eol;

  event_callback_call = tab(1)
    << "void " << string[_1 = at_c<0>(_val)] << "_callback_call("
    << arguments_declaration[_1 = at_c<1>(_val)] << ")" << eol
    << tab(1) << "{" << eol
    << tab(2) << "eo2_do(_eo_ptr(), ::"
    << string[_1 = at_c<0>(_val)] << "_callback_call("
    << arguments_list[_1 = at_c<1>(_val)] << "));" << eol
    << tab(1) << "}" << eol << eol;

  event_callback = event_callback_add[_1 = _val]
    << event_callback_del[_1 = _val]
    << event_callback_call[_1 = _val];

  start = *(event_callback);
};

} } }

#endif // EFL_ECXX_EO_CLASS_EVENT_GENERATOR_DEFINITION_HH

