/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_semaphore.h -- The sc_semaphore primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_SEMAPHORE_H
#define SC_SEMAPHORE_H


#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/communication/sc_semaphore_if.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_semaphore
//
//  The sc_semaphore primitive channel class.
// ----------------------------------------------------------------------------

class sc_semaphore
: public sc_semaphore_if,
  public sc_object
{
public:

    // constructors

    explicit sc_semaphore( int init_value_ );
    sc_semaphore( const char* name_, int init_value_ );


    // interface methods

    // lock (take) the semaphore, block if not available
    virtual int wait();

    // lock (take) the semaphore, return -1 if not available
    virtual int trywait();

    // unlock (give) the semaphore
    virtual int post();

    // get the value of the semaphore
    virtual int get_value() const
	{ return m_value; }

    virtual const char* kind() const
        { return "sc_semaphore"; }

protected:

    // support methods

    bool in_use() const
	{ return ( m_value <= 0 ); }


    // error reporting
    void report_error( const char* id, const char* add_msg = 0 ) const;

protected:

    sc_event m_free;        // event to block on when m_value is negative
    int      m_value;       // current value of the semaphore

private:

    // disabled
    sc_semaphore( const sc_semaphore& );
    sc_semaphore& operator = ( const sc_semaphore& );
};

} // namespace sc_core

//$Log: sc_semaphore.h,v $
//Revision 1.4  2011/08/26 20:45:42  acg
// Andy Goodrich: moved the modification log to the end of the file to
// eliminate source line number skew when check-ins are done.
//
//Revision 1.3  2011/02/18 20:23:45  acg
// Andy Goodrich: Copyright update.
//
//Revision 1.2  2010/11/02 16:31:01  acg
// Andy Goodrich: changed object derivation to use sc_object rather than
// sc_prim_channel as the parent class.
//
//Revision 1.1.1.1  2006/12/15 20:20:04  acg
//SystemC 2.3
//
//Revision 1.4  2006/11/28 20:30:49  acg
// Andy Goodrich: updated from 2.2 source. sc_event_queue constructors
// collapsed into a single constructor with an optional argument to get
// the sc_module_name stack done correctly. Class name prefixing added
// to sc_semaphore calls to wait() to keep gcc 4.x happy.
//
//Revision 1.2  2006/01/03 23:18:26  acg
//Changed copyright to include 2006.
//
//Revision 1.1.1.1  2005/12/19 23:16:43  acg
//First check in of SystemC 2.1 into its own archive.
//
//Revision 1.9  2005/06/10 22:43:55  acg
//Added CVS change log annotation.
//

#endif

// Taf!
