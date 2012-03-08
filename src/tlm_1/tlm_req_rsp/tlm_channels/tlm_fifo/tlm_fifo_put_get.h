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

#ifndef __TLM_FIFO_PUT_GET_IF_H__
#define __TLM_FIFO_PUT_GET_IF_H__

namespace tlm {

/******************************************************************
//
// get interface
//
******************************************************************/

template <typename T>
inline
T
tlm_fifo<T>::get( tlm_tag<T> * )
{

  while( is_empty() ) {
    wait( m_data_written_event );
  }

  m_num_read ++;
  request_update();

  return buffer.read();

}

// non-blocking read

template <typename T>
inline
bool
tlm_fifo<T>::nb_get( T& val_ )
{

  if( is_empty() ) {
    return false;
  }

  m_num_read ++;
  request_update();

  val_ = buffer.read();

  return true;

}

template <typename T>
inline
bool
tlm_fifo<T>::nb_can_get( tlm_tag<T> * ) const {

  return !is_empty();

}


/******************************************************************
//
// put interface
//
******************************************************************/

template <typename T>
inline
void
tlm_fifo<T>::put( const T& val_ )
{
    while( is_full() ) {
  wait( m_data_read_event );
    }

    if( buffer.is_full() ) {

      buffer.resize( buffer.size() * 2 );

    }

    m_num_written ++;
    buffer.write( val_ );

    request_update();
}

template <typename T>
inline
bool
tlm_fifo<T>::nb_put( const T& val_ )
{

  if( is_full() ) {
    return false;
  }

  if( buffer.is_full() ) {

    buffer.resize( buffer.size() * 2 );

  }

  m_num_written ++;
  buffer.write( val_ );
  request_update();

  return true;
}

template < typename T >
inline
bool
tlm_fifo<T>::nb_can_put( tlm_tag<T> * ) const {

  return !is_full();

}

} // namespace tlm

#endif
