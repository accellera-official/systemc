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

  sc_host_semaphore.h -- A "real" semaphore for the underlying host system

  Original Author: Philipp A. Hartmann, Intel

 CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_HOST_SEMAPHORE_H_INCLUDED_
#define SC_HOST_SEMAPHORE_H_INCLUDED_

#ifndef SC_INCLUDE_WINDOWS_H
#  define SC_INCLUDE_WINDOWS_H // include Windows.h, if needed
#endif
#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/communication/sc_semaphore_if.h"

#if !defined(WIN32) && !defined(_WIN32) // use POSIX semaphore
#include <semaphore.h>
#endif

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_host_semaphore
//
//   Wrapping an OS semaphore on the simulation host
// ----------------------------------------------------------------------------

class SC_API sc_host_semaphore : public sc_semaphore_if
{
#if defined(WIN32) || defined(_WIN32) // use Windows Semaphore

    typedef HANDLE underlying_type;

    void do_init(int init)
    {
      m_sem = CreateSemaphore( NULL, init, LONG_MAX, NULL );
      sc_assert( m_sem != NULL );
    }
    void do_wait()
      { WaitForSingleObject( m_sem, INFINITE ); }
    bool do_trywait()
      { return ( WaitForSingleObject( m_sem, 0 ) == WAIT_OBJECT_0 ); }
    void do_post()
      { ReleaseSemaphore( m_sem, 1, NULL ); }
    void do_destroy()
      { CloseHandle( m_sem ); }

#else // use POSIX semaphore

    typedef sem_t underlying_type;

    void do_init(int init) { sem_init( &m_sem, 0, init ); }
    void do_wait()         { sem_wait( &m_sem ); }
    bool do_trywait()      { return ( sem_trywait( &m_sem ) == 0 ); }
    void do_post()         { sem_post( &m_sem ); }
    void do_destroy()      { sem_close( &m_sem ); }

#endif // platform-specific implementation

public:

    // constructors and destructor

    explicit sc_host_semaphore(int init = 0) : m_sem()
      { do_init(init); }
    virtual ~sc_host_semaphore()
      { do_destroy(); }

    // interface methods

    // lock (take) the semaphore, block if not available
    virtual int wait()
      { do_wait(); return 0; }

    // lock (take) the semaphore, return -1 if not available
    virtual int trywait()
      { return do_trywait() ? 0 : -1; }

    // unlock (give) the semaphore
    virtual int post()
      { do_post(); return 0; }

    // get the value of the semaphore (not supported)
    virtual int get_value() const
      { sc_assert(false && "not supported on sc_host_semaphore"); return -1; }

private:
    underlying_type m_sem;
};

} // namespace sc_core

#endif // SC_HOST_SEMAPHORE_H_INCLUDED_
// Taf!
