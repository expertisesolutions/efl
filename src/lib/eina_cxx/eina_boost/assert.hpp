//
//  boost/assert.hpp - EFL_EINA_BOOST_ASSERT(expr)
//                     EFL_EINA_BOOST_ASSERT_MSG(expr, msg)
//                     EFL_EINA_BOOST_VERIFY(expr)
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2007 Peter Dimov
//  Copyright (c) Beman Dawes 2011
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  Note: There are no include guards. This is intentional.
//
//  See http://www.boost.org/libs/utility/assert.html for documentation.
//

//
// Stop inspect complaining about use of 'assert':
//
// boostinspect:naassert_macro
//

//--------------------------------------------------------------------------------------//
//                                     EFL_EINA_BOOST_ASSERT                                     //
//--------------------------------------------------------------------------------------//

#undef EFL_EINA_BOOST_ASSERT

#if defined(EFL_EINA_BOOST_DISABLE_ASSERTS)

# define EFL_EINA_BOOST_ASSERT(expr) ((void)0)

#elif defined(EFL_EINA_BOOST_ENABLE_ASSERT_HANDLER)

#include <eina_boost/config.hpp>
#include <eina_boost/current_function.hpp>

namespace efl_eina_boost
{
  void assertion_failed(char const * expr,
                        char const * function, char const * file, long line); // user defined
} // namespace efl_eina_boost

#define EFL_EINA_BOOST_ASSERT(expr) (EFL_EINA_BOOST_LIKELY(!!(expr)) \
  ? ((void)0) \
  : ::efl_eina_boost::assertion_failed(#expr, EFL_EINA_BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same
# define EFL_EINA_BOOST_ASSERT(expr) assert(expr)
#endif

//--------------------------------------------------------------------------------------//
//                                   EFL_EINA_BOOST_ASSERT_MSG                                   //
//--------------------------------------------------------------------------------------//

# undef EFL_EINA_BOOST_ASSERT_MSG

#if defined(EFL_EINA_BOOST_DISABLE_ASSERTS) || defined(NDEBUG)

  #define EFL_EINA_BOOST_ASSERT_MSG(expr, msg) ((void)0)

#elif defined(EFL_EINA_BOOST_ENABLE_ASSERT_HANDLER)

  #include <eina_boost/config.hpp>
  #include <eina_boost/current_function.hpp>

  namespace efl_eina_boost
  {
    void assertion_failed_msg(char const * expr, char const * msg,
                              char const * function, char const * file, long line); // user defined
  } // namespace efl_eina_boost

  #define EFL_EINA_BOOST_ASSERT_MSG(expr, msg) (EFL_EINA_BOOST_LIKELY(!!(expr)) \
    ? ((void)0) \
    : ::efl_eina_boost::assertion_failed_msg(#expr, msg, EFL_EINA_BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
  #ifndef EFL_EINA_BOOST_ASSERT_HPP
    #define EFL_EINA_BOOST_ASSERT_HPP
    #include <cstdlib>
    #include <iostream>
    #include <eina_boost/config.hpp>
    #include <eina_boost/current_function.hpp>

    //  IDE's like Visual Studio perform better if output goes to std::cout or
    //  some other stream, so allow user to configure output stream:
    #ifndef EFL_EINA_BOOST_ASSERT_MSG_OSTREAM
    # define EFL_EINA_BOOST_ASSERT_MSG_OSTREAM std::cerr
    #endif

    namespace efl_eina_boost
    {
      namespace assertion
      {
        namespace detail
        {
          // Note: The template is needed to make the function non-inline and avoid linking errors
          template< typename CharT >
          EFL_EINA_BOOST_NOINLINE void assertion_failed_msg(CharT const * expr, char const * msg, char const * function,
            char const * file, long line)
          {
            EFL_EINA_BOOST_ASSERT_MSG_OSTREAM
              << "***** Internal Program Error - assertion (" << expr << ") failed in "
              << function << ":\n"
              << file << '(' << line << "): " << msg << std::endl;
#ifdef UNDER_CE
            // The Windows CE CRT library does not have abort() so use exit(-1) instead.
            std::exit(-1);
#else
            std::abort();
#endif
          }
        } // detail
      } // assertion
    } // detail
  #endif

  #define EFL_EINA_BOOST_ASSERT_MSG(expr, msg) (EFL_EINA_BOOST_LIKELY(!!(expr)) \
    ? ((void)0) \
    : ::efl_eina_boost::assertion::detail::assertion_failed_msg(#expr, msg, \
          EFL_EINA_BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#endif

//--------------------------------------------------------------------------------------//
//                                     EFL_EINA_BOOST_VERIFY                                     //
//--------------------------------------------------------------------------------------//

#undef EFL_EINA_BOOST_VERIFY

#if defined(EFL_EINA_BOOST_DISABLE_ASSERTS) || ( !defined(EFL_EINA_BOOST_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define EFL_EINA_BOOST_VERIFY(expr) ((void)(expr))

#else

# define EFL_EINA_BOOST_VERIFY(expr) EFL_EINA_BOOST_ASSERT(expr)

#endif
