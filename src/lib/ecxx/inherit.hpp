
///
/// @file inherit.hpp
///

#ifndef EO3_INHERIT_HPP
#define EO3_INHERIT_HPP

#include "eo_private.hpp"

#include <boost/mpl/bool.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/at.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#include <cassert>

namespace efl { namespace eo {

/// @addtogroup Efl_Cxx_API
/// @{

/// @brief Template-class that allows client code to inherit from
/// <em>EO C++ Classes</em> without the need to make explicit calls to
/// <em>EO</em> methods --- that would naturally be necessary to
/// register itself in the <em>EO Subsystem</em>.
/// 
/// @param D The derived class
/// @param O The parent class
/// @param En Class extensions (either mixins or interfaces)
/// 
/// The derived class @p D will inherit all EO operations and event
/// callbacks from the parent class @p P, as well as from the <c>Base
/// Class</c> (@ref efl::eo::base) since every EO C++ Class must
/// inherit from it.
///
/// efl::eo::inherit makes use of meta-template elements to build (in
/// compile-time) code capable of registering @p D as an <em>EO
/// Class</em> within <em>EO Subsystem</em>. Each class is registered
/// only once upon instantiation of an object of its type.
/// 
/// @note Function overriding is currently not supported.
///
#ifdef GENERATING_DOCUMENTATION
template <typename D, typename P, typename... En>
#else
template <typename D, typename P
	  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(EFL_MAX_ARGS, typename E, = detail::void_tag_) >
#endif
struct inherit;

/// @}

namespace detail {

/// @addtogroup Efl_Cxx_Detail
/// @{

#if GENERATING_DOCUMENTATION
/// @internal
/// 
/// @brief Invokes the <em>EO C Constructor</em> that corresponds to the
/// binded <em>EO C++ Class</em>.
///
/// @param T The corresponding <em>EO C++ Class</em>
/// @param Args An heterogeneous list of constructor arguments
///
/// @param tag Used to instruct the compiler during compile-time which
/// of the overloads should be invoked.
/// @param eo A pointer to the <em>EO C Object</em> to be constructed.
/// @param cls Unused.
/// @param args An heterogenous vector containing the constructor
/// arguments, in the correct order.
///
/// To ensure full reciprocity of the C++ binding there must exist one
/// (and only one) implementation of @ref efl::eo::detail::call_constructor
/// for each available <em>EO C++ Class</em> --- the implementations
/// are differentiated by this unique specialization of
/// @ref efl::eo::detail::tag for the first argument of
/// @ref efl::eo::detail::call_constructor.
///
/// For example this is how the overload for @ref eo3_simple is
/// written as follows:
/// 
/// @dontinclude eo3_simple.hpp
/// @skip call_constructor
/// @until }
/// 
/// As you can see @c ::simple_constructor is called with a single
/// argument in this case. Each EO Class has its own constructor
/// prototype -- which can have different argument types as well as
/// distinct number of arguments, etc. -- hence the need to specify a
/// choice for every known <em>EO C++ Class</em>.
///
/// @see efl::eo::detail::tag
///
template <typename T, typename Args>
void call_constructor(efl::eo::detail::tag<T> tag, Eo* eo, Eo_Class const* cls, Args args);
#endif

/// @internal
/// 
/// @brief The void "specialization" of
/// efl::eo::detail::call_constructor. Its implementation is empty so it
/// generates no code.
/// 
/// @see efl::eo::detail::call_constructor
///
template <typename Args>
inline void call_constructor(tag<void>, Eo*, Eo_Class const*, Args) {}

/// @internal
/// 
/// @brief Sums up the number of <em>EO Operations</em> of each class
/// passed as argument to the template.
///
/// @see efl::eo::detail::operation_description_class_size
/// 
#ifdef GENERATING_DOCUMENTATION
template <typename P, typename... En>
#else
template <typename P BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(EFL_MAX_ARGS, typename E, = detail::void_tag_)>
#endif
struct operation_description_size
{
#define EFL_CXX_operation_description_class_size_val(Z, N, DATA) \
  + operation_description_class_size<BOOST_PP_CAT(E, N)>::value
  static const int value = operation_description_class_size<P>::value
    BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_operation_description_class_size_val, ~);
#undef EFL_CXX_operation_description_class_size_val
};

/// @internal
/// 
/// @brief Predicate to @c boost::mpl algorithms.
///
/// @param T An <em>EO C++ Class</em>.
///
/// This predicate has its @ref efl::eo::detail::predicate::apply::type
/// identical to @c boost::mpl::true_ if @p T matches an @ref
/// efl::eo::detail::args_class<T, Seq>, othewise it is identical to
/// boost::mpl::false_.
///
/// @see call_constructor_aux
///
template <typename T>
struct predicate
{
  /// @internal 
  /// @brief Selected when the predicate does <b>not</b> apply to @c T.
  /// 
  template <typename Args>
  struct apply 
  { 
    typedef boost::mpl::false_ type;
  };

