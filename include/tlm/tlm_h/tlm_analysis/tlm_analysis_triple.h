/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#ifndef __TLM_ANALYSIS_TRIPLE_H__
#define __TLM_ANALYSIS_TRIPLE_H__

//#include <systemc>

namespace tlm {

template< typename T>
struct tlm_analysis_triple {

  sc_core::sc_time start_time;
  T transaction;
  sc_core::sc_time end_time;

  tlm_analysis_triple() {}

  tlm_analysis_triple( const tlm_analysis_triple &triple ) {
    start_time = triple.start_time;
    transaction = triple.transaction;
    end_time = triple.end_time;
  }

  tlm_analysis_triple( const T &t ) {
    transaction = t;
  }

  operator T() { return transaction; }
  operator const T &() const { return transaction; }

};

} // namespace tlm

#endif
