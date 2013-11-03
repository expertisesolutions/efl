//  (C) Copyright John Maddock 2000. 
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_POLYMORPHIC_HPP
#define EFL_EINA_BOOST_TT_IS_POLYMORPHIC_HPP

#include <eina_boost/type_traits/intrinsics.hpp>
#ifndef EFL_EINA_BOOST_IS_POLYMORPHIC
#include <eina_boost/type_traits/is_class.hpp>
#include <eina_boost/type_traits/remove_cv.hpp>
#endif
// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>
#include <eina_boost/detail/workaround.hpp>

namespace efl_eina_boost{

#ifndef EFL_EINA_BOOST_IS_POLYMORPHIC

namespace detail{

template <class T>
struct is_polymorphic_imp1
{
# if EFL_EINA_BOOST_WORKAROUND(__MWERKS__, <= 0x2407) // CWPro7 should return false always.
    typedef char d1, (&d2)[2];
# else 
   typedef typename remove_cv<T>::type ncvT;
   struct d1 : public ncvT
   {
      d1();
#  if !defined(__GNUC__) // this raises warnings with some classes, and buys nothing with GCC
      ~d1()throw();
#  endif 
      char padding[256];
   private:
      // keep some picky compilers happy:
      d1(const d1&);
      d1& operator=(const d1&);
   };
   struct d2 : public ncvT
   {
      d2();
      virtual ~d2()throw();
#  if !defined(EFL_EINA_BOOST_MSVC) && !defined(__ICL)
      // for some reason this messes up VC++ when T has virtual bases,
      // probably likewise for compilers that use the same ABI:
      struct unique{};
      virtual void unique_name_to_boost5487629(unique*);
#  endif
      char padding[256];
   private:
      // keep some picky compilers happy:
      d2(const d2&);
      d2& operator=(const d2&);
   };
# endif 
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (sizeof(d2) == sizeof(d1)));
};

template <class T>
struct is_polymorphic_imp2
{
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};

template <bool is_class>
struct is_polymorphic_selector
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp2<T> type;
   };
};

template <>
struct is_polymorphic_selector<true>
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp1<T> type;
   };
};

template <class T>
struct is_polymorphic_imp
{
   typedef is_polymorphic_selector< ::efl_eina_boost::is_class<T>::value> selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type imp_type;
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = imp_type::value);
};

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_polymorphic,T,::efl_eina_boost::detail::is_polymorphic_imp<T>::value)

#else // EFL_EINA_BOOST_IS_POLYMORPHIC

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_polymorphic,T,EFL_EINA_BOOST_IS_POLYMORPHIC(T))

#endif

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif
