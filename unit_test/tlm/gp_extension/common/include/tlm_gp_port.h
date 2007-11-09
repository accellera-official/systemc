/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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


#ifndef __TLM_GP_PORT_H__
#define __TLM_GP_PORT_H__


#include "systemc"
#include "tlm_nb_transport_if.h"
#include "tlm.h"

namespace tlm {

template< unsigned int busdatawidth > // in bits
class tlm_gp_port
    : virtual public sc_core::sc_port< tlm::nb_transport_if<tlm::tlm_generic_payload> >
{
    typedef tlm::nb_transport_if<tlm::tlm_generic_payload> interfaceType;

public:
    
    // Constructor
    tlm_gp_port(const char* name_ = 0) 
        : sc_core::sc_port< interfaceType >(name_)
        , m_busdatawidth(busdatawidth)
    {
    }
    
    // Destructor
    ~tlm_gp_port() {}
    
    // Binding
    void operator () ( interfaceType& exp )
    {
        sc_core::sc_port< interfaceType >::operator() ( exp );
    }
    void operator () ( tlm_gp_port< busdatawidth >& p )
    {
        sc_core::sc_port< interfaceType >::operator() ( p );
    }
    
    // Convenient function
    unsigned int getBusDataWidth() {return m_busdatawidth;}
    
private:
    
    unsigned int m_busdatawidth; // in bits

};

}

#endif
