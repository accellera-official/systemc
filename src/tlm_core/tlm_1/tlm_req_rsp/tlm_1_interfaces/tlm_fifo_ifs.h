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

//
// Note to the LRM writer : These interfaces are channel specific interfaces
// useful in the context of tlm_fifo.
//

#ifndef __TLM_FIFO_IFS_H__
#define __TLM_FIFO_IFS_H__

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_core_ifs.h"

namespace tlm {

//
// Fifo specific interfaces
//

// Fifo Debug Interface

template< typename T >
class tlm_fifo_debug_if : public virtual sc_core::sc_interface
{
public:
  virtual int used() const = 0;
  virtual int size() const = 0;
  virtual void debug() const = 0;

  //
  // non blocking peek and poke - no notification
  //
  // n is index of data :
  // 0 <= n < size(), where 0 is most recently written, and size() - 1
  // is oldest ie the one about to be read.
  //

  virtual bool nb_peek( T & , int n ) const = 0;
  virtual bool nb_poke( const T & , int n = 0 ) = 0;

};

// fifo interfaces = extended + debug

template < typename T >
class tlm_fifo_put_if :
  public virtual tlm_put_if<T> ,
  public virtual tlm_fifo_debug_if<T> {};

template < typename T >
class tlm_fifo_get_if :
  public virtual tlm_get_peek_if<T> ,
  public virtual tlm_fifo_debug_if<T> {};

class tlm_fifo_config_size_if : public virtual sc_core::sc_interface
{
public:
  virtual void nb_expand( unsigned int n = 1 ) = 0;
  virtual void nb_unbound( unsigned int n = 16 ) = 0;

  virtual bool nb_reduce( unsigned int n = 1 ) = 0;
  virtual bool nb_bound( unsigned int n ) = 0;

};

} // namespace tlm

#endif

