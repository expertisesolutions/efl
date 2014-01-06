
//  (C) Copyright John maddock 1999. 
//  (C) David Abrahams 2002.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// use this header as a workaround for missing <limits>

//  See http://www.boost.org/libs/compatibility/index.html for documentation.

#ifndef EFL_EINA_BOOST_LIMITS
#define EFL_EINA_BOOST_LIMITS

#include <eina_boost/config.hpp>

#ifdef EFL_EINA_BOOST_NO_LIMITS
#  error "There is no std::numeric_limits suppport available."
#else
# include <limits>
#endif

#if (defined(EFL_EINA_BOOST_HAS_LONG_LONG) && defined(EFL_EINA_BOOST_NO_LONG_LONG_NUMERIC_LIMITS)) \
      || (defined(EFL_EINA_BOOST_HAS_MS_INT64) && defined(EFL_EINA_BOOST_NO_MS_INT64_NUMERIC_LIMITS))
// Add missing specializations for numeric_limits:
#ifdef EFL_EINA_BOOST_HAS_MS_INT64
#  define EFL_EINA_BOOST_LLT __int64
#  define EFL_EINA_BOOST_ULLT unsigned __int64
#else
#  define EFL_EINA_BOOST_LLT  ::efl_eina_boost::long_long_type
#  define EFL_EINA_BOOST_ULLT  ::efl_eina_boost::ulong_long_type
#endif

#include <climits>  // for CHAR_BIT

namespace std
{
  template<>
  class numeric_limits<EFL_EINA_BOOST_LLT> 
  {
   public:

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_specialized = true);
#ifdef EFL_EINA_BOOST_HAS_MS_INT64
      static EFL_EINA_BOOST_LLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0x8000000000000000i64; }
      static EFL_EINA_BOOST_LLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0x7FFFFFFFFFFFFFFFi64; }
#elif defined(LLONG_MAX)
      static EFL_EINA_BOOST_LLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return LLONG_MIN; }
      static EFL_EINA_BOOST_LLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return LLONG_MAX; }
#elif defined(LONGLONG_MAX)
      static EFL_EINA_BOOST_LLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return LONGLONG_MIN; }
      static EFL_EINA_BOOST_LLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return LONGLONG_MAX; }
#else
      static EFL_EINA_BOOST_LLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 1LL << (sizeof(EFL_EINA_BOOST_LLT) * CHAR_BIT - 1); }
      static EFL_EINA_BOOST_LLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ~(min)(); }
#endif
      EFL_EINA_BOOST_STATIC_CONSTANT(int, digits = sizeof(EFL_EINA_BOOST_LLT) * CHAR_BIT -1);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, digits10 = (CHAR_BIT * sizeof (EFL_EINA_BOOST_LLT) - 1) * 301L / 1000);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_signed = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_integer = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_exact = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, radix = 2);
      static EFL_EINA_BOOST_LLT epsilon() throw() { return 0; };
      static EFL_EINA_BOOST_LLT round_error() throw() { return 0; };

      EFL_EINA_BOOST_STATIC_CONSTANT(int, min_exponent = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, min_exponent10 = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, max_exponent = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, max_exponent10 = 0);

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_infinity = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_quiet_NaN = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_signaling_NaN = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_denorm = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_denorm_loss = false);
      static EFL_EINA_BOOST_LLT infinity() throw() { return 0; };
      static EFL_EINA_BOOST_LLT quiet_NaN() throw() { return 0; };
      static EFL_EINA_BOOST_LLT signaling_NaN() throw() { return 0; };
      static EFL_EINA_BOOST_LLT denorm_min() throw() { return 0; };

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_iec559 = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_bounded = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_modulo = true);

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, traps = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, tinyness_before = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(float_round_style, round_style = round_toward_zero);
      
  };

  template<>
  class numeric_limits<EFL_EINA_BOOST_ULLT> 
  {
   public:

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_specialized = true);
#ifdef EFL_EINA_BOOST_HAS_MS_INT64
      static EFL_EINA_BOOST_ULLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0ui64; }
      static EFL_EINA_BOOST_ULLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0xFFFFFFFFFFFFFFFFui64; }
#elif defined(ULLONG_MAX) && defined(ULLONG_MIN)
      static EFL_EINA_BOOST_ULLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULLONG_MIN; }
      static EFL_EINA_BOOST_ULLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULLONG_MAX; }
#elif defined(ULONGLONG_MAX) && defined(ULONGLONG_MIN)
      static EFL_EINA_BOOST_ULLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULONGLONG_MIN; }
      static EFL_EINA_BOOST_ULLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULONGLONG_MAX; }
#else
      static EFL_EINA_BOOST_ULLT min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0uLL; }
      static EFL_EINA_BOOST_ULLT max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (){ return ~0uLL; }
#endif
      EFL_EINA_BOOST_STATIC_CONSTANT(int, digits = sizeof(EFL_EINA_BOOST_LLT) * CHAR_BIT);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, digits10 = (CHAR_BIT * sizeof (EFL_EINA_BOOST_LLT)) * 301L / 1000);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_signed = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_integer = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_exact = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, radix = 2);
      static EFL_EINA_BOOST_ULLT epsilon() throw() { return 0; };
      static EFL_EINA_BOOST_ULLT round_error() throw() { return 0; };

      EFL_EINA_BOOST_STATIC_CONSTANT(int, min_exponent = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, min_exponent10 = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, max_exponent = 0);
      EFL_EINA_BOOST_STATIC_CONSTANT(int, max_exponent10 = 0);

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_infinity = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_quiet_NaN = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_signaling_NaN = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_denorm = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, has_denorm_loss = false);
      static EFL_EINA_BOOST_ULLT infinity() throw() { return 0; };
      static EFL_EINA_BOOST_ULLT quiet_NaN() throw() { return 0; };
      static EFL_EINA_BOOST_ULLT signaling_NaN() throw() { return 0; };
      static EFL_EINA_BOOST_ULLT denorm_min() throw() { return 0; };

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_iec559 = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_bounded = true);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, is_modulo = true);

      EFL_EINA_BOOST_STATIC_CONSTANT(bool, traps = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(bool, tinyness_before = false);
      EFL_EINA_BOOST_STATIC_CONSTANT(float_round_style, round_style = round_toward_zero);
      
  };
}
#endif 

#endif

