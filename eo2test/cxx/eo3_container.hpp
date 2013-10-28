
#ifndef GENCXX_EO3_CONTAINER_HPP
#define GENCXX_EO3_CONTAINER_HPP

/*
 * #define EO3_CONTAINER_CLASS eo3_container_class                             \
 *    , constructor_override(eo2_constructor, _constructor)                    \
 *    , destructor(_destructor)                                                \
 *    , function(eo3_container_add, _add, void, EO_TYPE(EO3_CONTAINER_CLASS) ) \
 *    , function(eo3_container_add2, _add2, void, EO_TYPE(EO3_SIMPLE_CLASS) )
 *
 * EO3_DECLARE_CLASS(EO3_CONTAINER_CLASS)
 */

extern "C" {
#include "eo3_container.h"
}

#include "eo3_simple.hpp"

struct eo3_container : efl::eo::Base
{
  explicit eo3_container(Eo* eo)
    : efl::eo::Base(eo) {}

  eo3_container()
    : efl::eo::Base(_c1())
  {
  }

  void eo3_container_add(eo3_container const& eo)
  {
    std::cout << "eo3_container::eo3_container_add [eo3_container]" << std::endl; // XXX
    eo2_do(_eo_ptr(), ::eo3_container_add(eo._eo_ptr()));
  }

  void eo3_container_add(Eo* eo)
  {
    std::cout << "eo3_container::eo3_container_add [EO3_CONTAINER_CLASS]" << std::endl; // XXX
    assert(eo_isa(eo, EO3_GET_CLASS(EO3_CONTAINER_CLASS)));
    eo2_do(_eo_ptr(), ::eo3_container_add(eo));
  }

  void eo3_container_add2(eo3_simple const& eo)
  {
    std::cout << "eo3_container::eo3_container_add2 [eo3_simple]" << std::endl; // XXX
    eo2_do(_eo_ptr(), ::eo3_container_add2(eo._eo_ptr()));
  }

  void eo3_container_add2(Eo* eo)
  {
    std::cout << "eo3_container::eo3_container_add2 [EO3_SIMPLE_CLASS]" << std::endl; // XXX
    assert(eo_isa(eo, EO3_GET_CLASS(EO3_SIMPLE_CLASS)));
    eo2_do(_eo_ptr(), ::eo3_container_add2(eo));
  }

private:
  static Eo* _c1()
  {
    return eo2_add(EO3_GET_CLASS(EO3_CONTAINER_CLASS), NULL);
  }
};

#endif // GENCXX_EO3_CONTAINER_HPP

