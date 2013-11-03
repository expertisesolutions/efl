
#ifndef EFL_EINA_BOOST_MPL_ASSERT_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_ASSERT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: assert.hpp 86514 2013-10-29 13:15:03Z bemandawes $
// $Date: 2013-10-29 06:15:03 -0700 (Tue, 29 Oct 2013) $
// $Revision: 86514 $

#include <eina_boost/mpl/not.hpp>
#include <eina_boost/mpl/aux_/value_wknd.hpp>
#include <eina_boost/mpl/aux_/nested_type_wknd.hpp>
#include <eina_boost/mpl/aux_/yes_no.hpp>
#include <eina_boost/mpl/aux_/na.hpp>
#include <eina_boost/mpl/aux_/adl_barrier.hpp>

#include <eina_boost/mpl/aux_/config/nttp.hpp>
#include <eina_boost/mpl/aux_/config/dtp.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/static_constant.hpp>
#include <eina_boost/mpl/aux_/config/pp_counter.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#include <eina_boost/preprocessor/cat.hpp>

#include <eina_boost/config.hpp> // make sure 'size_t' is placed into 'std'
#include <cstddef>

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1700)
#include <eina_boost/mpl/if.hpp>
#endif

#if EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) \
    || (EFL_EINA_BOOST_MPL_CFG_GCC != 0) \
    || EFL_EINA_BOOST_WORKAROUND(__IBMCPP__, <= 600)
#   define EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES
#endif

#if EFL_EINA_BOOST_WORKAROUND(__MWERKS__, < 0x3202) \
    || EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 238) \
    || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) \
    || EFL_EINA_BOOST_WORKAROUND(__DMC__, EFL_EINA_BOOST_TESTED_AT(0x840))
#   define EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
#endif

// agurt, 10/nov/06: use enums for Borland (which cannot cope with static constants) 
// and GCC (which issues "unused variable" warnings when static constants are used 
// at a function scope)
#if EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) \
    || (EFL_EINA_BOOST_MPL_CFG_GCC != 0)
#   define EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT(T, expr) enum { expr }
#else
#   define EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT(T, expr) EFL_EINA_BOOST_STATIC_CONSTANT(T, expr)
#endif


EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

struct failed {};

// agurt, 24/aug/04: MSVC 7.1 workaround here and below: return/accept 
// 'assert<false>' by reference; can't apply it unconditionally -- apparently it
// degrades the quality of GCC diagnostics
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1310)
#   define AUX778076_ASSERT_ARG(x) x&
#else
#   define AUX778076_ASSERT_ARG(x) x
#endif

template< bool C >  struct assert        { typedef void* type; };
template<>          struct assert<false> { typedef AUX778076_ASSERT_ARG(assert) type; };

template< bool C >
int assertion_failed( typename assert<C>::type );

template< bool C >
struct assertion
{
    static int failed( assert<false> );
};

template<>
struct assertion<true>
{
    static int failed( void* );
};

struct assert_
{
#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
    template< typename T1, typename T2 = na, typename T3 = na, typename T4 = na > struct types {};
#endif
    static assert_ const arg;
    enum relations { equal = 1, not_equal, greater, greater_equal, less, less_equal };
};


#if !defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

bool operator==( failed, failed );
bool operator!=( failed, failed );
bool operator>( failed, failed );
bool operator>=( failed, failed );
bool operator<( failed, failed );
bool operator<=( failed, failed );

#if defined(__EDG_VERSION__)
template< bool (*)(failed, failed), long x, long y > struct assert_relation {};
#   define EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<r,x,y>
#else
template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, x), EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, y), bool (*)(failed, failed) > 
struct assert_relation {};
#   define EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<x,y,r>
#endif

#else // EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

efl_eina_boost::mpl::aux::weighted_tag<1>::type operator==( assert_, assert_ );
efl_eina_boost::mpl::aux::weighted_tag<2>::type operator!=( assert_, assert_ );
efl_eina_boost::mpl::aux::weighted_tag<3>::type operator>(  assert_, assert_ );
efl_eina_boost::mpl::aux::weighted_tag<4>::type operator>=( assert_, assert_ );
efl_eina_boost::mpl::aux::weighted_tag<5>::type operator<( assert_, assert_ );
efl_eina_boost::mpl::aux::weighted_tag<6>::type operator<=( assert_, assert_ );

template< assert_::relations r, long x, long y > struct assert_relation {};

#endif 

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1700)

template<class Pred>
struct extract_assert_pred;

template<class Pred>
struct extract_assert_pred<void(Pred)> { typedef Pred type; };

template<class Pred>
struct eval_assert {
    typedef typename extract_assert_pred<Pred>::type P;
    typedef typename P::type p_type;
    typedef typename ::efl_eina_boost::mpl::if_c<p_type::value,
        AUX778076_ASSERT_ARG(assert<false>),
        failed ************ P::************
    >::type type;
};

