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

#ifndef __TLM_FIFO_RESIZE_H__
#define __TLM_FIFO_RESIZE_H__

/******************************************************************
//
// resize interface
//
******************************************************************/

namespace tlm {

template < typename T>
inline
void
tlm_fifo<T>::nb_expand( unsigned int n ) {

  if( m_size >= 0 ) {
    m_expand = true;
    m_size += n;
    request_update();
  }
}

template < typename T>
inline
void
tlm_fifo<T>::nb_unbound( unsigned int n ) {

  m_expand = true;
  m_size = -n;

  if( buffer.size() < static_cast<int>( n ) ) {
    buffer.resize( n );
  }

  request_update();

}

template < typename T>
inline
bool
tlm_fifo<T>::nb_reduce( unsigned int n ) {

  if( m_size < 0 ) {
    return false;
  }

  return nb_bound( size() - n );

}

template < typename T>
inline
bool
tlm_fifo<T>::nb_bound( unsigned int new_size ) {

  bool ret = true;

  if( static_cast<int>( new_size ) < used() ) {

    new_size = used();
    ret = false;

  }

  m_size = new_size;
  return ret;

}

} // namespace tlm

#endif
