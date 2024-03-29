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

#include <mutex>

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
public:

    // constructors and destructor

    sc_host_mutex() = default;
    virtual ~sc_host_mutex() = default;

    // interface methods

    // blocks until mutex could be locked
    virtual int lock()
      { m_mtx.lock(); return 0; }

    // returns -1 if mutex could not be locked
    virtual int trylock()
      { return m_mtx.try_lock() ? 0 : -1; }

    // should return -1 if mutex was not locked by caller,
    // but is not yet able to check this
    virtual int unlock()
      { m_mtx.unlock(); return 0; }

private:
    std::mutex m_mtx;
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
