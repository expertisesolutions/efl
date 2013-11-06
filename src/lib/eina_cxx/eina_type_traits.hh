#ifndef _EINA_TYPE_TRAITS_HH
#define _EINA_TYPE_TRAITS_HH

#if __cplusplus >= 201103
#include <type_traits>
#else
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/remove_cv.hpp>
#endif

namespace efl { namespace eina {

#if __cplusplus >= 201103
using std::enable_if;
using std::enable_if_c;
using std::is_integral;
using std::remove_cv;
#else
using boost::enable_if;
using boost::enable_if_c;
using boost::is_integral;
using boost::remove_cv;
#endif

} }

#endif
