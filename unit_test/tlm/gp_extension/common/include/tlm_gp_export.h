/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef __TLM_GP_EXPORT_H__
#define __TLM_GP_EXPORT_H__


#include "tlm.h"
#include "tlm_nb_transport_if.h"

namespace tlm {

template< unsigned int busdatawidth > // in bits
class tlm_gp_export
    : public sc_core::sc_export< tlm::nb_transport_if<tlm::tlm_generic_payload> >
{
    typedef tlm::nb_transport_if<tlm::tlm_generic_payload> interfaceType;
    
public:
    
    // Constructor
    tlm_gp_export(const char* name_ = 0) 
        : sc_core::sc_export< interfaceType >(name_)
        , m_busdatawidth(busdatawidth)
    {
    }
    
    // Destructor
    ~tlm_gp_export() {}
    
    // binding operators
    void bind(tlm_gp_export< busdatawidth >& ex)
    {
        sc_core::sc_export< interfaceType >::bind(ex);
    }
    void bind(interfaceType& if_)
    {
        sc_core::sc_export< interfaceType >::bind(if_);
    }
    
    void operator() (tlm_gp_export< busdatawidth >& ex) { bind(ex); }
    void operator() ( interfaceType& _if ) { bind(_if); }
    
    // Convenient function
    unsigned int getBusDataWidth() {return m_busdatawidth;}
    
private:
    
    unsigned int m_busdatawidth; // in bits
};

}

#endif
