
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is guarded externally

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif

#if !defined(EFL_EINA_BOOST_PP_VALUE)
#   include <eina_boost/preprocessor/slot/slot.hpp>
#   include <eina_boost/preprocessor/iteration/self.hpp>

#   include <eina_boost/function_types/detail/encoding/def.hpp>
#   include <eina_boost/function_types/detail/encoding/aliases_def.hpp>

namespace efl_eina_boost { namespace function_types {

namespace detail
{
  template<class Tag, class RefTag> struct selector_bits
  {
#   define  EFL_EINA_BOOST_PP_VALUE non_member|member|non_variadic|variadic
#   include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, value = (
        (::efl_eina_boost::function_types::detail::bits<Tag>::value & EFL_EINA_BOOST_FT_default_cc) 
      | (::efl_eina_boost::function_types::detail::bits<RefTag>::value & EFL_EINA_BOOST_PP_SLOT(1))
    ));
  };

  template<bits_t SelectorBits> struct default_cc_tag; 
  
  template<class Tag, class RefTag> struct retag_default_cc
    : detail::compound_tag
      < Tag, detail::default_cc_tag< 
          ::efl_eina_boost::function_types::detail::selector_bits<Tag,RefTag>::value > >
  { };

  template<bits_t SelectorBits> struct default_cc_tag
  {
    typedef null_tag::bits bits;
    typedef null_tag::mask mask;
  };

  class test_class;
  typedef constant<EFL_EINA_BOOST_FT_cc_mask> cc_mask_constant;

#   define EFL_EINA_BOOST_FT_self \
      <eina_boost/function_types/detail/pp_retag_default_cc/master.hpp>

#   define  default_cc_ EFL_EINA_BOOST_FT_default_cc

#   define  EFL_EINA_BOOST_PP_VALUE default_cc_|non_member|non_variadic
#   define  EFL_EINA_BOOST_FT_tester void (*tester)()
#   define  EFL_EINA_BOOST_PP_INDIRECT_SELF EFL_EINA_BOOST_FT_self
#   include EFL_EINA_BOOST_PP_INCLUDE_SELF()

#   define  EFL_EINA_BOOST_PP_VALUE default_cc_|non_member|variadic
#   define  EFL_EINA_BOOST_FT_tester void (*tester)(...)
#   define  EFL_EINA_BOOST_PP_INDIRECT_SELF EFL_EINA_BOOST_FT_self
#   include EFL_EINA_BOOST_PP_INCLUDE_SELF()

#   define  EFL_EINA_BOOST_PP_VALUE default_cc_|member|non_variadic
#   define  EFL_EINA_BOOST_FT_tester void (test_class::*tester)()
#   define  EFL_EINA_BOOST_PP_INDIRECT_SELF EFL_EINA_BOOST_FT_self
#   include EFL_EINA_BOOST_PP_INCLUDE_SELF()

#   define  EFL_EINA_BOOST_PP_VALUE default_cc_|member|variadic
#   define  EFL_EINA_BOOST_FT_tester void (test_class::*tester)(...)
#   define  EFL_EINA_BOOST_PP_INDIRECT_SELF EFL_EINA_BOOST_FT_self
#   include EFL_EINA_BOOST_PP_INCLUDE_SELF()

#   undef   default_cc_

#   undef EFL_EINA_BOOST_FT_self

} } } // namespace ::efl_eina_boost::function_types::detail

#   include <eina_boost/function_types/detail/encoding/aliases_undef.hpp>
#   include <eina_boost/function_types/detail/encoding/undef.hpp>

#else // if defined(EFL_EINA_BOOST_PP_VALUE)

#   include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)

  template<> struct default_cc_tag<EFL_EINA_BOOST_PP_SLOT(1)> 
  {
    typedef EFL_EINA_BOOST_FT_tester;
    typedef mpl::bitand_<components<tester>::bits,cc_mask_constant> bits;
    typedef cc_mask_constant mask;
  };

#   undef EFL_EINA_BOOST_FT_tester

#endif

