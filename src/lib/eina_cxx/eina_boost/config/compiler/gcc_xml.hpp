//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  GCC-XML C++ compiler setup:

#  if !defined(__GCCXML_GNUC__) || ((__GCCXML_GNUC__ <= 3) && (__GCCXML_GNUC_MINOR__ <= 3))
#     define EFL_EINA_BOOST_NO_IS_ABSTRACT
#  endif

//
// Threading support: Turn this on unconditionally here (except for
// those platforms where we can know for sure). It will get turned off again
// later if no threading API is detected.
//
#if !defined(__MINGW32__) && !defined(_MSC_VER) && !defined(linux) && !defined(__linux) && !defined(__linux__)
# define EFL_EINA_BOOST_HAS_THREADS
#endif

//
// gcc has "long long"
//
#define EFL_EINA_BOOST_HAS_LONG_LONG

// C++0x features:
//
#  define EFL_EINA_BOOST_NO_CXX11_CONSTEXPR
#  define EFL_EINA_BOOST_NO_CXX11_NULLPTR
#  define EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#  define EFL_EINA_BOOST_NO_CXX11_DECLTYPE
#  define EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276
#  define EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
#  define EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT
#  define EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES
#  define EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS
#  define EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS
#  define EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#  define EFL_EINA_BOOST_NO_CXX11_CHAR16_T
#  define EFL_EINA_BOOST_NO_CXX11_CHAR32_T
#  define EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#  define EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
#  define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#  define EFL_EINA_BOOST_NO_SFINAE_EXPR
#  define EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#  define EFL_EINA_BOOST_NO_CXX11_LAMBDAS
#  define EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#  define EFL_EINA_BOOST_NO_CXX11_RANGE_BASED_FOR
#  define EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS
#  define EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS
#  define EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#  define EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#  define EFL_EINA_BOOST_NO_CXX11_USER_DEFINED_LITERALS
#  define EFL_EINA_BOOST_NO_CXX11_ALIGNAS
#  define EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#  define EFL_EINA_BOOST_NO_CXX11_INLINE_NAMESPACES

#define EFL_EINA_BOOST_COMPILER "GCC-XML C++ version " __GCCXML__


