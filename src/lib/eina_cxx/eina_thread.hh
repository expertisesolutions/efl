#ifndef EINA_THREAD_HH_
#define EINA_THREAD_HH_

#include <Eina.h>
#include <eina_error.hh>
#include <eina_defs.hh>
#include <eina_boost/config.hpp>

#include <eina_boost/fusion/include/vector.hpp>
#include <eina_boost/fusion/include/make_vector.hpp>
#include <eina_boost/fusion/include/at_c.hpp>
#include <eina_boost/fusion/include/fused.hpp>
#include <eina_boost/fusion/include/advance.hpp>

#include <eina_boost/move/utility.hpp>

#include <memory>
#include <iterator>
#include <cstdlib>
#include <cassert>
#include <iosfwd>
#include <functional>
#if __cplusplus >= 201103L
#include <chrono>
#else
#include <eina_boost/preprocessor/iterate.hpp>
#include <eina_boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#endif

namespace efl { namespace eina {

struct mutex
{
  typedef Eina_Lock* native_handle_type;

  mutex()
  {
    ::eina_lock_new(&_mutex);
  }
  ~mutex()
  {
    ::eina_lock_free(&_mutex);
  }
  void lock()
  {
    ::Eina_Lock_Result r = ::eina_lock_take(&_mutex);
    switch(r)
      {
      case EINA_LOCK_SUCCEED:
        return;
      case EINA_LOCK_DEADLOCK:
        throw system_error(error_code(eina::errc::resource_deadlock_would_occur
                                      , get_generic_category()));
      default:
        throw system_error(get_error_code());
      }
  }
  bool try_lock()
  {
    ::Eina_Lock_Result r = ::eina_lock_take_try(&_mutex);
    switch(r)
      {
      case EINA_LOCK_SUCCEED:
        return true;
      case EINA_LOCK_FAIL:
        return false;
      case EINA_LOCK_DEADLOCK:
        throw system_error(error_code(eina::errc::resource_deadlock_would_occur
                                      , get_generic_category()));
      default:
        throw system_error(get_error_code());
      }
  }
  void unlock()
  {
    ::Eina_Lock_Result r = ::eina_lock_release(&_mutex);
    switch(r)
      {
      case EINA_LOCK_SUCCEED:
        return;
      case EINA_LOCK_DEADLOCK:
        throw system_error(error_code(eina::errc::resource_deadlock_would_occur
                                      , get_generic_category()));
      default:
        throw system_error(get_error_code());
      }
  }
  void debug()
  {
    ::eina_lock_debug(&_mutex);
  }
  native_handle_type native_handle()
  {
    return &_mutex;
  }
private:
#if __cplusplus >= 201103L
  mutex(mutex const&) = delete;
  mutex& operator=(mutex const&) = delete;
#else
  mutex(mutex const&);
  mutex& operator=(mutex const&);
#endif
  Eina_Lock _mutex;
};

#if __cplusplus >= 201103L
using std::lock_guard;
using std::unique_lock;
#else
struct adopt_lock_t {};
struct try_to_lock_t {};
struct defer_lock_t {};
adopt_lock_t const adopt_lock = {};
try_to_lock_t const try_to_lock = {};
defer_lock_t const defer_lock = {};

template <typename Mutex>
struct lock_guard
{
  typedef Mutex mutex_type;

  explicit lock_guard(mutex_type& m)
    : _mutex(&m)
  {
    _mutex->lock();
  }
  lock_guard(mutex_type& m, adopt_lock_t)
    : _mutex(&m)
  {
  }
  ~lock_guard()
  {
    _mutex->unlock();
  }
private:
  mutex_type* _mutex;
};

template <typename Mutex>
class unique_lock
{
  EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE(unique_lock<Mutex>)
public:
  typedef Mutex mutex_type;

