#ifndef EINA_HH_
#define EINA_HH_

#include <eina_iterator.hh>
#include <eina_ptrlist.hh>
#include <eina_inarray.hh>
#include <eina_inlist.hh>
#include <eina_stringshare.hh>
#include <eina_error.hh>

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

} }

#endif
