
#ifndef EFL_ECXX_EO_CLASS_EVENT_GENERATOR_HH
#define EFL_ECXX_EO_CLASS_EVENT_GENERATOR_HH

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
struct eo_class_event_generator
  : karma::grammar<OutputIterator, std::vector<eo_event>()>
{
   eo_class_event_generator();
   karma::rule<OutputIterator, std::vector<eo_event>()> start;
   karma::rule<OutputIterator, eo_event()> event_callback;
   karma::rule<OutputIterator, eo_event()> event_callback_add;
   karma::rule<OutputIterator, eo_event()> event_callback_del;
   karma::rule<OutputIterator, eo_event()> event_callback_call;
   karma::rule<OutputIterator, void(unsigned int)> tab;
};

} } }

#endif // EFL_ECXX_EO_CLASS_EVENT_GENERATOR_HH