  /// @internal 
  /// @brief Selected when the predicate applies to @c T.
  /// 
  template <typename V>
  struct apply<args_class<T, V> >
  {
    typedef boost::mpl::true_ type;
  };
};

/// @internal
/// 
/// @brief An auxiliary template-class used to select the correct
/// implementation of @ref efl::eo::call_constructor for @p T with
/// proper parameters and variadic size.
///
/// @param T An <em>EO C++ Class</em>.
///
template <typename T>
struct call_constructor_aux
{
  /// @internal
  /// 
  /// @brief Invoke the @c void "specialization" of @ref
  /// efl::eo::detail::call_constructor.
  /// 
  /// @param args An heterogenous sequence of arguments.
  /// @param eo The opaque <em>EO Object</em>.
  /// @param cls The opaque <em>EO Class</em>.
  /// 
  template <typename I, typename Args>
  static void do_aux(Args const&, Eo* eo, Eo_Class const* cls, boost::mpl::true_)
  {
    detail::call_constructor(tag<void>(), eo, cls
                             , args_class<T, boost::fusion::vector0<> >
                             (boost::fusion::vector0<>()));
  }

  /// @internal
  /// 
  /// @brief Invoke the "specialization" of @ref
  /// efl::eo::detail::call_constructor for @c T.
  ///
  /// @param args An heterogenous sequence of arguments.
  /// @param eo The opaque <em>EO Object</em>.
  /// @param cls The opaque <em>EO Class</em>.
  /// 
  template <typename I, typename Args>
  static void do_aux(Args const& args, Eo* eo, Eo_Class const* cls, boost::mpl::false_)
  {
    typedef typename
      boost::mpl::distance<typename boost::mpl::begin<Args>::type, I>::type
      index_type;
    detail::call_constructor
      (tag<T>(), eo, cls
       , boost::fusion::at<index_type>(args));
  }

  /// @internal
  /// 
  /// @brief Invoke @def efl::eo::detail::call_constructor
  /// implementation for the parent and each available extension.
  /// 
  /// @param args An heterogenous sequence of arguments.
  /// @param eo The opaque <em>EO Object</em>.
  /// @param cls The opaque <em>EO Class</em>.
  /// 
  template <typename Args>
  static void do_(Args const& args, Eo* eo, Eo_Class const* cls)
  {
    typedef typename boost::mpl::find_if<Args, predicate<T> >::type
      iterator;
    do_aux<iterator>(args
                     , eo, cls
		     , typename boost::is_same
		     <
                       typename boost::mpl::end<Args>::type
		       , iterator
		     >::type());
  }
};

/// @internal
/// 
/// @brief The procedure that is invoked when the constructor of @c D
/// is sought from the <em>EO Subsystem</em>.
///
/// @param obj The opaque <em>EO Object</em>.
/// @param self A pointer to @p obj's private data.
/// @param this_ A void pointer to the opaque <em>EO Class</em> ---
/// passed as <em>user data</em>.
/// @param args The arguments for the underlying constructor.
///
template <typename D, typename P
  BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, typename E)
  , typename Args>
void inherit_constructor_impl(Eo* obj, Inherit_Private_Data* self, void* this_, Args args)
{
  self->this_ = this_;
  Eo_Class const* cls
    = static_cast<inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>*>(this_)->_eo_class();
  detail::call_constructor_aux<P>::do_(args, obj, cls);

#define EFL_CXX_call_constructor_aux_rep(Z, N, DATA) \
  detail::call_constructor_aux<BOOST_PP_CAT(E, N)>::do_(args, obj, cls);
  BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_call_constructor_aux_rep, ~);
#undef EFL_CXX_call_constructor_aux_rep
}

/// @internal
/// 
/// @brief Find the correct function for the <em>"constructor"</em>
/// operation and invoke it.
///
/// @param this_ The <em>user data</em> to be passed to the resolved function.
/// @param args An heterogeneous sequence of arguments.
///
template <typename P
  BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, typename E)
  , typename Args>
EAPI void inherit_constructor(void* this_, Args args)
{
  typedef void (*func_t)(Eo *, void *, void*, Args);
  Eo2_Op_Call_Data call;
  static Eo_Op op = EO_NOOP;
  if ( op == EO_NOOP )
    op = eo2_api_op_id_get((void*)&inherit_constructor
                           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E), Args>
                           , EO_OP_TYPE_REGULAR);
  if (!eo2_call_resolve(op, &call)) assert(eo2_call_resolve(op, &call));
  func_t func = (func_t) call.func;
  return func(call.obj, call.data, this_, args);
}

