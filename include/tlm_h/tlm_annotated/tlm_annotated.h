#pragma once

#ifndef __TLM_ANNOTATED_H__
#define __TLM_ANNOTATED_H__

//#include <systemc>

#include <map>
#include <deque>

namespace tlm_annotated  /// TLM annotated header sub group
{

#include "tlm_annotated_ifs/tlm_annotated_ifs.h"
#include "tlm_annotated_ifs/tlm_annotated_master_slave_ifs.h"

#include "tlm_annotated_channels/tlm_peq.h"
#include "tlm_annotated_channels/tlm_annotated_fifo.h"
#include "tlm_annotated_channels/tlm_annotated_req_rsp.h"

#include "tlm_fw_bw_ifs/tlm_fw_bw_ifs.h"

}

#endif /* TLM_ANNOTATED_HEADER */
