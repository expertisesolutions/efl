#include <string>

#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace efl { namespace ecxx { 

struct eo_constructor;
struct eo_destructor;
struct eo_function;
struct eo_function_override;
struct eo_event;

typedef boost::variant<
  eo_constructor,
  eo_destructor,
  eo_function,
  eo_function_override,
  eo_event>
eo_class_member;

typedef std::vector<eo_class_member> eo_class_members;

struct eo_class
{
  enum eo_class_type
  {
    regular_, regular_noninst_, interface_, mixin_
  };
  eo_class_type type;
  std::string name;
  eo_class_members members;
};

struct eo_function
{
  enum eo_function_type 
  { 
    regular_, class_
  };
  eo_function_type type;
  std::string name;
};

struct eo_constructor {};
struct eo_destructor {};
struct eo_function_override {};
struct eo_event {};

} } 
 
BOOST_FUSION_ADAPT_STRUCT(::efl::ecxx::eo_class,
  (::efl::ecxx::eo_class::eo_class_type, type)
  (std::string, name)
  (::efl::ecxx::eo_class_members, members));

BOOST_FUSION_ADAPT_STRUCT(::efl::ecxx::eo_function,
  (::efl::ecxx::eo_function::eo_function_type, type)
  (std::string, name));