template<class Pred>
struct eval_assert_not {
    typedef typename extract_assert_pred<Pred>::type P;
    typedef typename P::type p_type;
    typedef typename ::efl_eina_boost::mpl::if_c<!p_type::value,
        AUX778076_ASSERT_ARG(assert<false>),
        failed ************ ::efl_eina_boost::mpl::not_<P>::************
    >::type type;
};

template< typename T >
T make_assert_arg();

#elif !defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)

template< bool > struct assert_arg_pred_impl { typedef int type; };
template<> struct assert_arg_pred_impl<true> { typedef void* type; };

template< typename P > struct assert_arg_pred
{
    typedef typename P::type p_type;
    typedef typename assert_arg_pred_impl< p_type::value >::type type;
};

template< typename P > struct assert_arg_pred_not
{
    typedef typename P::type p_type;
    EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( bool, p = !p_type::value );
    typedef typename assert_arg_pred_impl<p>::type type;
};

template< typename Pred >
failed ************ (Pred::************ 
      assert_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type )
    );

template< typename Pred >
failed ************ (efl_eina_boost::mpl::not_<Pred>::************ 
      assert_not_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type )
    );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_not_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type );


#else // EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
        
template< bool c, typename Pred > struct assert_arg_type_impl
{
    typedef failed      ************ Pred::* mwcw83_wknd;
    typedef mwcw83_wknd ************* type;
};

template< typename Pred > struct assert_arg_type_impl<true,Pred>
{
    typedef AUX778076_ASSERT_ARG(assert<false>) type;
};

template< typename Pred > struct assert_arg_type
    : assert_arg_type_impl< EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(EFL_EINA_BOOST_MPL_AUX_NESTED_TYPE_WKND(Pred))::value, Pred >
{
};

template< typename Pred >
typename assert_arg_type<Pred>::type 
assert_arg(void (*)(Pred), int);

template< typename Pred >
typename assert_arg_type< efl_eina_boost::mpl::not_<Pred> >::type 
assert_not_arg(void (*)(Pred), int);

#   if !defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)
template< long x, long y, bool (*r)(failed, failed) >
typename assert_arg_type_impl< false,EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x,y,r) >::type
assert_rel_arg( EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x,y,r) );
#   else
template< assert_::relations r, long x, long y >
typename assert_arg_type_impl< false,assert_relation<r,x,y> >::type
assert_rel_arg( assert_relation<r,x,y> );
#   endif

#endif // EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER

#undef AUX778076_ASSERT_ARG

EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1700)

// EFL_EINA_BOOST_MPL_ASSERT((pred<x,...>))

#define EFL_EINA_BOOST_MPL_ASSERT(pred) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          efl_eina_boost::mpl::assertion_failed<false>( \
              efl_eina_boost::mpl::make_assert_arg< \
                  typename efl_eina_boost::mpl::eval_assert<void pred>::type \
                >() \
            ) \
        ) \
    ) \
/**/

// EFL_EINA_BOOST_MPL_ASSERT_NOT((pred<x,...>))

#define EFL_EINA_BOOST_MPL_ASSERT_NOT(pred) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          efl_eina_boost::mpl::assertion_failed<false>( \
              efl_eina_boost::mpl::make_assert_arg< \
                  typename efl_eina_boost::mpl::eval_assert_not<void pred>::type \
                >() \
            ) \
        ) \
    ) \
/**/

#else

// EFL_EINA_BOOST_MPL_ASSERT((pred<x,...>))

#define EFL_EINA_BOOST_MPL_ASSERT(pred) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          efl_eina_boost::mpl::assertion_failed<false>( \
              efl_eina_boost::mpl::assert_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
    ) \
/**/

// EFL_EINA_BOOST_MPL_ASSERT_NOT((pred<x,...>))

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300)
#   define EFL_EINA_BOOST_MPL_ASSERT_NOT(pred) \
enum { \
      EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          efl_eina_boost::mpl::assertion<false>::failed( \
              efl_eina_boost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/
#else
#   define EFL_EINA_BOOST_MPL_ASSERT_NOT(pred) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          efl_eina_boost::mpl::assertion_failed<false>( \
              efl_eina_boost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
   ) \
/**/
#endif

#endif

// EFL_EINA_BOOST_MPL_ASSERT_RELATION(x, ==|!=|<=|<|>=|>, y)

#if defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

#   if !defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
// agurt, 9/nov/06: 'enum' below is a workaround for gcc 4.0.4/4.1.1 bugs #29522 and #29518
#   define EFL_EINA_BOOST_MPL_ASSERT_RELATION_IMPL(counter, x, rel, y)      \
enum { EFL_EINA_BOOST_PP_CAT(mpl_assert_rel_value,counter) = (x rel y) }; \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        efl_eina_boost::mpl::assertion_failed<EFL_EINA_BOOST_PP_CAT(mpl_assert_rel_value,counter)>( \
            (efl_eina_boost::mpl::failed ************ ( efl_eina_boost::mpl::assert_relation< \
                  efl_eina_boost::mpl::assert_::relations( sizeof( \
                      efl_eina_boost::mpl::assert_::arg rel efl_eina_boost::mpl::assert_::arg \
                    ) ) \
                , x \
                , y \
                >::************)) 0 ) \
        ) \
    ) \
