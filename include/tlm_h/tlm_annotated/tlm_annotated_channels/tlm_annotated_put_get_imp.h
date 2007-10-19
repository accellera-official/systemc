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


//
// To the LRM writer : these classes are purely artifacts of the implementation.
//

#ifndef __TLM_ANNOTATED_PUT_GET_IMP_H__
#define __TLM_ANNOTATED_PUT_GET_IMP_H__

//#include "tlm_core.h"
#include "tlm_h/tlm_annotated/tlm_annotated_ifs/tlm_annotated_master_slave_ifs.h"

// TODO: delete this:
using tlm_core::tlm_put_get_imp;

template< typename PUT_DATA , typename GET_DATA >
class tlm_annotated_put_get_imp :
  private virtual tlm_annotated_put_if< PUT_DATA > ,
  private virtual tlm_annotated_get_peek_if< GET_DATA > ,
  private tlm_put_get_imp< PUT_DATA , GET_DATA > {
public:

  tlm_annotated_put_get_imp( tlm_annotated_put_if <PUT_DATA > &put_if ,
			     tlm_annotated_get_peek_if< GET_DATA > &get_peek_if ) :
    tlm_put_get_imp<PUT_DATA,GET_DATA>( put_if , get_peek_if ) ,
    delayed_put_if( put_if ) , delayed_get_if( get_peek_if ) {}

  bool nb_put( const PUT_DATA &t , const sc_core::sc_time &time ) {
    return delayed_put_if.nb_put( t , time );
  }

  bool nb_can_put( const sc_core::sc_time &time , tlm_tag<PUT_DATA> *t = 0 ) const {
    return delayed_put_if.nb_can_put( time , t );
  }

  bool nb_get( GET_DATA &t , const sc_core::sc_time &time ) {
    return delayed_get_if.nb_get( t , time );
  }

  bool nb_can_get( const sc_core::sc_time &time , tlm_tag<GET_DATA> *t = 0 ) const {
    return delayed_get_if.nb_can_get( time , t );
  }


private:
  tlm_annotated_nonblocking_put_if< PUT_DATA > &delayed_put_if;
  tlm_annotated_nonblocking_get_if<GET_DATA> &delayed_get_if;

};

template < typename REQ , typename RSP >
class tlm_annotated_master_imp :
  private tlm_annotated_put_get_imp< REQ , RSP > ,
  public virtual tlm_annotated_master_if< REQ , RSP >
{
public:

  tlm_annotated_master_imp( tlm_annotated_put_if<REQ> &req ,
			    tlm_annotated_get_peek_if<RSP> &rsp ) :
    tlm_annotated_put_get_imp<REQ,RSP>( req , rsp ) {}

};

template < typename REQ , typename RSP >
class tlm_annotated_slave_imp :
  private tlm_annotated_put_get_imp< RSP , REQ > ,
  public virtual tlm_annotated_slave_if< REQ , RSP >
{
public:

  tlm_annotated_slave_imp( tlm_annotated_get_peek_if<REQ> &req ,
			   tlm_annotated_put_if<RSP> &rsp ) :
    tlm_annotated_put_get_imp<RSP,REQ>( rsp , req ) {}

};

#endif
