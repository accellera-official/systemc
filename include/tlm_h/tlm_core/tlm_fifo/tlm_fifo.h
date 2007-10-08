
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


#ifndef TLM_FIFO_HEADER
#define TLM_FIFO_HEADER

//
// This implements put, get and peek
//
// It also implements 0 and infinite size fifos - but the size
// zero fifos aren't rendezvous like zero length fifos, they simply are both
// full and empty at the same time.
//
// The size can be dynamically changed using the resize interface
//
// To get an infinite fifo use a -ve size in the constructor.
// The absolute value of the size is taken as the starting size of the
// actual physical buffer.
//

#include <systemc.h>

#include "tlm_core/tlm_interfaces/tlm_fifo_ifs.h"
#include "tlm_core/tlm_fifo/circular_buffer.h"



template <class T>
class tlm_fifo :
  public virtual tlm_fifo_get_if<T>,
  public virtual tlm_fifo_put_if<T>,
  public sc_prim_channel
{
public:

    // constructors

    explicit tlm_fifo( int size_ = 1 )
      : sc_prim_channel( sc_gen_unique_name( "fifo" ) ) {
     
      init( size_ );

    }

    explicit tlm_fifo( const char* name_, int size_ = 1 )
      : sc_prim_channel( name_ ) {
    
      init( size_ );

    }

    // destructor

    virtual ~tlm_fifo() {

      if( buffer != 0 ) delete buffer;

    }

    // tlm get interface

    T get( tlm_tag<T> *t = 0 );

    bool nb_get( T& );
    bool nb_can_get( tlm_tag<T> *t = 0 ) const;
    const sc_event &ok_to_get( tlm_tag<T> *t = 0 ) const {
      return m_data_written_event;
    }

    // tlm peek interface

    T peek( tlm_tag<T> *t = 0 ) const;

    bool nb_peek( T& ) const;
    bool nb_can_peek( tlm_tag<T> *t = 0 ) const;
    const sc_event &ok_to_peek( tlm_tag<T> *t = 0 ) const {
      return m_data_written_event;
    }

    // tlm put interface 

    void put( const T& );

    bool nb_put( const T& );
    bool nb_can_put( tlm_tag<T> *t = 0 ) const;

    const sc_event& ok_to_put( tlm_tag<T> *t = 0 ) const {
      return m_data_read_event;
    }

    // resize if

    void nb_expand( unsigned int n = 1 );
    void nb_unbound( unsigned int n = 16 );

    bool nb_reduce( unsigned int n = 1 );
    bool nb_bound( unsigned int n );

    // debug interface

    bool nb_peek( T & , int n ) const;
    bool nb_poke( const T & , int n = 0 );

    int used() const {
      return m_num_readable - m_num_read;
    }

    int size() const {
      return m_size;
    }

    void debug() const {

      if( is_empty() ) cout << "empty" << endl;
      if( is_full() ) cout << "full" << endl;

      cout << "size " << size() << " - " << used() << " used " << endl;
      cout << "readable " << m_num_readable << endl;
      cout << "written/read " << m_num_written << "/" << m_num_read << endl;

    }

    // support functions

    static const char* const kind_string;

    const char* kind() const
        { return kind_string; }


    friend class tlm_annotated::tlm_annotated_fifo<T>;

private: // some methods to support tlm_annotated_fifo !

    bool nb_can_put( int count , tlm_tag<T> *t = 0 ) const {
      if( m_size < 0 ) {
	return true;
      }
      return m_num_readable + m_num_written + count <= m_size;
    }

    bool nb_get_no_notify( T& );

protected:
    sc_event &read_event( tlm_tag<T> *t = 0 ) {
      return m_data_read_event;
    }

protected:

    void update();

    // support methods

    void init( int );

protected:

    circular_buffer<T> *buffer;

    int m_size;                  // logical size of fifo

    int m_num_readable;	         // #samples readable
    int m_num_read;	         // #samples read during this delta cycle
    int m_num_written;	         // #samples written during this delta cycle
    bool m_expand;               // has an expand occurred during this delta cycle ? 
    int m_num_read_no_notify;    // #samples read without notify during this delta cycle

    sc_event m_data_read_event;
    sc_event m_data_written_event;

private:

    // disabled
    tlm_fifo( const tlm_fifo<T>& );
    tlm_fifo& operator = ( const tlm_fifo<T>& );

    //
    // use nb_can_get() and nb_can_put() rather than the following two
    // private functions
    //

    bool is_empty() const { 
      return used() == 0;
    }

    bool is_full() const {
      return size() == m_num_readable + m_num_written;
    }

};

template <typename T>
const char* const tlm_fifo<T>::kind_string = "tlm_fifo";


/******************************************************************
//
// init and update
//
******************************************************************/

template< typename T >
inline 
void
tlm_fifo<T>::init( int size_ ) {

  if( size_ > 0 ) {
    buffer = new circular_buffer<T>( size_);
  }
  
  else if( size_ < 0 ) {
    buffer = new circular_buffer<T>( -size_ );
  }

  else {
    buffer = new circular_buffer<T>( 16 );
  }

  m_size = size_;
  m_num_readable = 0;
  m_num_read = 0;
  m_num_written = 0;
  m_expand = false;
  m_num_read_no_notify = false;

}

template < typename T>
inline
void
tlm_fifo<T>::update()
{
    if( m_num_read > m_num_read_no_notify || m_expand ) {
	m_data_read_event.notify( SC_ZERO_TIME );
    }

    if( m_num_written > 0 ) {
	m_data_written_event.notify( SC_ZERO_TIME );
    }

    m_expand = false;
    m_num_read = 0;
    m_num_written = 0;
    m_num_readable = buffer->used();
    m_num_read_no_notify = 0;

}

#include "tlm_core/tlm_fifo/tlm_fifo_put_get.h"
#include "tlm_core/tlm_fifo/tlm_fifo_peek.h"
#include "tlm_core/tlm_fifo/tlm_fifo_resize.h"

#endif