  unique_lock(EFL_EINA_BOOST_RV_REF(unique_lock<Mutex>) other)
    :  _mutex(other._mutex), _locked(other._mutex)
  {}
  unique_lock<Mutex>& operator=(EFL_EINA_BOOST_RV_REF(unique_lock<Mutex>) other)
  {
    _mutex = other._mutex;
    _locked = other._locked;
    return *this;
  }  
  unique_lock() EFL_EINA_BOOST_NOEXCEPT : _mutex(0), _locked(false) {}
  explicit unique_lock(mutex_type& m)
    : _mutex(&m), _locked(false)
  {
    lock();
  }
  unique_lock(mutex_type& m, adopt_lock_t)  EFL_EINA_BOOST_NOEXCEPT
    : _mutex(&m), _locked(true) {}
  unique_lock(mutex_type& m, try_to_lock_t)  EFL_EINA_BOOST_NOEXCEPT
    : _mutex(&m), _locked(false)
  {
    try_lock();
  }
  unique_lock(mutex_type& m, defer_lock_t)  EFL_EINA_BOOST_NOEXCEPT
    : _mutex(&m), _locked(false) {}
  ~unique_lock()
  {
    if(owns_lock())
      unlock();
  }
  bool owns_lock() EFL_EINA_BOOST_NOEXCEPT
  {
    return _locked;
  }
  bool try_lock()
  {
    if(_mutex->try_lock())
      return _locked = true;
    else
      return false;
  }
  void lock()
  {
    assert(!owns_lock());
    _mutex->lock();
    _locked = true;
  }
  void unlock()
  {
    assert(owns_lock());
    _mutex->unlock();
    _locked = false;
  }
  mutex_type* mutex()
  {
    return &_mutex;
  }
  mutex_type const* mutex() const
  {
    return &_mutex;
  }
private:
  mutex_type* _mutex;
  bool _locked;
};
#endif

struct condition_variable
{
  typedef Eina_Condition* native_handle_type;

  condition_variable()
  {
    ::eina_condition_new(&_cond, _mutex.native_handle());
  }
  ~condition_variable()
  {
    ::eina_condition_free(&_cond);
  }

  void notify_one()
  {
    eina::unique_lock<eina::mutex> l(_mutex);
    Eina_Bool r = eina_condition_signal(&_cond);
    if(!r)
      throw eina::system_error(eina::get_error_code());
  }
  void notify_all()
  {
    eina::unique_lock<eina::mutex> l(_mutex);
    Eina_Bool r = eina_condition_broadcast(&_cond);
    if(!r)
      throw eina::system_error(eina::get_error_code());
  }
  template <typename Lock>
  void wait(Lock& lock)
  {
    eina::unique_lock<eina::mutex> l(_mutex);
    lock.unlock();
    ::eina_condition_wait(&_cond);
    lock.lock();
  }
  template <typename Lock, typename Predicate>
  void wait(Lock& lock, Predicate p)
  {
    while(!p())
      wait(lock);
  }
  native_handle_type native_handle()
  {
    return &_cond;
  }
private:
  condition_variable(condition_variable const&);
  condition_variable& operator=(condition_variable const&);

  mutex _mutex;
  Eina_Condition _cond;
};

struct thread_id
{
  thread_id() EFL_EINA_BOOST_NOEXCEPT
    : _raw(0u)
  {
  }
  thread_id(Eina_Thread raw)
    : _raw(raw) {}
  friend inline bool operator==(thread_id lhs, thread_id rhs)
  {
    return lhs._raw == rhs._raw;
  }
  friend inline bool operator!=(thread_id lhs, thread_id rhs)
  {
    return lhs._raw != rhs._raw;
  }
  friend inline bool operator<(thread_id lhs, thread_id rhs)
  {
    return std::less<Eina_Thread>()(lhs._raw, rhs._raw);
  }
private:
  Eina_Thread _raw;
};

inline bool operator<=(thread_id lhs, thread_id rhs)
{
  return (lhs == rhs) || lhs < rhs;
}
inline bool operator>(thread_id lhs, thread_id rhs)
{
  return !(lhs <= rhs);
}
inline bool operator>=(thread_id lhs, thread_id rhs)
{
  return !(lhs < rhs);
}

template <typename charT, typename Traits>
std::basic_ostream<charT, Traits>&
operator<<(std::basic_ostream<charT, Traits>& out, thread_id id)
{
  return out << id._raw;
}

namespace _detail {

template <typename Arguments>
void* create_thread(void* data, Eina_Thread)
{
  namespace fusion = ::efl_eina_boost::fusion;

  Arguments* args = static_cast<Arguments*>(data);
  Arguments local_args = *args;

  typedef typename remove_pointer
    <typename remove_reference
     <typename fusion::result_of::at_c<Arguments, 0u>::type>
     ::type>::type function_type;

  function_type
#if __cplusplus >= 201103L
  && f = std::move(
#else
     f =
#endif
              *fusion::at_c<0u>(local_args)
#if __cplusplus >= 201103L
              )
#endif
              ;

  eina_lock_take(&fusion::at_c<2u>(*args));
  fusion::at_c<3u>(*args) = true;
  eina_condition_signal(&fusion::at_c<1u>(*args));
  eina_lock_release(&fusion::at_c<2u>(*args));

  fusion::fused<function_type> ff(f);
  fusion::iterator_range
     <typename fusion::result_of::advance_c
     <typename fusion::result_of::begin<Arguments>::type
     , 4u>::type
     , typename fusion::result_of::end<Arguments>::type
     >
    range(fusion::advance_c<4u>(fusion::begin(local_args))
          , fusion::end(local_args));
  ff(range);
  
  return 0;
}

}

class thread
{
   EFL_EINA_BOOST_MOVABLE_BUT_NOT_COPYABLE(thread)
public:
  typedef thread_id id;
  typedef Eina_Thread native_handle_type;

