//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2012-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#if defined EFL_EINA_BOOST_MSVC
   #pragma warning (pop)
   #ifdef EFL_EINA_BOOST_MOVE_DETAIL_CRT_SECURE_NO_DEPRECATE
      #undef EFL_EINA_BOOST_MOVE_DETAIL_CRT_SECURE_NO_DEPRECATE
      #undef _CRT_SECURE_NO_DEPRECATE
   #endif
   #ifndef EFL_EINA_BOOST_MOVE_SCL_SECURE_NO_WARNINGS
      #undef  EFL_EINA_BOOST_MOVE_SCL_SECURE_NO_WARNINGS
      #undef _SCL_SECURE_NO_WARNINGS
   #endif
#endif