/// @internal
/// 
/// @brief This function is responsible for declaring a new <em>EO C
/// Class</em> representing @p D within <em>EO Subsystem</em>.
/// 
/// @param D The derived class
/// @param P The parent class
/// @param En Class extensions (either mixins or interfaces)
/// @param Args An heterogeneous list of arguments to be passed to the
/// constructor of this class.
///
/// @see efl::eo::inherit::inherit
/// 
#ifdef GENERATING_DOCUMENTATION
template <typename D, typename P, typename... En, typename Args>
#else
template <typename D, typename P
  BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, typename E)
  , typename Args>
#endif
Eo_Class const* create_class(/*info*/)
{
  static const Eo_Class* my_class = NULL;
  static Eo2_Op_Description op_descs
    [ detail::operation_description_size
      <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value + 2 ];

  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value].func =
    reinterpret_cast<void*>
    (
     static_cast<void(*)(Eo*, Inherit_Private_Data*, void*, Args)>
     (&detail::inherit_constructor_impl<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E), Args>)
    );

  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value].api_func =
    reinterpret_cast<void*>
    (
     static_cast<void(*)(void*, Args)>
     (&detail::inherit_constructor<P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E), Args>)
    );

  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value].op = EO_NOOP;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value].op_type = EO_OP_TYPE_REGULAR;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value].doc = "";
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value+1].func = 0;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value+1].api_func = 0;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value+1].op = 0;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value+1].op_type = EO_OP_TYPE_INVALID;
  op_descs[detail::operation_description_size
           <P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::value+1].doc = 0;

  detail::initialize_operation_description
    <inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)> >
    (detail::tag<P>(), op_descs);

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PP_DEC(EFL_MAX_ARGS), "inherit_pp_operation_description.hpp"))
#include BOOST_PP_ITERATE()

  //locks
  if(!my_class)
  {
    static Eo_Class_Description class_desc = {
      EO2_VERSION,
      "C++ Inherited class",
      EO_CLASS_TYPE_REGULAR,
      EO2_CLASS_DESCRIPTION_OPS(op_descs),
      NULL,
      sizeof(detail::Inherit_Private_Data),
      NULL,
      NULL
    };
    my_class = detail::do_eo_class_new<P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>::do_(class_desc);
  }
  return my_class;
}

} // namespace detail

#define EFL_CXX_inherit_operations_rep(Z, N, DATA) \
  , detail::operations<BOOST_PP_CAT(E, N)>::template type<inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)> >

#define EFL_CXX_inherit_conversion_operator_rep(Z, N, DATA) \
, detail::conversion_operator<inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>, BOOST_PP_CAT(E, N)>

/// @addtogroup Efl_Cxx_API
/// @{

template <typename D, typename P
    BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, typename E)>
struct inherit
    : detail::operations<P>::template type<inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)> >
    BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_inherit_operations_rep, ~)
    , detail::conversion_operator<inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)>, P>
    BOOST_PP_REPEAT(EFL_MAX_ARGS, EFL_CXX_inherit_conversion_operator_rep, ~)
{
  /// @typedef inherit_base
  ///
  typedef inherit<D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)> inherit_base;

  /// @brief Class constructor.
  ///
  /// @ref inherit has a "variadic" constructor implementation that
  /// allows from zero to EFL_MAX_ARGS heterogeneous parameters.
  ///
#ifdef GENERATING_DOCUMENTATION
  template<typename... An> inherit(An... args);
#else
# define BOOST_PP_ITERATION_PARAMS_1 (3, (0, EFL_MAX_ARGS, "inherit_pp_constructor.hpp"))
# include BOOST_PP_ITERATE()
#endif

  /// @brief Class destructor.
  ///
  ~inherit()
  {
    eo_unref(_eo_raw);
  }

  /// @brief Gets the <em>EO Object</em> corresponding to this <em>EO
  /// C++ Object</em>.
  /// 
  /// @return A pointer to the <em>EO Object</em>.
  /// 
  Eo* _eo_ptr() const { return _eo_raw; }

  /// @brief Gets the <em>EO Class</em> corresponding to this <em>EO
  /// C++ Class</em>.
  /// 
  /// @return A pointer to the <em>EO Class</em>.
  /// 
  Eo_Class const* _eo_class() const { return _eo_cls; }

protected:
  /// @brief Copy constructor.
  /// 
  inherit(inherit const& other)
    : _eo_cls(other._eo_cls)
    , _eo_raw(other._eo_raw)
  { eo_ref(_eo_raw); }

  /// @brief Assignment Operator
  /// 
  inherit& operator=(inherit const& other)
  { 
    _eo_cls = other._eo_cls;
    _eo_raw = other._eo_raw;
    eo_ref(_eo_raw);
    return *this;
  }

private:
  Eo_Class const* _eo_cls;   ///< The <em>EO Class</em>.
  Eo* _eo_raw;               ///< The <em>EO Object</em>
};

/// @} 

#undef EFL_CXX_inherit_operations_rep
#undef EFL_CXX_inherit_conversion_operator_rep

} }

#endif // EO3_INHERIT_HPP
