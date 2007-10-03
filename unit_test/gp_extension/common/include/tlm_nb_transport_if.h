//////////////////////////////////////////////////////////////////
// Copyright (C) 2007 Synopsys, Inc.
// All rights reserved.
//
// Author: Olaf Scheufen
//////////////////////////////////////////////////////////////////

#ifndef __tlm_nb_transport_if_h
#define __tlm_nb_transport_if_h

#include <systemc>

namespace tlm {

template<typename REQ>
class nb_transport_if
  : virtual public sc_core::sc_interface
{
public:

    virtual void nb_transport(REQ* r) = 0;

    virtual unsigned int transport_dbg(REQ* r) = 0;

};

}
#endif
