
#ifndef EO3_EO_CALLBACK_HPP
#define EO3_EO_CALLBACK_HPP

#include "Eina.h"

namespace efl { namespace eo {

struct callback_token
{
  void* data;
  void* function;
  typedef void(*free_type)(void*);
  free_type free_function;
};

namespace detail {

template <typename F>
void free_callback(void* f)
{
  delete static_cast<F*>(f);
}

// XXX preprocessor
template <typename F, typename A0>
static Eina_Bool callback_function_object1(void* data, A0 a0)
{
  F* f = static_cast<F*>(data);
  (*f)(a0);
};

}

} }

#endif
