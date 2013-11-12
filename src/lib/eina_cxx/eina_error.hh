#ifndef _EINA_ERROR_HH
#define _EINA_ERROR_HH

#if __cplusplus >= 201103L
#include <system_error>
#else
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#endif

namespace efl { namespace eina {

enum error_type {};

struct error_category : boost::system::error_category
{
  const char* name() const
  {
    return "eina";
  }

  bool equivalent(int code, boost::system::error_condition const& condition) const
  {
    return code == condition.value();
  }

  bool equivalent(boost::system::error_code const& code, int condition) const
  {
    return code.value() == condition;
  }

  std::string message(int condition) const
  {
    return ::eina_error_msg_get(condition);
  }
};

inline boost::system::error_category& eina_error_category()
{
  static error_category _error_category;
  return _error_category;
}

inline boost::system::error_code get_error_code()
{
  Eina_Error error = eina_error_get();
  if(error)
    {
      eina_error_set(0);
      return boost::system::error_code(error, eina_error_category());
    }
  else
    return boost::system::error_code();
}

inline boost::system::error_condition get_error_condition()
{
  Eina_Error error = eina_error_get();
  if(error)
    {
      eina_error_set(0);
      return boost::system::error_condition(error, eina_error_category());
    }
  else
    return boost::system::error_condition();
}

inline error_type get_error_code_enum()
{
  return static_cast<error_type>( ::eina_error_get() );
}


inline void throw_on_error()
{
  boost::system::error_code ec = get_error_code();
  if(ec)
  {
    throw boost::system::system_error(ec, "EFL Eina Error");
  }
}

} }

#if __cplusplus >= 201103
namespace std {

template <> struct is_error_condition_enum< ::efl::eina::error_type> : true_type {};
template <> struct is_error_code_enum< ::efl::eina::error_type> : true_type {};

}
#else
namespace boost { namespace system {

template <> struct is_error_condition_enum< ::efl::eina::error_type>
 : boost::mpl::true_ {};
template <> struct is_error_code_enum< ::efl::eina::error_type>
 : boost::mpl::true_ {};

} }
#endif

#endif
