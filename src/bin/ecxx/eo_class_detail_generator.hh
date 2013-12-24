
#ifndef EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_HH
#define EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_HH

#include <string>
#include <vector>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "eo_type_def.hh"

namespace efl { namespace ecxx { namespace grammar {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct eo_class_detail_generator : karma::grammar<OutputIterator, eo_class()>
{
   eo_class_detail_generator();
   karma::rule<OutputIterator, eo_class()> start;
   karma::rule<OutputIterator, void(unsigned int)> tab;
   karma::rule<OutputIterator, eo_function()> eo_operation_wrapper;
   karma::rule<OutputIterator, std::vector<eo_function>()> eo_operations_wrappers_loop;
   karma::rule<OutputIterator, eo_function(), karma::locals<std::string, bool> > virtual_operation;
   karma::rule<OutputIterator, std::vector<eo_function>()> virtual_operations_loop;
   karma::rule<OutputIterator, eo_class()> class_operations;
   karma::rule<OutputIterator, eo_class(), karma::locals<unsigned int> > operation_description_class_size;
   karma::rule<OutputIterator, eo_function(int)> operation_description;
   karma::rule<OutputIterator, std::vector<eo_function>(), karma::locals<int> > operations_descriptions_loop;
   karma::rule<OutputIterator, eo_class()> initialize_operation_description;
   karma::rule<OutputIterator, std::string(int)> class_constructor_argument;
   karma::rule<OutputIterator, std::vector<std::string>(), karma::locals<int> > class_constructor_arguments_loop;
   karma::rule<OutputIterator, eo_constructor(std::string)> class_constructor;
   karma::rule<OutputIterator, std::vector<eo_constructor>(std::string)> class_constructors_loop;
   karma::rule<OutputIterator, eo_class()> get_eo_class;
   karma::rule<OutputIterator, eo_function()> extension_inheritance_function;
   karma::rule<OutputIterator, std::vector<eo_function>()> extension_inheritance_functions_loop;
   karma::rule<OutputIterator, eo_class()> extension_inheritance;
};

} } }

#endif // EFL_ECXX_EO_CLASS_DETAIL_GENERATOR_HH



