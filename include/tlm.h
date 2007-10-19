
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
 
#pragma once

#ifndef __TLM_H__
#define __TLM_H__

#include <systemc>    /// main SystemC header
#include <deque>      /// STD deque class
#include <map>        /// STD map class

#include "tlm_h/tlm_core/tlm_core.h"
#include "tlm_h/tlm_analysis/tlm_analysis.h"
#include "tlm_h/tlm_annotated/tlm_annotated.h"
#include "tlm_h/tlm_generic_payload/tlm_generic_payload.h"
//#include "tlm_h/tlm_analysis/tlm_bus_analysis.h"
#include "tlm_h/tlm_ports/tlm_ports.h"
#include "tlm_h/tlm_quantum/tlm_quantum.h"

namespace tlm  /// TLM namespace
{
  using namespace tlm_core;
  using namespace analysis;
  using namespace tlm_annotated;
  using namespace tlm_generic_payload_ns;
  using namespace tlm_ports;
  using namespace tlm_quantum;
}

#endif /* __TLM_H__ */
