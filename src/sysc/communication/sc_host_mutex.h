/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_host_mutex.h -- A "real" mutex for the underlying host system

  Original Author: Philipp A. Hartmann, OFFIS

 CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_HOST_MUTEX_H_INCLUDED_
#define SC_HOST_MUTEX_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/communication/sc_mutex_if.h"

#if SC_CPLUSPLUS >= 201103L
# include <mutex>
#elif !defined(WIN32) && !defined(_WIN32) // use pthread mutex
# include <pthread.h>
#else // use Windows critical section
# ifndef SC_INCLUDE_WINDOWS_H
#   define SC_INCLUDE_WINDOWS_H // include Windows.h, if needed
#   include "sysc/kernel/sc_cmnhdr.h"
# endif
#endif // SC_CPLUSPLUS

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
# pragma warning(push)
# pragma warning(disable: 4251) // DLL import for std::mutex
#endif

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_host_mutex
//
//   The sc_host_mutex class, wrapping an OS mutex on the simulation host
// ----------------------------------------------------------------------------

class SC_API sc_host_mutex : public sc_mutex_if
{
#if SC_CPLUSPLUS >= 201103L

    typedef std::mutex underlying_type;

    void do_init()    { /* no-op */ }
    void do_lock()    { m_mtx.lock(); }
    bool do_trylock() { return m_mtx.try_lock(); }
    void do_unlock()  { m_mtx.unlock(); }
    void do_destroy() { /* no-op */ }

#elif defined(WIN32) || defined(_WIN32) // use CRITICAL_SECTION on Windows

    typedef CRITICAL_SECTION underlying_type;

    void do_init()    { InitializeCriticalSection( &m_mtx ); }
    void do_lock()    { EnterCriticalSection( &m_mtx ); }
    bool do_trylock() { return ( TryEnterCriticalSection( &m_mtx ) != 0 ); }
    void do_unlock()  { LeaveCriticalSection( &m_mtx ); }
    void do_destroy() { DeleteCriticalSection( &m_mtx ); }

#else // use pthread mutex

# if defined(__hpux)
#   define SC_PTHREAD_NULL_ cma_c_null
# else // !defined(__hpux)
#   define SC_PTHREAD_NULL_ NULL
# endif

    typedef pthread_mutex_t underlying_type;

    void do_init()    { pthread_mutex_init( &m_mtx, SC_PTHREAD_NULL_ ); }
    void do_lock()    { pthread_mutex_lock( &m_mtx ); }
# ifdef _XOPEN_SOURCE
    bool do_trylock() { return ( pthread_mutex_trylock( &m_mtx ) == 0 ); }
# else
    bool do_trylock() { return /* not supported */ false; }
# endif
    void do_unlock()  { pthread_mutex_unlock( &m_mtx ); }
    void do_destroy() { pthread_mutex_destroy( &m_mtx ); }

# undef SC_PTHREAD_NULL_

#endif // platform-specific implementation

public:

    // constructors and destructor

    sc_host_mutex() : m_mtx()
      { do_init(); }
    virtual ~sc_host_mutex()
      { do_destroy(); }


    // interface methods

    // blocks until mutex could be locked
    virtual int lock()
      { do_lock(); return 0; }

    // returns -1 if mutex could not be locked
    virtual int trylock()
      { return do_trylock() ? 0 : -1; }

    // should return -1 if mutex was not locked by caller,
    // but is not yet able to check this
    virtual int unlock()
      { do_unlock(); return 0; }

private:
    underlying_type m_mtx;
};

} // namespace sc_core

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
# pragma warning(pop)
#endif

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
 *****************************************************************************/
//$Log: sc_host_mutex.h,v $
//Revision 1.4  2011/08/30 21:53:23  acg
// Jerome Cornet: include window.h for gnu c in windows case.
//
//Revision 1.3  2011/08/07 19:08:01  acg
// Andy Goodrich: moved logs to end of file so line number synching works
// better between versions.
//
//Revision 1.2  2011/06/25 17:08:38  acg
// Andy Goodrich: Jerome Cornet's changes to use libtool to build the
// library.
//
//Revision 1.1  2011/04/18 19:04:11  acg
// Philipp A. Hartmann: first check in of file.
//

#endif

// Taf!
