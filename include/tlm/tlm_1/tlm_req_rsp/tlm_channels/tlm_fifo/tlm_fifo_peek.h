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

#ifndef __TLM_FIFO_PEEK_H__
#define __TLM_FIFO_PEEK_H__

namespace tlm {

template < typename T>
inline
T
tlm_fifo<T>::peek( tlm_tag<T> * ) const {

  while( is_empty() ) {

    // call free-standing sc_core::wait(),
    // since sc_prim_channel::wait(.) is not const

    sc_core::wait( m_data_written_event );
  }

  return buffer.read_data();

}

template < typename T>
inline
bool
tlm_fifo<T>::nb_peek( T &t ) const {

  if( used() < 1 ) {
    return false;
  }

  t = buffer.peek_data( 0 );
  return true;

}

template < typename T>
inline
bool
tlm_fifo<T>::nb_peek( T &t , int n ) const {

  if( n >= used() || n < -1 ) {
    return false;
  }

  if( n == -1 ) {
    n = used() - 1;
  }

  t = buffer.peek_data( n );
  return true;

}

template< typename T >
inline
bool
tlm_fifo<T>::nb_can_peek( tlm_tag<T> * ) const
{
  return !is_empty();
}

template < typename T>
inline
bool
tlm_fifo<T>::nb_poke( const T &t , int n ) {

  if( n >= used() || n < 0 ) {
    return false;
  }

  buffer.poke_data( n ) = t;
  return true;

}

} // namespace tlm

#endif
