#ifndef EINA_REF_HH
#define EINA_REF_HH

#ifdef EFL_EINA_CXX11
#include <functional>

namespace efl { namespace eina {

using std::ref;
using std::cref;
using std::reference_wrapper;

template <typename T>
T& unref(T& t)
{
   return t;
}
		
template <typename T>
T& unref(reference_wrapper<T> t)
{
   return t.get();
}

}}
#else
#include <eina_boost/reference_wrapper.hpp>

namespace efl { namespace eina {

using efl_eina_boost::ref;
using efl_eina_boost::cref;
using efl_eina_boost::reference_wrapper;
using efl_eina_boost::unref;

}}
#endif

#endif
