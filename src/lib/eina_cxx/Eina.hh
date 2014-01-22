#ifndef EINA_HH_
#define EINA_HH_

#include <eina_config.hh>
#include <eina_callback_helper.hh>
#include <eina_defs.hh>
#include <eina_iterator.hh>
#include <eina_ptrarray.hh>
#include <eina_ptrlist.hh>
#include <eina_inarray.hh>
#include <eina_inlist.hh>
#include <eina_stringshare.hh>
#include <eina_error.hh>
#include <eina_accessor.hh>
#include <eina_thread.hh>
#include <eina_value.hh>
#include <eina_ref.hh>

namespace efl { namespace eina {

struct eina_init
{
  eina_init()
  {
    ::eina_init();
  }
  ~eina_init()
  {
    ::eina_shutdown();
  }
};

struct eina_threads_init
{
  eina_threads_init()
  {
    ::eina_threads_init();
  }
  ~eina_threads_init()
  {
    ::eina_threads_shutdown();
  }
};

} }

#endif
