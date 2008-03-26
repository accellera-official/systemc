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

  sc_wait_cthread.h -- Wait() and related functions for SC_CTHREADs.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/******************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

******************************************************************************/

/* 
$Log: sc_wait_cthread.h,v $
Revision 1.6  2005/01/10 17:52:20  acg
Addition of namespace specifications.

Revision 1.5  2004/09/27 20:49:10  acg
Andy Goodrich, Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments appear in the
     checkout source.

*/

#ifndef SC_WAIT_CTHREAD_H
#define SC_WAIT_CTHREAD_H


#include "sysc/kernel/sc_lambda.h"
#include "sysc/kernel/sc_simcontext.h"

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
wait_until( const sc_lambda_ptr&,
	    sc_simcontext* = sc_get_curr_simcontext() );

inline
void
wait_until( const sc_signal_bool_deval& s,
	    sc_simcontext* simc = sc_get_curr_simcontext() )
{
    wait_until( sc_lambda_ptr( s ), simc );
}


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



inline
void
watching( const sc_lambda_ptr& lambda,
	  sc_simcontext* simc = sc_get_curr_simcontext() )
{
    (*simc->m_watching_fn)( lambda, simc );
}

inline
void
watching( const sc_signal_bool_deval& s,
	  sc_simcontext* simc = sc_get_curr_simcontext() )
{
    (*simc->m_watching_fn)( sc_lambda_ptr( s ), simc );
}


extern
void
__open_watching( sc_cthread_handle );

extern
void
__close_watching( sc_cthread_handle );

extern
int
__watch_level( sc_cthread_handle );

extern
void
__watching_first( sc_cthread_handle );

extern
void
__sanitycheck_watchlists( sc_cthread_handle );


class sc_watch
{
public:

    sc_cthread_handle cthread_h;

    sc_watch( sc_simcontext* simc )
    {
        sc_curr_proc_handle cpi = simc->get_curr_proc_info();
        assert( SC_CTHREAD_PROC_ == cpi->kind );
        cthread_h = RCAST<sc_cthread_handle>( cpi->process_handle );
        __open_watching( cthread_h );
    }

    ~sc_watch()
    {
        __close_watching( cthread_h );
    }
};

} // namespace sc_core

#endif
