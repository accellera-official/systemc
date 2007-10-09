#pragma once

#ifndef TLM_ANNOTATED_HEADER
#define TLM_ANNOTATED_HEADER

//#include <systemc.h>

#include <map>
#include <deque>

namespace tlm_annotated  /// TLM annotated header sub group
{

#include "tlm_annotated_ifs/tlm_annotated_ifs.h"
#include "tlm_annotated_ifs/tlm_annotated_master_slave_ifs.h"
#include "tlm_annotated_ifs/tlm_nonblocking_transport_if.h"

#include "tlm_annotated_channels/tlm_peq.h"
#include "tlm_annotated_channels/tlm_annotated_fifo.h"
#include "tlm_annotated_channels/tlm_annotated_req_rsp.h"

}

#endif /* TLM_ANNOTATED_HEADER */
