// Copyright (C) 2004 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef EFL_EINA_BOOST_TYPE_TRAITS_MSVC_REMOVE_POINTER_HOLT_2004_0827
#define EFL_EINA_BOOST_TYPE_TRAITS_MSVC_REMOVE_POINTER_HOLT_2004_0827

#include <eina_boost/type_traits/msvc/typeof.hpp>
#include <eina_boost/type_traits/is_pointer.hpp>

namespace efl_eina_boost {
    namespace detail {
        template<int IsPointer>
        struct remove_pointer_impl_typeof {
            template<typename T,typename ID>
            struct inner {
                typedef T type;
            };
        };
        template<>
        struct remove_pointer_impl_typeof<true> {
            template<typename T,typename ID>
            struct inner {
                template<typename U>
                static msvc_register_type<U,ID> test(U*);
                static msvc_register_type<T,ID> test(...);
                EFL_EINA_BOOST_STATIC_CONSTANT(unsigned,register_test=sizeof(test( *((T*)NULL) ) ));
                typedef typename msvc_extract_type<ID>::id2type::type type;
            };
        };
    } //namespace detail

    template<typename T>
    struct remove_pointer {
        typedef typename efl_eina_boost::detail::remove_pointer_impl_typeof<
            efl_eina_boost::is_pointer<T>::value
        >::template inner<T,remove_pointer<T> >::type type;
        EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,remove_pointer,T)
    };
} //namespace efl_eina_boost

#endif //EFL_EINA_BOOST_TYPE_TRAITS_REMOVE_POINTER_HOLT_2004_0827
