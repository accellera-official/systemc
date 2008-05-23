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
// To the LRM writer : this class is purely an artifact of the implementation.
//

#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <iostream>

namespace tlm {

template < typename T >
class circular_buffer
{
public:
  circular_buffer( int size );

  circular_buffer<T> &operator=( const circular_buffer<T> & ); 

  ~circular_buffer() { delete [] m_buf; }
  
  void resize( int size );

  const T &read();
  void write( const T & );

  bool is_empty() const { return used() == 0; }
  bool is_full() const { return free() == 0; }

  int size() const { return m_size; }
  int used() const { return m_used; }
  int free() const { return m_free; }

  T &write_data() { return m_buf[m_wi]; }
  const T &read_data() const { return m_buf[m_ri]; }

  const T &peek_data( int i ) const { return m_buf[(m_ri + i) % size()]; }
  T &poke_data( int i ) { return m_buf[(m_ri + i) % size()]; }

  void increment_write_pos( int i = 1 );
  void increment_read_pos( int i = 1 );

  void init();

  void debug() const;

private:
  circular_buffer( const circular_buffer<T> &b ); // disabled
  void copy( const circular_buffer<T> &b );

private:
  int m_size;			// size of the buffer
  T*  m_buf;			// the buffer
  int m_free;                   // number of free spaces
  int m_used;                   // number of used spaces
  int m_ri;			// index of next read
  int m_wi;			// index of next write
  
};

template< typename T >
void
circular_buffer<T>::debug() const
{

  std::cout << "Buffer debug" << std::endl;
  std::cout << "Size : " << size() << std::endl;
  std::cout << "Free/Used " << free() << "/" << used() << std::endl;
  std::cout << "Indeces : r/w = " << m_ri << "/" << m_wi << std::endl;

  if( is_empty() ) {

    std::cout << "empty" << std::endl;

  }

  if( is_full() ) {

    std::cout << "full" << std::endl;

  }

  std::cout << "Data : " << std::endl;
  for( int i = 0; i < used(); i++ ) {

    std::cout << peek_data( i ) << std::endl;

  }


}

template < typename T >
circular_buffer<T>::
circular_buffer( int size ) {

  m_size = size;
  m_buf = new T[m_size];

  init();

}
 
template < typename T >
circular_buffer<T> &
circular_buffer<T>::operator=( const circular_buffer<T> &b ) {

  init();

  for( int i = 0; i < size() && i < b.used(); i++ ) {

    write( b.peek_data( i ) );

  }

  return *this;
  
}

template < typename T >
void
circular_buffer<T>::resize( int size )
{

  int i;
  T *new_buf = new T[size];

  for( i = 0; i < size && i < used(); i++ ) {

    new_buf[i] = peek_data( i );

  }

  delete [] m_buf;

  m_size = size;
  m_ri = 0;
  m_wi = i % m_size;
  m_used = i;
  m_free = m_size - m_used;
  
  m_buf = new_buf;

}
 

template < typename T >
void
circular_buffer<T>::init() {

  m_free = m_size;
  m_used = 0;
  m_ri = 0;
  m_wi = 0;

}

template < typename T >
void
circular_buffer<T>::copy( const circular_buffer<T> &b )
{

  m_size = b.m_size;   // size of the buffer
  m_buf = b.m_buf;     // the buffer
  m_free = b.m_free;   // number of free spaces
  m_used = b.m_used;   // number of used spaces
  m_ri = b.m_ri;       // index of next read
  m_wi = b.m_wi;       // index of next write  

}

template < typename T >
const T &
circular_buffer<T>::read()
{

  const T &t = read_data();

  increment_read_pos();

  return t;

}

template < typename T >
void
circular_buffer<T>::write( const T &t )
{

  write_data() = t;
  increment_write_pos();

}


template < typename T >
void
circular_buffer<T>::increment_write_pos( int i ) {

  m_wi = ( m_wi + i ) % m_size;
  m_used += i;
  m_free -= i;
  
}

template < typename T >
void
circular_buffer<T>::increment_read_pos( int i ) {

  m_ri = ( m_ri + i ) % m_size;
  m_used -= i;
  m_free += i;
  
}

} // namespace tlm

#endif

