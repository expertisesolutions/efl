
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_DETAIL_SYNTHESIZE_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_DETAIL_SYNTHESIZE_HPP_INCLUDED

#include <cstddef>

#include <eina_boost/mpl/at.hpp>
#include <eina_boost/mpl/size.hpp>

#include <eina_boost/function_types/config/config.hpp>
#include <eina_boost/function_types/property_tags.hpp>
#include <eina_boost/function_types/detail/cv_traits.hpp>
#include <eina_boost/function_types/detail/retag_default_cc.hpp>

namespace efl_eina_boost { namespace function_types { namespace detail {

template<bits_t Flags, bits_t CCID, std::size_t Size>
struct synthesize_impl_o
{
  template<typename Seq> struct synthesize_impl_i { };
};

template<typename Seq, bits_t Bits>
struct synthesize_impl
  : detail::synthesize_impl_o
    < ::efl_eina_boost::function_types::detail::decode_bits<Bits>::flags
    , ::efl_eina_boost::function_types::detail::decode_bits<Bits>::cc_id
    , ::efl_eina_boost::mpl::size<Seq>::value
    >
    ::template synthesize_impl_i<Seq>
{ };

template<typename Seq, typename Tag>
struct synthesize_func
  : detail::synthesize_impl
    < Seq
    , ::efl_eina_boost::function_types::detail::bits
      < detail::retag_default_cc
        < function_types::tag<nv_dcc_func, Tag> > 
      >::value 
    >
{ };

template<typename Seq, typename Tag>
struct synthesize_mfp
  : detail::synthesize_impl
    < Seq 
    , ::efl_eina_boost::function_types::detail::bits
      < detail::retag_default_cc
        < function_types::tag
          < typename detail::cv_traits< typename mpl::at_c<Seq,1>::type >::tag
          , nv_dcc_mfp, Tag
        > >
      >::value
    >
{ };

template<typename S, typename R = typename mpl::at_c<S,0>::type,
    typename C = typename mpl::at_c<S,1>::type>
struct synthesize_mop
{
  typedef R C::* type;
};

#define EFL_EINA_BOOST_FT_variations EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_member_pointer
#define EFL_EINA_BOOST_FT_al_path eina_boost/function_types/detail/synthesize_impl
#include <eina_boost/function_types/detail/pp_loop.hpp>

} } } // namespace ::efl_eina_boost::function_types::detail

#endif

