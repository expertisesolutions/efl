
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

#if !defined(EFL_EINA_BOOST_FT_PREPROCESSING_MODE) || defined(EFL_EINA_BOOST_FT_CONFIG_HPP_INCLUDED)
#   error "this file used with two-pass preprocessing, only"
#endif

#include <eina_boost/preprocessor/slot/slot.hpp>
#include <eina_boost/function_types/detail/encoding/def.hpp>

namespace efl_eina_boost { namespace function_types {

typedef detail::property_tag<EFL_EINA_BOOST_FT_non_variadic,EFL_EINA_BOOST_FT_variadic_mask> non_variadic;
typedef detail::property_tag<EFL_EINA_BOOST_FT_variadic,EFL_EINA_BOOST_FT_variadic_mask>     variadic;
                                                                       
typedef detail::property_tag<0,EFL_EINA_BOOST_FT_const>                     non_const;
typedef detail::property_tag<EFL_EINA_BOOST_FT_const,EFL_EINA_BOOST_FT_const>        const_qualified;
                                                                       
typedef detail::property_tag<0,EFL_EINA_BOOST_FT_volatile>                  non_volatile;
typedef detail::property_tag<EFL_EINA_BOOST_FT_volatile,EFL_EINA_BOOST_FT_volatile>  volatile_qualified; 

typedef detail::property_tag<EFL_EINA_BOOST_FT_default_cc,EFL_EINA_BOOST_FT_cc_mask> default_cc;

#define EFL_EINA_BOOST_PP_VALUE EFL_EINA_BOOST_FT_const|EFL_EINA_BOOST_FT_volatile 
#include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)

typedef detail::property_tag<0                , EFL_EINA_BOOST_PP_SLOT(1)> non_cv;
typedef detail::property_tag<EFL_EINA_BOOST_FT_const   , EFL_EINA_BOOST_PP_SLOT(1)> const_non_volatile;
typedef detail::property_tag<EFL_EINA_BOOST_FT_volatile, EFL_EINA_BOOST_PP_SLOT(1)> volatile_non_const;
typedef detail::property_tag<EFL_EINA_BOOST_PP_SLOT(1) , EFL_EINA_BOOST_PP_SLOT(1)> cv_qualified;

namespace detail {

  typedef constant<EFL_EINA_BOOST_FT_full_mask> full_mask;

  template <bits_t Flags, bits_t CCID> struct encode_bits_impl
  {
    EFL_EINA_BOOST_STATIC_CONSTANT( bits_t, value = 
      Flags | (EFL_EINA_BOOST_FT_default_cc * CCID) << 1 );
  };

  template <bits_t Flags, bits_t CCID, std::size_t Arity> 
  struct encode_charr_impl
  {
    EFL_EINA_BOOST_STATIC_CONSTANT(std::size_t, value = (std::size_t)(1+
      Flags | (EFL_EINA_BOOST_FT_default_cc * CCID) << 1 | Arity << EFL_EINA_BOOST_FT_arity_shift
    ));
  };

  template <bits_t Bits> struct decode_bits
  {
    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, flags = Bits & EFL_EINA_BOOST_FT_flags_mask);

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, cc_id = 
      ( (Bits & EFL_EINA_BOOST_FT_full_mask) / EFL_EINA_BOOST_FT_default_cc) >> 1 
    );

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, tag_bits = (Bits & EFL_EINA_BOOST_FT_full_mask));

    EFL_EINA_BOOST_STATIC_CONSTANT(std::size_t, arity = (std::size_t)
      (Bits >> EFL_EINA_BOOST_FT_arity_shift) 
    );
  };

  template <bits_t LHS_bits, bits_t LHS_mask, bits_t RHS_bits, bits_t RHS_mask>
  struct tag_ice
  {
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, match =
      RHS_bits == (LHS_bits & RHS_mask & (RHS_bits |~EFL_EINA_BOOST_FT_type_mask))
    );

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, combined_bits = 
      (LHS_bits & ~RHS_mask) | RHS_bits
    );

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, combined_mask =
      LHS_mask | RHS_mask
    );

    EFL_EINA_BOOST_STATIC_CONSTANT(bits_t, extracted_bits =
      LHS_bits & RHS_mask
    );

  };

#define EFL_EINA_BOOST_FT_mask EFL_EINA_BOOST_FT_type_mask
  typedef property_tag<EFL_EINA_BOOST_FT_callable_builtin,EFL_EINA_BOOST_FT_mask>            callable_builtin_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_non_member_callable_builtin,EFL_EINA_BOOST_FT_mask> nonmember_callable_builtin_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_function,EFL_EINA_BOOST_FT_mask>       function_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_reference,EFL_EINA_BOOST_FT_mask>      reference_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_pointer,EFL_EINA_BOOST_FT_mask>        pointer_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_member_function_pointer,EFL_EINA_BOOST_FT_mask> member_function_pointer_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_member_object_pointer,EFL_EINA_BOOST_FT_mask> member_object_pointer_tag;
  typedef property_tag<EFL_EINA_BOOST_FT_member_object_pointer_flags,EFL_EINA_BOOST_FT_full_mask> member_object_pointer_base;
  typedef property_tag<EFL_EINA_BOOST_FT_member_pointer,EFL_EINA_BOOST_FT_mask> member_pointer_tag;
#undef  EFL_EINA_BOOST_FT_mask 

#define EFL_EINA_BOOST_PP_VALUE EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_default_cc
#include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)
#define EFL_EINA_BOOST_PP_VALUE EFL_EINA_BOOST_FT_type_mask|EFL_EINA_BOOST_FT_variadic_mask|EFL_EINA_BOOST_FT_cc_mask
#include EFL_EINA_BOOST_PP_ASSIGN_SLOT(2)

  typedef property_tag< EFL_EINA_BOOST_PP_SLOT(1) , EFL_EINA_BOOST_PP_SLOT(2) > nv_dcc_func;

#define EFL_EINA_BOOST_PP_VALUE \
    EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_default_cc
#include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)

  typedef property_tag< EFL_EINA_BOOST_PP_SLOT(1) , EFL_EINA_BOOST_PP_SLOT(2) > nv_dcc_mfp;

} // namespace detail

} } // namespace ::efl_eina_boost::function_types

