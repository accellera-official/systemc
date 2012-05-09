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

#ifndef __TLM_ANALYSIS_FIFO_H__
#define __TLM_ANALYSIS_FIFO_H__

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/tlm_fifo.h"
#include "tlm_core/tlm_1/tlm_analysis/tlm_analysis_if.h"
#include "tlm_core/tlm_1/tlm_analysis/tlm_analysis_triple.h"

namespace tlm {

template< typename T >
class tlm_analysis_fifo :
  public tlm_fifo< T > ,
  public virtual tlm_analysis_if< T > ,
  public virtual tlm_analysis_if< tlm_analysis_triple< T > > {

 public:

 // analysis fifo is an unbounded tlm_fifo

  tlm_analysis_fifo( const char *nm ) : tlm_fifo<T>( nm , -16 ) {}
  tlm_analysis_fifo() : tlm_fifo<T>( -16 ) {}

  void write( const tlm_analysis_triple<T> &t ) {
    nb_put( t );
  }

  void write( const T &t ) {
    nb_put( t );
  }

};

} // namespace tlm

#endif
