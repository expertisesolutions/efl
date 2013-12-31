
#include "arguments_generator_definition.hh"
#include "common_types.hh"

template struct
efl::ecxx::grammar::argument_generator< ::efl::ecxx::output_iterator_type>;

template struct
efl::ecxx::grammar::arguments_declaration_generator< ::efl::ecxx::output_iterator_type>;

template struct
efl::ecxx::grammar::arguments_prepended_declaration_generator< ::efl::ecxx::output_iterator_type>;

template struct
efl::ecxx::grammar::arguments_types_generator< ::efl::ecxx::output_iterator_type>;

template struct
efl::ecxx::grammar::arguments_prepended_types_generator< ::efl::ecxx::output_iterator_type>;

template struct
efl::ecxx::grammar::arguments_list_generator< ::efl::ecxx::output_iterator_type>;

