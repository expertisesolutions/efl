
///
/// @file eo_private.hpp
///

#if !defined(BOOST_PP_IS_ITERATING)

#ifndef EO3_EO_PRIVATE_HPP
#define EO3_EO_PRIVATE_HPP

extern "C" {
#include "Eo.h"
}

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#define EFL_MAX_ARGS 8          ///< The maxium number of arguments for templates and constructors.

namespace efl { namespace eo { namespace detail {

/// @addtogroup Efl_Cxx_Detail
/// @{

/// @internal
/// 
/// @brief Provides a getter for an heterogeous sequence of arguments.
/// 
/// @param T An <em>EO C++ Class</em>
/// @param Seq An heterogenous sequence of arguments.
///
template <typename T, typename Seq>
struct args_class
{
  /// @internal
  /// 
  /// @brief Class constructor.
  /// 
  /// @param seq An heterogenous sequence of arguments.
  /// 
  args_class(Seq seq)
    : seq(seq) {}

  /// @internal
  /// 
  /// @brief Get the Nth element of the sequence.
  /// 
  /// @param N The index of the argument to be retrieved.
  /// 
  template <int N>
  typename boost::remove_reference
  <
    typename boost::fusion::result_of::at_c<Seq, N>::type
  >::type get() const
  {
    return boost::fusion::at_c<N>(seq);
  }

  Seq seq; ///< The sequence of arguments.
};

/// @internal
/// 
/// @brief A simple generic tag to help keeping track of a type.
/// 
/// @details 
/// Used to mimic what would be a "template specialization" of a
/// function through the overload of an argument of type @ref
/// efl::eo::detail::tag --- because @c C++0x does not implement
/// template specialization of functions.
///
template <typename T> struct tag {};

#define EFL_CXX_void_tag(Z, N, DATA) struct BOOST_PP_CAT(void_tag_, N) {};
BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_void_tag, ~)
#undef EFL_CXX_void_tag

/// @internal
/// 
/// @brief Invokes the different implementations of @ref
/// efl::eo::detail::eo_class_new for the parent and all extensions.
///
/// @see efl::eo::detail::eo_class_new
/// 
template <typename P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, typename E)>
struct do_eo_class_new
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
#define EFL_CXX_get_eo_class(Z, N, DATA) ,get_eo_class(tag<BOOST_PP_CAT(E,N)>())
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_get_eo_class, ~),
			(void*)NULL);
  }
};

template <typename T> struct operation_description_class_size;

/// @internal
///
/// @brief Provides the operator to convert @p T to @p D.
/// 
/// @param D The target (derived) class
/// @param T An <em>EO C++ Class</em>
/// 
template <typename D, typename T>
struct conversion_operator
{
  operator T() const
  {
    eo_ref(static_cast<D const*>(this)->_eo_ptr());
    return T(static_cast<D const*>(this)->_eo_ptr());
  }
};

/// @}

} // namespace detail {

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, EFL_MAX_ARGS, "eo_private.hpp"))
#include BOOST_PP_ITERATE()

namespace detail {

/// @addtogroup Efl_Cxx_Detail
/// @{

/// @internal
///
/// @brief Provides all operations of type @p T.
///
/// @param T An <em>EO C++ Class</em>
///
/// There must be an unique specialization of this class for each
/// declared <em>EO C++ Class</em>.
/// 
template <typename T> struct operations;

#define EFL_CXX_void_operations(Z, N, DATA) \
  template <> struct operations<BOOST_PP_CAT(void_tag_, N)> \
  {                                                       \
    template <typename D>                                 \
    struct type {};                                       \
  };
BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_void_operations, ~)
#undef EFL_CXX_void_operations

#define EFL_CXX_void_operation_description_class_size(Z, N, DATA)		    \
  template <> struct operation_description_class_size<BOOST_PP_CAT(void_tag_, N)>   \
  {										    \
    static const int value = 0;						            \
  };
BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_void_operation_description_class_size, ~)
#undef EFL_CXX_void_operation_description_class_size


/// @internal
///
/// @brief Provides the operations of an extension as well as its
/// conversion operator.
///
/// @param T The <em>EO C++ Extension</em>
///
/// There must be an unique specialization of @ref
/// efl::eo::detail::extension_inheritance for each known <em>EO C++
/// Extension</em> -- i.e., @em Interfaces and @em Mixins.
/// 
template <typename T>
struct extension_inheritance {};

/// @internal
///
/// @brief efl::eo::inherit's private data.
/// 
struct Inherit_Private_Data
{
  void* this_;
};

/// @}

} } }

#undef EFL_CXX_get_eo_class

#endif // EO3_EO_PRIVATE_HPP
#else 

#define I BOOST_PP_ITERATION()

#if I
template <typename T BOOST_PP_ENUM_TRAILING_PARAMS(I, typename A)>
detail::args_class<T, boost::fusion::vector< BOOST_PP_ENUM_PARAMS(I, A) > > 
  args(BOOST_PP_ENUM_BINARY_PARAMS(I, A, a))
{
  return detail::args_class<T, boost::fusion::vector<BOOST_PP_ENUM_PARAMS(I, A)> >
    (boost::fusion::vector<BOOST_PP_ENUM_PARAMS(I, A)>(BOOST_PP_ENUM_PARAMS(I, a)));
}

namespace detail {

#define EFL_CXX_do_eo_class_new_spec(Z, N, DATA) , BOOST_PP_CAT(void_tag_, BOOST_PP_ADD(BOOST_PP_SUB(EFL_MAX_ARGS, I), N))
template <typename P BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(EFL_MAX_ARGS, I), typename E)>
struct do_eo_class_new<P 
  BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(EFL_MAX_ARGS, I), E) 
  BOOST_PP_REPEAT(I, EFL_CXX_do_eo_class_new_spec, ~) >
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			BOOST_PP_REPEAT(BOOST_PP_SUB(EFL_MAX_ARGS,I), EFL_CXX_get_eo_class, ~)
			, (void*)NULL);
  }
};

}
#endif // #if I

#if I != EFL_MAX_ARGS
namespace detail {

template <typename T>
inline void initialize_operation_description(tag<BOOST_PP_CAT(void_tag_, I)>, Eo2_Op_Description*)
{
}

template <typename D>
struct conversion_operator<D, BOOST_PP_CAT(void_tag_, I)> {};

}
#endif

#endif // !defined(BOOST_PP_IS_ITERATING)
