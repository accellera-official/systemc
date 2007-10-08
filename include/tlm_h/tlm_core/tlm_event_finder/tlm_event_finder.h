
/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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

#ifndef TLM_EVENT_FINDER
#define TLM_EVENT_FINDER

#include <systemc.h>

#include "tlm_core/tlm_interfaces/tlm_tag.h"


template <class IF , class T>
class tlm_event_finder_t
: public sc_event_finder
{
public:

    // constructor

    tlm_event_finder_t( const sc_port_base& port_,
		       const sc_event& (IF::*event_method_) ( tlm_tag<T> * ) const )
        : sc_event_finder( port_ ), m_event_method( event_method_ )
        {}

    // destructor (does nothing)

    virtual ~tlm_event_finder_t()
        {}

    virtual const sc_event& find_event() const;

private:

    const sc_event& (IF::*m_event_method) ( tlm_tag<T> * ) const;

private:

    // disabled
    tlm_event_finder_t();
    tlm_event_finder_t( const tlm_event_finder_t<IF,T>& );
    tlm_event_finder_t<IF,T>& operator = ( const tlm_event_finder_t<IF,T>& );
};


template <class IF , class T>
inline
const sc_event&
tlm_event_finder_t<IF,T>::find_event() const
{
    const IF* iface = DCAST<const IF*>( port().get_interface() );
    if( iface == 0 ) {
	report_error( SC_ID_FIND_EVENT_, "port is not bound" );
    }
    return (CCAST<IF*>( iface )->*m_event_method) ( 0 );
}

#endif
