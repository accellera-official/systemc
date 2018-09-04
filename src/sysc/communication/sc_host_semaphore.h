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

#include "sysc/kernel/sc_cmnhdr.h" // SC_CPLUSPLUS
#include "sysc/communication/sc_semaphore_if.h"

#if SC_CPLUSPLUS >= 201103L
# include <mutex>
# include <condition_variable>
#elif !defined(WIN32) && !defined(_WIN32) // use POSIX semaphore
# include <semaphore.h>
#else // use Windows semaphore
# ifndef SC_INCLUDE_WINDOWS_H
#   define SC_INCLUDE_WINDOWS_H // include Windows.h, if needed
#   include "sysc/kernel/sc_cmnhdr.h"
# endif
#endif // SC_CPLUSPLUS

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_host_semaphore
//
//   Wrapping an OS semaphore on the simulation host
// ----------------------------------------------------------------------------

class SC_API sc_host_semaphore : public sc_semaphore_if
{
#if SC_CPLUSPLUS >= 201103L

    struct underlying_type
    {
      std::mutex mtx;
      std::condition_variable cond_var;
      int value;
    };

    void do_init(int init) { m_sem.value = init; }
    void do_wait()
    {
      std::unique_lock<std::mutex> lock(m_sem.mtx);
      while (m_sem.value <= 0) {
        m_sem.cond_var.wait(lock);
      }
      --m_sem.value;
    }
    bool do_trywait()
    {
      std::unique_lock<std::mutex> lock(m_sem.mtx);
      if (m_sem.value <= 0)
        return false;
      --m_sem.value;
      return true;
    }
    void do_post()
    {
      std::unique_lock<std::mutex> lock(m_sem.mtx);
      ++m_sem.value;
      m_sem.cond_var.notify_one();
    }
    void do_destroy() { /* no-op */ }

#elif defined(WIN32) || defined(_WIN32) // use Windows Semaphore

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

    void do_init(int init)
    {
      int semaphore_initialized = sem_init( &m_sem, 0, init );
      sc_assert(semaphore_initialized == 0);
    }
    void do_wait()         { sem_wait( &m_sem ); }
    bool do_trywait()      { return ( sem_trywait( &m_sem ) == 0 ); }
    void do_post()         { sem_post( &m_sem ); }
    void do_destroy()      { sem_destroy( &m_sem ); }

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
