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

#if defined(WIN32) || defined(_WIN32)

#define SC_SEM_TYPE_ HANDLE

#define SC_SEM_INIT_( Semaphore, InitValue ) \
    (Semaphore) = CreateSemaphore( NULL, InitValue, LONG_MAX, NULL ); \
    sc_assert( (Semaphore) != NULL )
#define SC_SEM_WAIT_( Semaphore ) \
    WaitForSingleObject( (Semaphore), INFINITE )
#define SC_SEM_TRYWAIT_( Semaphore ) \
    ( WaitForSingleObject( (Semaphore), 0 ) == WAIT_OBJECT_0 )
#define SC_SEM_POST_( Semaphore ) \
    ReleaseSemaphore( (Semaphore), 1, NULL )
#define SC_SEM_DESTROY_( Semaphore ) \
    CloseHandle( (Semaphore) )

#else // use POSIX semaphore

#include <semaphore.h>
#define SC_SEM_TYPE_ sem_t

#define SC_SEM_INIT_( Semaphore, InitValue ) \
    sem_init( &(Semaphore), 0, InitValue )
#define SC_SEM_WAIT_( Semaphore ) \
    sem_wait( &(Semaphore) )
#define SC_SEM_TRYWAIT_( Semaphore ) \
    ( sem_trywait( &(Semaphore) ) == 0 )
#define SC_SEM_POST_( Semaphore ) \
    sem_post( &(Semaphore) )
#define SC_SEM_DESTROY_( Semaphore ) \
    sem_close( &(Semaphore) )

#endif

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_host_semaphore
//
//   Wrapping an OS semaphore on the simulation host
// ----------------------------------------------------------------------------

class SC_API sc_host_semaphore : public sc_semaphore_if
{
    typedef SC_SEM_TYPE_ underlying_type;
public:

    // constructors and destructor

    explicit sc_host_semaphore(int init = 0)
      { SC_SEM_INIT_(m_sem, init); }
    virtual ~sc_host_semaphore()
      { SC_SEM_DESTROY_(m_sem); }

    // interface methods

    // lock (take) the semaphore, block if not available
    virtual int wait()
      { SC_SEM_WAIT_(m_sem); return 0; }

    // lock (take) the semaphore, return -1 if not available
    virtual int trywait()
      { return SC_SEM_TRYWAIT_(m_sem) ? 0 : -1; }

    // unlock (give) the semaphore
    virtual int post()
      { SC_SEM_POST_(m_sem); return 0; }

    // get the value of the semaphore (not supported)
    virtual int get_value() const
      { sc_assert(false && "not supported on sc_host_semaphore"); return -1; }

private:
    underlying_type m_sem;
};

} // namespace sc_core

#undef SC_SEM_TYPE_
#undef SC_SEM_INIT_
#undef SC_SEM_DESTROY_
#undef SC_SEM_WAIT_
#undef SC_SEM_TRYWAIT_
#undef SC_SEM_POST_

#endif // SC_HOST_SEMAPHORE_H_INCLUDED_
// Taf!
