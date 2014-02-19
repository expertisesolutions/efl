#ifndef _EINA_ERROR_HH
#define _EINA_ERROR_HH

#include <Eina.h>

#if defined(EFL_EINA_CXX11) && !defined(EFL_EINA_HAVE_BOOST_SYSTEM)
#include <system_error>
#elif defined(EFL_EINA_HAVE_BOOST_SYSTEM)
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/mpl/bool.hpp>
#else
#error Must have one of boost.system or be compiled in C++11-mode
#endif



namespace efl { namespace eina {

#if defined(EFL_EINA_CXX11) && !defined(EFL_EINA_HAVE_BOOST_SYSTEM)
using std::errc;
using std::system_error;
using std::error_code;
using std::error_condition;
typedef std::error_category system_error_category;

inline system_error_category const& get_generic_category()
{
  return ::std::generic_category();
}
inline system_error_category const& get_system_category()
{
  return ::std::system_category();
}
#elif defined(EFL_EINA_HAVE_BOOST_SYSTEM)
namespace errc = boost::system::errc;
using ::boost::system::system_error;
using ::boost::system::error_code;
using ::boost::system::error_condition;
typedef ::boost::system::error_category system_error_category;

inline system_error_category const& get_generic_category()
{
  return ::boost::system::get_generic_category();
}
inline system_error_category const& get_system_category()
{
  return ::boost::system::get_system_category();
}
#endif

enum error_type {};

struct error_category : system_error_category
{
  const char* name() const throw()
  {
    return "eina";
  }

  bool equivalent(int code, eina::error_condition const& condition) const throw()
  {
    return code == condition.value();
  }

  bool equivalent(eina::error_code const& code, int condition) const throw()
  {
    return code.value() == condition;
  }

  std::string message(int condition) const
  {
    const char* e = ::eina_error_msg_get(condition);
    return e? e : "::eina_error_msg_get returned NULL. No error message available";
  }
};

inline eina::system_error_category& eina_error_category()
{
  static error_category _error_category;
  return _error_category;
}

inline eina::error_code get_error_code()
{
  Eina_Error error = eina_error_get();
  if(error)
    {
      eina_error_set(0);
      return eina::error_code(error, eina_error_category());
    }
  else
    return eina::error_code();
}

inline eina::error_condition get_error_condition()
{
  Eina_Error error = eina_error_get();
  if(error)
    {
      eina_error_set(0);
      return eina::error_condition(error, eina_error_category());
    }
  else
    return eina::error_condition();
}

inline error_type get_error_code_enum()
{
  return static_cast<error_type>( ::eina_error_get() );
}


inline void throw_on_error()
{
  eina::error_code ec = get_error_code();
  if(ec)
  {
    throw eina::system_error(ec, "EFL Eina Error");
  }
}

} }

#if !defined(EFL_EINA_HAVE_BOOST_SYSTEM)
namespace std {

template <> struct is_error_condition_enum< ::efl::eina::error_type> : true_type {};
template <> struct is_error_code_enum< ::efl::eina::error_type> : true_type {};

}
#else
namespace boost { namespace system {

template <> struct is_error_condition_enum< ::efl::eina::error_type>
 : ::boost::mpl::true_ {};
template <> struct is_error_code_enum< ::efl::eina::error_type>
 : ::boost::mpl::true_ {};

} }
#endif

#endif
