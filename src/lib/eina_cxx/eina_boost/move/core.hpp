//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2012-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////

//! \file core.hpp
//! This header implements macros to define movable classes and
//! move-aware functions

#ifndef EFL_EINA_BOOST_MOVE_CORE_HPP
#define EFL_EINA_BOOST_MOVE_CORE_HPP

#include <eina_boost/move/detail/config_begin.hpp>

//boost_move_no_copy_constructor_or_assign typedef
//used to detect noncopyable types for other Boost libraries.
#ifdef EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
   #define EFL_EINA_BOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE) \
      private:\
      TYPE(TYPE &);\
      TYPE& operator=(TYPE &);\
      public:\
      typedef int boost_move_no_copy_constructor_or_assign; \
      private:\
   //
#else
   #define EFL_EINA_BOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE) \
      public:\
      TYPE(TYPE const &) = delete;\
      TYPE& operator=(TYPE const &) = delete;\
      public:\
      typedef int boost_move_no_copy_constructor_or_assign; \
      private:\
   //
#endif   //EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS

#if defined(EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)

   #include <eina_boost/move/detail/meta_utils.hpp>

   //Move emulation rv breaks standard aliasing rules so add workarounds for some compilers
   #if defined(__GNUC__) && (__GNUC__ >= 4)
      #define EFL_EINA_BOOST_MOVE_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__))
   #else
      #define EFL_EINA_BOOST_MOVE_ATTRIBUTE_MAY_ALIAS
   #endif

   namespace efl_eina_boost {

   //////////////////////////////////////////////////////////////////////////////
   //
   //                            struct rv
   //
   //////////////////////////////////////////////////////////////////////////////
   template <class T>
   class rv
      : public ::efl_eina_boost::move_detail::if_c
         < ::efl_eina_boost::move_detail::is_class_or_union<T>::value
         , T
         , ::efl_eina_boost::move_detail::empty
         >::type
   {
      rv();
      ~rv();
      rv(rv const&);
      void operator=(rv const&);
   } EFL_EINA_BOOST_MOVE_ATTRIBUTE_MAY_ALIAS;


   //////////////////////////////////////////////////////////////////////////////
   //
   //                            move_detail::is_rv
   //
   //////////////////////////////////////////////////////////////////////////////

   namespace move_detail {

   template <class T>
   struct is_rv
      : ::efl_eina_boost::move_detail::integral_constant<bool, false>
   {};

   template <class T>
   struct is_rv< rv<T> >
      : ::efl_eina_boost::move_detail::integral_constant<bool, true>
   {};

   template <class T>
   struct is_rv< const rv<T> >
      : ::efl_eina_boost::move_detail::integral_constant<bool, true>
   {};

   }  //namespace move_detail {

   //////////////////////////////////////////////////////////////////////////////
   //
   //                               has_move_emulation_enabled
   //
   //////////////////////////////////////////////////////////////////////////////
   template<class T>
   struct has_move_emulation_enabled
      : ::efl_eina_boost::move_detail::is_convertible< T, ::efl_eina_boost::rv<T>& >
   {};

   template<class T>
   struct has_move_emulation_enabled<T&>
      : ::efl_eina_boost::move_detail::integral_constant<bool, false>
   {};

   template<class T>
   struct has_move_emulation_enabled< ::efl_eina_boost::rv<T> >
      : ::efl_eina_boost::move_detail::integral_constant<bool, false>
   {};

   }  //namespace efl_eina_boost {

   #define EFL_EINA_BOOST_RV_REF(TYPE)\
      ::efl_eina_boost::rv< TYPE >& \
   //

   #define EFL_EINA_BOOST_RV_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      ::efl_eina_boost::rv< TYPE<ARG1, ARG2> >& \
   //

   #define EFL_EINA_BOOST_RV_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      ::efl_eina_boost::rv< TYPE<ARG1, ARG2, ARG3> >& \
   //

   #define EFL_EINA_BOOST_RV_REF_BEG\
      ::efl_eina_boost::rv<   \
   //

   #define EFL_EINA_BOOST_RV_REF_END\
      >& \
   //

   #define EFL_EINA_BOOST_FWD_REF(TYPE)\
      const TYPE & \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF(TYPE)\
      const ::efl_eina_boost::rv< TYPE >& \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_BEG \
      const ::efl_eina_boost::rv<  \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_END \
      >& \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      const ::efl_eina_boost::rv< TYPE<ARG1, ARG2> >& \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      const ::efl_eina_boost::rv< TYPE<ARG1, ARG2, ARG3> >& \
   //

   #define EFL_EINA_BOOST_CATCH_CONST_RLVALUE(TYPE)\
      const ::efl_eina_boost::rv< TYPE >& \
   //

   //////////////////////////////////////////////////////////////////////////////
   //
   //                         EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE
   //
   //////////////////////////////////////////////////////////////////////////////
   #define EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      EFL_EINA_BOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE)\
      public:\
      operator ::efl_eina_boost::rv<TYPE>&() \
      {  return *static_cast< ::efl_eina_boost::rv<TYPE>* >(this);  }\
      operator const ::efl_eina_boost::rv<TYPE>&() const \
      {  return *static_cast<const ::efl_eina_boost::rv<TYPE>* >(this);  }\
      private:\
   //

   //////////////////////////////////////////////////////////////////////////////
   //
   //                         EFL_EINA_BOOST_COPYABLE_AND_MOVABLE
   //
   //////////////////////////////////////////////////////////////////////////////

   #define EFL_EINA_BOOST_COPYABLE_AND_MOVABLE(TYPE)\
      public:\
      TYPE& operator=(TYPE &t)\
      {  this->operator=(static_cast<const ::efl_eina_boost::rv<TYPE> &>(const_cast<const TYPE &>(t))); return *this;}\
      public:\
      operator ::efl_eina_boost::rv<TYPE>&() \
      {  return *static_cast< ::efl_eina_boost::rv<TYPE>* >(this);  }\
      operator const ::efl_eina_boost::rv<TYPE>&() const \
      {  return *static_cast<const ::efl_eina_boost::rv<TYPE>* >(this);  }\
      private:\
   //

   #define EFL_EINA_BOOST_COPYABLE_AND_MOVABLE_ALT(TYPE)\
      public:\
      operator ::efl_eina_boost::rv<TYPE>&() \
      {  return *static_cast< ::efl_eina_boost::rv<TYPE>* >(this);  }\
      operator const ::efl_eina_boost::rv<TYPE>&() const \
      {  return *static_cast<const ::efl_eina_boost::rv<TYPE>* >(this);  }\
      private:\
   //

