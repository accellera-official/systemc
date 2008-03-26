/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_event_queue.cpp -- Event Queue Support

  Original Author: Stuart Swan, Cadence Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_event_queue.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.4  2006/01/26 21:00:50  acg
//  Andy Goodrich: conversion to use sc_event::notify(SC_ZERO_TIME) instead of
//  sc_event::notify_delayed()
//
// Revision 1.3  2006/01/13 18:47:42  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_event_queue.h"
#include "sysc/kernel/sc_method_process.h"

namespace sc_core {

static int 
sc_time_compare( const void* p1, const void* p2 )
{
    const sc_time* t1 = static_cast<const sc_time*>( p1 );
    const sc_time* t2 = static_cast<const sc_time*>( p2 );

    if( *t1 < *t2 ) {
	return 1;
    } else if( *t1 > *t2 ) {
	return -1;
    } else {
	return 0;
    }  
}

sc_event_queue::sc_event_queue( sc_module_name name_ )
    : sc_module( name_ ),
      m_ppq( 128, sc_time_compare ),
      m_pending_delta(0)
{
    m_delta=0;
    SC_METHOD( fire_event );
    sensitive << m_e;
    dont_initialize();
}

sc_event_queue::~sc_event_queue()
{
  while (m_ppq.size() > 0) {
    delete m_ppq.extract_top();
  }
}

void sc_event_queue::cancel_all()
{
    m_pending_delta = 0;
    while( m_ppq.size() > 0 )
	m_ppq.extract_top();
    m_e.cancel();
}

void sc_event_queue::notify (const sc_time& when)
{
    m_delta = sc_delta_count();
    sc_time* t = new sc_time( when+sc_time_stamp() );
    if ( m_ppq.size()==0 || *t < *m_ppq.top() ) {
	m_e.notify( when );
    }
    m_ppq.insert( t );
}
    
void sc_event_queue::fire_event()
{
    sc_time* t = m_ppq.extract_top();
    assert( *t==sc_time_stamp() );
    delete t;

    if ( m_ppq.size() > 0 ) {
	m_e.notify( *m_ppq.top() - sc_time_stamp() );
    }
}

} // namespace sc_core
