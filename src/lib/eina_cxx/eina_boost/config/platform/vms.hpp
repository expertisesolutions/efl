//  (C) Copyright Artyom Beilis 2010.  
//  Use, modification and distribution are subject to the  
//  Boost Software License, Version 1.0. (See accompanying file  
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 

#ifndef EFL_EINA_BOOST_CONFIG_PLATFORM_VMS_HPP 
#define EFL_EINA_BOOST_CONFIG_PLATFORM_VMS_HPP 

#define EFL_EINA_BOOST_PLATFORM "OpenVMS" 

#undef  EFL_EINA_BOOST_HAS_STDINT_H 
#define EFL_EINA_BOOST_HAS_UNISTD_H 
#define EFL_EINA_BOOST_HAS_NL_TYPES_H 
#define EFL_EINA_BOOST_HAS_GETTIMEOFDAY 
#define EFL_EINA_BOOST_HAS_DIRENT_H 
#define EFL_EINA_BOOST_HAS_PTHREADS 
#define EFL_EINA_BOOST_HAS_NANOSLEEP 
#define EFL_EINA_BOOST_HAS_CLOCK_GETTIME 
#define EFL_EINA_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE 
#define EFL_EINA_BOOST_HAS_LOG1P 
#define EFL_EINA_BOOST_HAS_EXPM1 
#define EFL_EINA_BOOST_HAS_THREADS 
#undef  EFL_EINA_BOOST_HAS_SCHED_YIELD 

#endif 
