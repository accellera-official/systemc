/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef __TLM_ANNOTATED_IFS_H__
#define __TLM_ANNOTATED_IFS_H__

namespace tlm {

template< typename REQ , typename RSP >
class tlm_delayed_transport_if : public virtual sc_core::sc_interface {
 public:
  virtual void transport( const REQ &req , RSP &rsp , sc_core::sc_time &t ) = 0;
};

template <  typename REQ , typename RSP >
class  tlm_annotated_transport_if :
  public virtual tlm_transport_if< REQ , RSP > ,
  public virtual tlm_delayed_transport_if< REQ , RSP >
{
public:
  using tlm_transport_if<REQ, RSP>::transport;
  using tlm_delayed_transport_if<REQ, RSP>::transport;
};



template< typename T >
class tlm_annotated_nonblocking_get_if :
  public virtual tlm_nonblocking_get_if< T >
{
 public:
  tlm_nonblocking_get_if< T >::nb_get;
  tlm_nonblocking_get_if< T >::nb_can_get;

  virtual bool nb_get( T & , const sc_core::sc_time & ) = 0;
  virtual bool nb_can_get( const sc_core::sc_time & , tlm_tag<T> *t = 0 ) const = 0;
};



template < typename T >
class tlm_annotated_nonblocking_put_if :
  public virtual tlm_nonblocking_put_if < T >
{
 public:
  tlm_nonblocking_put_if< T >::nb_put;
  tlm_nonblocking_put_if< T >::nb_can_put;

  virtual bool nb_put( const T & , const sc_core::sc_time & ) = 0;
  virtual bool nb_can_put( const sc_core::sc_time & , tlm_tag<T> *t = 0 ) const = 0;
};

template < typename T >
class tlm_annotated_get_if :
  public virtual tlm_blocking_get_if< T > ,
  public virtual tlm_annotated_nonblocking_get_if< T > ,
  public virtual tlm_get_if< T > {};

template < typename T >
class tlm_annotated_put_if :
  public virtual tlm_blocking_put_if< T > ,
  public virtual tlm_annotated_nonblocking_put_if< T > ,
  public virtual tlm_put_if<T>{};

template < typename T >
class tlm_annotated_nonblocking_get_peek_if :
  public virtual tlm_annotated_nonblocking_get_if<T> ,
  public virtual tlm_nonblocking_peek_if< T > ,
  public virtual tlm_nonblocking_get_peek_if< T > {};

template < typename T >
class tlm_annotated_get_peek_if :
  public virtual tlm_annotated_get_if< T > ,
  public virtual tlm_peek_if<T> ,
  public virtual tlm_blocking_get_peek_if<T> ,
  public virtual tlm_annotated_nonblocking_get_peek_if< T > ,
  public virtual tlm_get_peek_if< T >
  {};

} // namespace tlm

#endif