#else    //EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES

   //Compiler workaround detection
   #if !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)
      #if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ < 5) && !defined(__clang__)
         //Pre-standard rvalue binding rules
         #define EFL_EINA_BOOST_MOVE_OLD_RVALUE_REF_BINDING_RULES
      #elif defined(_MSC_VER) && (_MSC_VER == 1600)
         //Standard rvalue binding rules but with some bugs
         #define EFL_EINA_BOOST_MOVE_MSVC_10_MEMBER_RVALUE_REF_BUG
         //Use standard library for MSVC to avoid namespace issues as
         //some move calls in the STL are not fully qualified.
         //#define EFL_EINA_BOOST_MOVE_USE_STANDARD_LIBRARY_MOVE
      #endif
   #endif

   //! This macro marks a type as movable but not copyable, disabling copy construction
   //! and assignment. The user will need to write a move constructor/assignment as explained
   //! in the documentation to fully write a movable but not copyable class.
   #define EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      EFL_EINA_BOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE)\
      public:\
      typedef int boost_move_emulation_t;\
   //

   //! This macro marks a type as copyable and movable.
   //! The user will need to write a move constructor/assignment and a copy assignment
   //! as explained in the documentation to fully write a copyable and movable class.
   #define EFL_EINA_BOOST_COPYABLE_AND_MOVABLE(TYPE)\
   //

   #if !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)
   #define EFL_EINA_BOOST_COPYABLE_AND_MOVABLE_ALT(TYPE)\
   //
   #endif   //#if !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)

   namespace efl_eina_boost {

   //!This trait yields to a compile-time true boolean if T was marked as
   //!EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE or EFL_EINA_BOOST_COPYABLE_AND_MOVABLE and
   //!rvalue references are not available on the platform. False otherwise.
   template<class T>
   struct has_move_emulation_enabled
   {
      static const bool value = false;
   };

   }  //namespace efl_eina_boost{

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for classes marked as
   //!EFL_EINA_BOOST_COPYABLE_AND_MOVABLE or EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE
   #define EFL_EINA_BOOST_RV_REF(TYPE)\
      TYPE && \
   //

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for template classes marked as
   //!EFL_EINA_BOOST_COPYABLE_AND_MOVABLE or EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE.
   //!As macros have problems with comma-separatd template arguments,
   //!the template argument must be preceded with EFL_EINA_BOOST_RV_REF_START
   //!and ended with EFL_EINA_BOOST_RV_REF_END
   #define EFL_EINA_BOOST_RV_REF_BEG\
         \
   //

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for template classes marked as
   //!EFL_EINA_BOOST_COPYABLE_AND_MOVABLE or EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE.
   //!As macros have problems with comma-separatd template arguments,
   //!the template argument must be preceded with EFL_EINA_BOOST_RV_REF_START
   //!and ended with EFL_EINA_BOOST_RV_REF_END
   #define EFL_EINA_BOOST_RV_REF_END\
      && \

   //!This macro is used to achieve portable syntax in copy
   //!assignment for classes marked as EFL_EINA_BOOST_COPYABLE_AND_MOVABLE.
   #define EFL_EINA_BOOST_COPY_ASSIGN_REF(TYPE)\
      const TYPE & \
   //

   //! This macro is used to implement portable perfect forwarding
   //! as explained in the documentation.
   #define EFL_EINA_BOOST_FWD_REF(TYPE)\
      TYPE && \
   //

   #if !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)
   /// @cond

   #define EFL_EINA_BOOST_RV_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      TYPE<ARG1, ARG2> && \
   //

   #define EFL_EINA_BOOST_RV_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      TYPE<ARG1, ARG2, ARG3> && \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_BEG \
      const \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_END \
      & \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      const TYPE<ARG1, ARG2> & \
   //

   #define EFL_EINA_BOOST_COPY_ASSIGN_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      const TYPE<ARG1, ARG2, ARG3>& \
   //

   #define EFL_EINA_BOOST_CATCH_CONST_RLVALUE(TYPE)\
      const TYPE & \
   //

   /// @endcond

   #endif   //#if !defined(EFL_EINA_BOOST_MOVE_DOXYGEN_INVOKED)

#endif   //EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES

#include <eina_boost/move/detail/config_end.hpp>

#endif //#ifndef EFL_EINA_BOOST_MOVE_CORE_HPP