/**/
#   else
#   define EFL_EINA_BOOST_MPL_ASSERT_RELATION_IMPL(counter, x, rel, y)    \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assert_rel,counter) = sizeof( \
          efl_eina_boost::mpl::assert_::arg rel efl_eina_boost::mpl::assert_::arg \
        ) \
    ); \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( bool, EFL_EINA_BOOST_PP_CAT(mpl_assert_rel_value,counter) = (x rel y) ); \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        efl_eina_boost::mpl::assertion_failed<EFL_EINA_BOOST_PP_CAT(mpl_assert_rel_value,counter)>( \
              efl_eina_boost::mpl::assert_rel_arg( efl_eina_boost::mpl::assert_relation< \
                  efl_eina_boost::mpl::assert_::relations(EFL_EINA_BOOST_PP_CAT(mpl_assert_rel,counter)) \
                , x \
                , y \
                >() ) \
            ) \
        ) \
    ) \
/**/
#   endif

#   define EFL_EINA_BOOST_MPL_ASSERT_RELATION(x, rel, y) \
EFL_EINA_BOOST_MPL_ASSERT_RELATION_IMPL(EFL_EINA_BOOST_MPL_AUX_PP_COUNTER(), x, rel, y) \
/**/

#else // !EFL_EINA_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

#   if defined(EFL_EINA_BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
#   define EFL_EINA_BOOST_MPL_ASSERT_RELATION(x, rel, y) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
        efl_eina_boost::mpl::assertion_failed<(x rel y)>( efl_eina_boost::mpl::assert_rel_arg( \
              efl_eina_boost::mpl::EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x,y,(&efl_eina_boost::mpl::operator rel))() \
            ) ) \
        ) \
    ) \
/**/
#   else
#   define EFL_EINA_BOOST_MPL_ASSERT_RELATION(x, rel, y) \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,EFL_EINA_BOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
        efl_eina_boost::mpl::assertion_failed<(x rel y)>( (efl_eina_boost::mpl::failed ************ ( \
            efl_eina_boost::mpl::EFL_EINA_BOOST_MPL_AUX_ASSERT_RELATION(x,y,(&efl_eina_boost::mpl::operator rel))::************))0 ) \
        ) \
    ) \
/**/
#   endif

#endif


// EFL_EINA_BOOST_MPL_ASSERT_MSG( (pred<x,...>::value), USER_PROVIDED_MESSAGE, (types<x,...>) ) 

#if EFL_EINA_BOOST_WORKAROUND(__MWERKS__, EFL_EINA_BOOST_TESTED_AT(0x3202))
#   define EFL_EINA_BOOST_MPL_ASSERT_MSG_IMPL( counter, c, msg, types_ ) \
struct msg; \
typedef struct EFL_EINA_BOOST_PP_CAT(msg,counter) : efl_eina_boost::mpl::assert_ \
{ \
    using efl_eina_boost::mpl::assert_::types; \
    static efl_eina_boost::mpl::failed ************ (msg::************ assert_arg()) types_ \
    { return 0; } \
} EFL_EINA_BOOST_PP_CAT(mpl_assert_arg,counter); \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        efl_eina_boost::mpl::assertion<(c)>::failed( EFL_EINA_BOOST_PP_CAT(mpl_assert_arg,counter)::assert_arg() ) \
        ) \
    ) \
/**/
#else
#   define EFL_EINA_BOOST_MPL_ASSERT_MSG_IMPL( counter, c, msg, types_ )  \
struct msg; \
typedef struct EFL_EINA_BOOST_PP_CAT(msg,counter) : efl_eina_boost::mpl::assert_ \
{ \
    static efl_eina_boost::mpl::failed ************ (msg::************ assert_arg()) types_ \
    { return 0; } \
} EFL_EINA_BOOST_PP_CAT(mpl_assert_arg,counter); \
EFL_EINA_BOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , EFL_EINA_BOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        efl_eina_boost::mpl::assertion_failed<(c)>( EFL_EINA_BOOST_PP_CAT(mpl_assert_arg,counter)::assert_arg() ) \
        ) \
    ) \
/**/
#endif

#define EFL_EINA_BOOST_MPL_ASSERT_MSG( c, msg, types_ ) \
EFL_EINA_BOOST_MPL_ASSERT_MSG_IMPL( EFL_EINA_BOOST_MPL_AUX_PP_COUNTER(), c, msg, types_ ) \
/**/

#endif // EFL_EINA_BOOST_MPL_ASSERT_HPP_INCLUDED
