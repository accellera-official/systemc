/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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
// To the LRM writer : these classes are purely artifacts of the implementation.
//

#ifndef __TLM_PUT_GET_IMP_H__
#define __TLM_PUT_GET_IMP_H__

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_master_slave_ifs.h"

namespace tlm {

template < typename PUT_DATA , typename GET_DATA>
class tlm_put_get_imp :
  private virtual tlm_put_if< PUT_DATA > ,
  private virtual tlm_get_peek_if< GET_DATA >
{
public:
  tlm_put_get_imp( tlm_put_if<PUT_DATA> &p ,
       tlm_get_peek_if<GET_DATA> &g ) :
    put_fifo( p ) , get_fifo( g ) {}

  // put interface

  void put( const PUT_DATA &t ) { put_fifo.put( t ); }

  bool nb_put( const PUT_DATA &t ) { return put_fifo.nb_put( t ); }
  bool nb_can_put( tlm_tag<PUT_DATA> *t = 0 ) const {
    return put_fifo.nb_can_put( t );
  }
  const sc_core::sc_event &ok_to_put( tlm_tag<PUT_DATA> *t = 0 ) const {
    return put_fifo.ok_to_put( t );
  }

  // get interface

  GET_DATA get( tlm_tag<GET_DATA> * = 0 ) { return get_fifo.get(); }

  bool nb_get( GET_DATA &t ) { return get_fifo.nb_get( t ); }

  bool nb_can_get( tlm_tag<GET_DATA> *t = 0 ) const {
    return get_fifo.nb_can_get( t );
  }

  virtual const sc_core::sc_event &ok_to_get( tlm_tag<GET_DATA> *t = 0 ) const {
    return get_fifo.ok_to_get( t );
  }

  // peek interface

  GET_DATA peek( tlm_tag<GET_DATA> * = 0 ) const { return get_fifo.peek(); }

  bool nb_peek( GET_DATA &t ) const { return get_fifo.nb_peek( t ); }

  bool nb_can_peek( tlm_tag<GET_DATA> *t = 0 ) const {
    return get_fifo.nb_can_peek( t );
  }

  virtual const sc_core::sc_event &ok_to_peek( tlm_tag<GET_DATA> *t = 0 ) const {
    return get_fifo.ok_to_peek( t );
  }

private:
  tlm_put_if<PUT_DATA> &put_fifo;
  tlm_get_peek_if<GET_DATA> &get_fifo;
};

template < typename REQ , typename RSP >
class tlm_master_imp :
  private tlm_put_get_imp< REQ , RSP > ,
  public virtual tlm_master_if< REQ , RSP >
{
public:

  tlm_master_imp( tlm_put_if<REQ> &req ,
                  tlm_get_peek_if<RSP> &rsp ) :
    tlm_put_get_imp<REQ,RSP>( req , rsp ) {}

};

template < typename REQ , typename RSP >
class tlm_slave_imp :
  private tlm_put_get_imp< RSP , REQ > ,
  public virtual tlm_slave_if< REQ , RSP >
{
public:

  tlm_slave_imp( tlm_get_peek_if<REQ> &req ,
                 tlm_put_if<RSP> &rsp ) :
    tlm_put_get_imp<RSP,REQ>( rsp  , req ) {}

};

} // namespace tlm

#endif
