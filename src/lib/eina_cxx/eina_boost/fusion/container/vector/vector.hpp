/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_VECTOR_07072005_1244)
#define FUSION_VECTOR_07072005_1244

#include <eina_boost/fusion/container/vector/vector_fwd.hpp>
#include <eina_boost/fusion/container/vector/detail/vector_n_chooser.hpp>
#include <eina_boost/fusion/sequence/intrinsic/begin.hpp>
#include <eina_boost/mpl/at.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/type_traits/add_reference.hpp>
#include <eina_boost/type_traits/add_const.hpp>
#include <eina_boost/type_traits/is_base_of.hpp>
#include <eina_boost/detail/workaround.hpp>

#if !defined(__WAVE__)

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1600)

#define EFL_EINA_BOOST_FUSION_VECTOR_COPY_INIT()                                       \
    ctor_helper(rhs, is_base_of<vector, Sequence>())                          \

#define EFL_EINA_BOOST_FUSION_VECTOR_CTOR_HELPER()                                     \
    static vector_n const&                                                    \
    ctor_helper(vector const& rhs, mpl::true_)                                \
    {                                                                         \
        return rhs.vec;                                                       \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    static T const&                                                           \
    ctor_helper(T const& rhs, mpl::false_)                                    \
    {                                                                         \
        return rhs;                                                           \
    }

#else

#define EFL_EINA_BOOST_FUSION_VECTOR_COPY_INIT()                                       \
    rhs                                                                       \

#define EFL_EINA_BOOST_FUSION_VECTOR_CTOR_HELPER()

#endif

#endif // !defined(__WAVE__)

#if !defined(EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <eina_boost/fusion/container/vector/detail/preprocessed/vector.hpp>
#else
#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/vvector" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace efl_eina_boost { namespace fusion
{
    struct void_;
    struct fusion_sequence_tag;

    template <EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct vector
        : sequence_base<vector<EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> >
    {
    private:

        typedef typename detail::vector_n_chooser<
            EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)>::type
        vector_n;

        template <EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        friend struct vector;

    public:

        typedef typename vector_n::types types;
        typedef typename vector_n::fusion_tag fusion_tag;
        typedef typename vector_n::tag tag;
        typedef typename vector_n::size size;
        typedef typename vector_n::category category;
        typedef typename vector_n::is_view is_view;

        vector()
            : vec() {}

        template <EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        vector(vector<EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
            : vec(rhs.vec) {}

        vector(vector const& rhs)
            : vec(rhs.vec) {}

#if !defined(EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES)
        vector(vector&& rhs)
            : vec(std::forward<vector_n>(rhs.vec)) {}
#endif

        template <typename Sequence>
        vector(Sequence const& rhs)
            : vec(EFL_EINA_BOOST_FUSION_VECTOR_COPY_INIT()) {}

        //  Expand a couple of forwarding constructors for arguments
        //  of type (T0), (T0, T1), (T0, T1, T2) etc. Example:
        //
        //  vector(
        //      typename detail::call_param<T0>::type _0
        //    , typename detail::call_param<T1>::type _1)
        //    : vec(_0, _1) {}
        #include <eina_boost/fusion/container/vector/detail/vector_forward_ctor.hpp>

        template <EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        vector&
        operator=(vector<EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
        {
            vec = rhs.vec;
            return *this;
        }

        template <typename T>
        vector&
        operator=(T const& rhs)
        {
            vec = rhs;
            return *this;
        }

        vector&
        operator=(vector const& rhs)
        {
            vec = rhs.vec;
            return *this;
        }

#if !defined(EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES)
        vector&
        operator=(vector&& rhs)
        {
            vec = std::forward<vector_n>(rhs.vec);
            return *this;
        }

        template <typename T>
        vector&
        operator=(T&& rhs)
        {
            vec = std::forward<T>(rhs);
            return *this;
        }
#endif

        template <int N>
        typename add_reference<
            typename mpl::at_c<types, N>::type
        >::type
        at_impl(mpl::int_<N> index)
        {
            return vec.at_impl(index);
        }

        template <int N>
        typename add_reference<
            typename add_const<
                typename mpl::at_c<types, N>::type
            >::type
        >::type
        at_impl(mpl::int_<N> index) const
        {
            return vec.at_impl(index);
        }

        template <typename I>
        typename add_reference<
            typename mpl::at<types, I>::type
        >::type
        at_impl(I /*index*/)
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename add_reference<
            typename add_const<
                typename mpl::at<types, I>::type
            >::type
        >::type
        at_impl(I /*index*/) const
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

    private:

        EFL_EINA_BOOST_FUSION_VECTOR_CTOR_HELPER()
        vector_n vec;
    };
}}

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
