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

#include <mutex>
#include <condition_variable>

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(disable: 4251) // DLL import for std::mutex
#endif

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_host_semaphore
//
//   Wrapping an OS semaphore on the simulation host
// ----------------------------------------------------------------------------

class SC_API sc_host_semaphore : public sc_semaphore_if
{
public:

    // constructors and destructor

    explicit sc_host_semaphore(int init = 0) : m_value(init) {}
    virtual ~sc_host_semaphore() = default;

    // interface methods

    // lock (take) the semaphore, block if not available
    virtual int wait()
    {
      std::unique_lock lock(m_mtx);
      while (m_value <= 0) {
        m_cond_var.wait(lock);
      }
      --m_value;
      return 0;
    }

    // lock (take) the semaphore, return -1 if not available
    virtual int trywait()
    {
      std::unique_lock lock(m_mtx);
      if (m_value <= 0)
        return -1;
      --m_value;
      return 0;
    }

    // unlock (give) the semaphore
    virtual int post()
    {
      std::unique_lock lock(m_mtx);
      ++m_value;
      m_cond_var.notify_one();
      return 0;
    }

    // get the value of the semaphore
    virtual int get_value() const
    {
      return m_value;
    }

private:
    std::mutex m_mtx;
    std::condition_variable m_cond_var;
    int m_value = 0;
};

} // namespace sc_core

#endif // SC_HOST_SEMAPHORE_H_INCLUDED_
// Taf!
