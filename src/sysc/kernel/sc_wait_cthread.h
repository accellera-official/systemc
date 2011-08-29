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

  sc_wait_cthread.h -- Wait() and related functions for SC_CTHREADs.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_WAIT_CTHREAD_H
#define SC_WAIT_CTHREAD_H


#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/communication/sc_signal_ifs.h"


namespace sc_core 
{

// for SC_CTHREADs

extern
void
halt( sc_simcontext* = sc_get_curr_simcontext() );


extern
void
wait( int,
      sc_simcontext* = sc_get_curr_simcontext() );


extern
void
at_posedge( const sc_signal_in_if<bool>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_posedge( const sc_signal_in_if<sc_dt::sc_logic>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_negedge( const sc_signal_in_if<bool>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_negedge( const sc_signal_in_if<sc_dt::sc_logic>&,
	    sc_simcontext* = sc_get_curr_simcontext() );



class sc_watch
{
public:

    sc_cthread_handle cthread_h;

    sc_watch( sc_simcontext* simc ) : cthread_h()
    {
        sc_curr_proc_handle cpi = simc->get_curr_proc_info();
        assert( SC_CTHREAD_PROC_ == cpi->kind );
        cthread_h = RCAST<sc_cthread_handle>( cpi->process_handle );
    }

    ~sc_watch()
    {
    }
};

} // namespace sc_core

/* 
$Log: sc_wait_cthread.h,v $
Revision 1.6  2011/08/26 20:46:11  acg
 Andy Goodrich: moved the modification log to the end of the file to
 eliminate source line number skew when check-ins are done.

Revision 1.5  2011/08/24 22:05:51  acg
 Torsten Maehne: initialization changes to remove warnings.

Revision 1.4  2011/02/18 20:27:14  acg
 Andy Goodrich: Updated Copyrights.

Revision 1.3  2011/02/13 21:47:38  acg
 Andy Goodrich: update copyright notice.

Revision 1.2  2008/05/22 17:06:27  acg
 Andy Goodrich: updated copyright notice to include 2008.

Revision 1.1.1.1  2006/12/15 20:20:05  acg
SystemC 2.3

Revision 1.2  2006/01/03 23:18:45  acg
Changed copyright to include 2006.

Revision 1.1.1.1  2005/12/19 23:16:44  acg
First check in of SystemC 2.1 into its own archive.

Revision 1.10  2005/09/02 19:03:30  acg
Changes for dynamic processes. Removal of lambda support.

Revision 1.9  2005/04/04 00:16:08  acg
Changes for directory name change to sys from systemc.
Changes for sc_string going to std::string.
Changes for sc_pvector going to std::vector.
Changes for reference pools for bit and part selections.
Changes for const sc_concatref support.

Revision 1.6  2005/01/10 17:52:20  acg
Addition of namespace specifications.

Revision 1.5  2004/09/27 20:49:10  acg
Andy Goodrich, Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments appear in the
     checkout source.

*/

#endif