  thread() EFL_EINA_BOOST_NOEXCEPT
    : _joinable(false), _raw(0u)
  {
  }

#if __cplusplus >= 201103L
  template <typename F, class ... Args>
  explicit thread(F&& f, Args&&... args)
  {
    namespace fusion = ::efl_eina_boost::fusion;
    typedef typename fusion::result_of::make_vector
      <F*, Eina_Condition, Eina_Lock, bool, Args...>
      ::type arguments_type;

    arguments_type arguments (&f, {}, {}, false
                              , args...);
    init(arguments);
  }
#else
#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 (3, (0, EFL_EINA_MAX_ARGS, "eina_thread_constr.x"))
#include EFL_EINA_BOOST_PP_ITERATE()
#endif
  thread(EFL_EINA_BOOST_RV_REF(thread) other)
    : _joinable(other._joinable), _raw(other._raw)
  {
  }

  thread& operator=(EFL_EINA_BOOST_RV_REF(thread) other)
  {
    _raw = other._raw;
    _joinable = other._joinable;
    return *this;
  }

  ~thread()
  {
    assert(!joinable());
  }
 
  void swap(thread& other) EFL_EINA_BOOST_NOEXCEPT
  {
    std::swap(_raw, other._raw);
  }
  bool joinable() const EFL_EINA_BOOST_NOEXCEPT
  {
    return _joinable;
  }

  void join()
  {
    assert(joinable());
    ::eina_thread_join(_raw);
    _joinable = false;
  }

  void detach()
  {
    assert(joinable());
    _joinable = false;
  }

  id get_id() const EFL_EINA_BOOST_NOEXCEPT
  {
    return id(_raw);
  }
  native_handle_type native_handle() const
  {
    return _raw;
  }

  static unsigned hardware_concurrency() EFL_EINA_BOOST_NOEXCEPT;
private:
  template <typename Arguments>
  void init(Arguments& arguments)
  {
    _joinable = true;
    namespace fusion = ::efl_eina_boost::fusion;
    Eina_Bool r = ::eina_lock_new(&fusion::at_c<2u>(arguments));
    if(!r) throw eina::system_error(eina::get_error_code());
    r = ::eina_condition_new(&fusion::at_c<1u>(arguments)
                             , &fusion::at_c<2u>(arguments));
    if(!r) throw eina::system_error(eina::get_error_code());

    if(!eina_thread_create
       (&_raw, ::EINA_THREAD_NORMAL
        , -1, &eina::_detail::create_thread<Arguments>
        , &arguments))
      {
        eina_condition_free(&fusion::at_c<1u>(arguments));
        eina_lock_free(&fusion::at_c<2u>(arguments));
        throw eina::system_error(eina::get_error_code());
      }
    Eina_Lock_Result lr = ::eina_lock_take(&fusion::at_c<2u>(arguments));
    if(lr != EINA_LOCK_SUCCEED)
      throw eina::system_error(eina::get_error_code());
    while(!fusion::at_c<3u>(arguments))
      {
        r = eina_condition_wait(&fusion::at_c<1u>(arguments));
        if(!r) throw eina::system_error(eina::get_error_code());
      }
    lr = eina_lock_release(&fusion::at_c<2u>(arguments));
    if(lr != EINA_LOCK_SUCCEED)
      throw eina::system_error(eina::get_error_code());

    eina_condition_free(&fusion::at_c<1u>(arguments));
    eina_lock_free(&fusion::at_c<2u>(arguments));
  }

  bool _joinable;
  Eina_Thread _raw;
};

inline void swap(thread& lhs, thread& rhs)
{
  lhs.swap(rhs);
}

namespace this_thread {

inline thread::id get_id()
{
  return thread::id(eina_thread_self());
}

inline void yield() {}

#if __cplusplus >= 201103L
template <typename Clock, typename Duration>
void sleep_until(std::chrono::time_point<Clock, Duration>const& abs_time);

template <typename Rep, typename Period>
void sleep_for(std::chrono::duration<Rep, Period>const& rel_time);
#endif
}

} }

namespace std {

#if __cplusplus >= 201103L
template <typename T> struct hash;
template <>
struct hash< ::efl::eina::thread_id> : hash<unsigned long>
{};
#endif

}

#endif
