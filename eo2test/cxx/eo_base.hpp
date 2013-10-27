
#ifndef EO3_EO_BASE_HPP
#define EO3_EO_BASE_HPP

namespace efl { namespace eo {

struct Base
{
  explicit Base(Eo* eo) : _eo_raw(eo)
  {
  }

  ~Base()
  {
    std::cout << "~Base" << std::endl;
    eo_unref(_eo_raw);
  }

  Base& operator=(Base const& other)
  {
    eo_ref(_eo_raw);
    return *this;
  }

  Eo* _eo_ptr() const { return _eo_raw; }
protected:
  Eo* _eo_raw;
};

}}

#endif // EO3_EO_BASE_HPP
