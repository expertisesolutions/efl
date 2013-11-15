#ifndef EFL_ECXX_EO_TYPE_DEF_HH
#define EFL_ECXX_EO_TYPE_DEF_HH

#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>

namespace efl { namespace ecxx {

struct eo_constructor;
struct eo_function;
struct eo_event;

struct eo_class
{
  enum eo_class_type
  {
    regular_, regular_noninst_, interface_, mixin_
  };
  eo_class_type type;
  std::string name;
  std::string eo_name;
  std::string parent;
  std::vector<std::string> extensions;
  std::vector<eo_constructor> constructors;
  std::vector<eo_function> functions;
  std::vector<eo_event> events;
};

struct eo_constructor 
{
  std::string name;
  std::vector<std::string> params;
};

struct eo_function
{
  enum eo_function_type 
  { 
    regular_, class_
  };
  eo_function_type type;
  std::string name;
  std::string impl;
  std::string ret;
  std::vector<std::string> params;
};

struct eo_event
{
  std::string name;
  std::vector<std::string> params;
  bool is_hot;
};

} } 

// There's no need to describe 'constructor_overrides', 'destructors'
// and 'function_overrides'. EO (C subsystem) already provide the
// means to enable each of them -- also, the generated code does not
// depend on any user parameter.

BOOST_FUSION_ADAPT_STRUCT( ::efl::ecxx::eo_class,
  ( ::efl::ecxx::eo_class::eo_class_type, type )
  ( std::string, name )
  ( std::string, eo_name )
  ( std::string, parent )
  ( std::vector<std::string>, extensions )
  ( std::vector<efl::ecxx::eo_constructor>, constructors )
  ( std::vector<efl::ecxx::eo_function>, functions )
  ( std::vector<efl::ecxx::eo_event>, events) );

BOOST_FUSION_ADAPT_STRUCT( ::efl::ecxx::eo_constructor,
  ( std::string, name )
  ( std::vector<std::string>, params) );

BOOST_FUSION_ADAPT_STRUCT( ::efl::ecxx::eo_function,
  ( ::efl::ecxx::eo_function::eo_function_type, type )
  ( std::string, name )
  ( std::string, impl )
  ( std::string, ret )
  ( std::vector<std::string>, params) );

BOOST_FUSION_ADAPT_STRUCT( ::efl::ecxx::eo_event,
  ( std::string, name )
  ( std::vector<std::string>, params)
  ( bool, is_hot ));

#endif // EFL_ECXX_EO_TYPE_DEF_HH
