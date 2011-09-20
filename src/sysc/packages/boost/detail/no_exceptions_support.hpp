#ifndef SC_BOOST_DETAIL_NO_EXCEPTIONS_SUPPORT_HPP_
#define SC_BOOST_DETAIL_NO_EXCEPTIONS_SUPPORT_HPP_

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

//----------------------------------------------------------------------
// (C) Copyright 2004 Pavel Vozenilek.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// This file contains helper macros used when exception support may be
// disabled (as indicated by macro SC_BOOST_NO_EXCEPTIONS).
//
// Before picking up these macros you may consider using RAII techniques
// to deal with exceptions - their syntax can be always the same with 
// or without exception support enabled.
//

/* Example of use:

void foo() {
  SC_BOOST_TRY {
    ...
  } SC_BOOST_CATCH(const std::bad_alloc&) {
      ...
      SC_BOOST_RETHROW
  } SC_BOOST_CATCH(const std::exception& e) {
      ...
  }
  SC_BOOST_CATCH_END
}

With exception support enabled it will expand into:

void foo() {
  { try {
    ...
  } catch (const std::bad_alloc&) {
      ...
      throw;
  } catch (const std::exception& e) {
      ...
  }
  }
}

With exception support disabled it will expand into:

void foo() {
  { if(true) {
    ...
  } else if (false) {
      ...
  } else if (false)  {
      ...
  }
  }
}
*/
//----------------------------------------------------------------------

#include <sysc/packages/boost/config.hpp>
#include <sysc/packages/boost/detail/workaround.hpp>

#if !(defined SC_BOOST_NO_EXCEPTIONS)
#    define SC_BOOST_TRY { try
#    define SC_BOOST_CATCH(x) catch(x)
#    define SC_BOOST_RETHROW throw;
#    define SC_BOOST_CATCH_END }
#else
#    if SC_BOOST_WORKAROUND(__BORLANDC__, SC_BOOST_TESTED_AT(0x564))
#        define SC_BOOST_TRY { if ("")
#        define SC_BOOST_CATCH(x) else if (!"")
#    else
#        define SC_BOOST_TRY { if (true)
#        define SC_BOOST_CATCH(x) else if (false)
#    endif
#    define SC_BOOST_RETHROW
#    define SC_BOOST_CATCH_END }
#endif


#endif 
