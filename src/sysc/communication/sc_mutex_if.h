/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_mutex_if.h -- The sc_mutex_if interface class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_MUTEX_IF_H
#define SC_MUTEX_IF_H

#include "sysc/communication/sc_interface.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_mutex_if
//
//  The sc_mutex_if interface class.
// ----------------------------------------------------------------------------

class sc_mutex_if
: virtual public sc_interface
{
public:

    // the classical operations: lock(), trylock(), and unlock()

    // blocks until mutex could be locked
    virtual int lock() = 0;

    // returns -1 if mutex could not be locked
    virtual int trylock() = 0;

    // returns -1 if mutex was not locked by caller
    virtual int unlock() = 0;

protected:

    // constructor

    sc_mutex_if()
	{}

private:

    // disabled
    sc_mutex_if( const sc_mutex_if& );
    sc_mutex_if& operator = ( const sc_mutex_if& );
};

} // namespace sc_core

#endif

// Taf!
