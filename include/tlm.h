#pragma once

#ifndef TLM_HEADER
#define TLM_HEADER

#include "systemc.h"  /// main SystemC header
#include <deque>      /// STD deque class
#include <map>        /// STD map class

#include "tlm_core/tlm_core.h"
#include "tlm_analysis/tlm_analysis.h"
#include "tlm_annotated/tlm_annotated.h"
#include "tlm_bus/tlm_bus.h"
//#include "tlm_analysis/tlm_bus_analysis.h"
#include "tlm_ports/tlm_ports.h"
#include "tlm_quantum/tlm_quantum.h"

namespace tlm  /// TLM namespace
{

  using namespace tlm_core;
  using namespace analysis;
  using namespace tlm_annotated;
  using namespace tlm_bus;
  using namespace tlm_ports;
  using namespace tlm_quantum;

}

#endif /* TLM_HEADER */
