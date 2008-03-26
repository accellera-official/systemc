/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

/*****************************************************************************

  sc_fifo.h -- The sc_fifo<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_FIFO_H
#define SC_FIFO_H


#include "sysc/communication/sc_communication_ids.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/communication/sc_fifo_ifs.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_string.h"
#include "sysc/tracing/sc_trace.h"
#include <typeinfo>

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_fifo<T>
//
//  The sc_fifo<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo
: public sc_fifo_in_if<T>,
  public sc_fifo_out_if<T>,
  public sc_prim_channel
{
public:

    // constructors

    explicit sc_fifo( int size_ = 16 )
	: sc_prim_channel( sc_gen_unique_name( "fifo" ) )
	{ init( size_ ); }

    explicit sc_fifo( const char* name_, int size_ = 16 )
	: sc_prim_channel( name_ )
	{ init( size_ ); }


    // destructor

    virtual ~sc_fifo()
	{ delete [] m_buf; }


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // blocking read
    virtual void read( T& );
    virtual T read();

    // non-blocking read
    virtual bool nb_read( T& );


    // get the number of available samples

    virtual int num_available() const
	{ return ( m_num_readable - m_num_read ); }


    // get the data written event

    virtual const sc_event& data_written_event() const
	{ return m_data_written_event; }


    // blocking write
    virtual void write( const T& );

    // non-blocking write
    virtual bool nb_write( const T& );


    // get the number of free spaces

    virtual int num_free() const
	{ return ( m_size - m_num_readable - m_num_written ); }


    // get the data read event

    virtual const sc_event& data_read_event() const
	{ return m_data_read_event; }


    // other methods

    operator T ()
	{ return read(); }


    sc_fifo<T>& operator = ( const T& a )
        { write( a ); return *this; }


    void trace( sc_trace_file* tf ) const;


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_fifo"; }

protected:

    virtual void update();

    // support methods

    void init( int );

    void buf_init( int );
    bool buf_write( const T& );
    bool buf_read( T& );

protected:

    int m_size;			// size of the buffer
    T*  m_buf;			// the buffer
    int m_free;			// number of free spaces
    int m_ri;			// index of next read
    int m_wi;			// index of next write

    sc_port_base* m_reader;	// used for static design rule checking
    sc_port_base* m_writer;	// used for static design rule checking

    int m_num_readable;		// #samples readable
    int m_num_read;		// #samples read during this delta cycle
    int m_num_written;		// #samples written during this delta cycle

    sc_event m_data_read_event;
    sc_event m_data_written_event;

private:

    // disabled
    sc_fifo( const sc_fifo<T>& );
    sc_fifo& operator = ( const sc_fifo<T>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
inline
void
sc_fifo<T>::register_port( sc_port_base& port_,
			    const char* if_typename_ )
{
    std::string nm( if_typename_ );
    if( nm == typeid( sc_fifo_in_if<T> ).name() ) {
	// only one reader can be connected
	if( m_reader != 0 ) {
	    SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 );
	}
	m_reader = &port_;
    } else {  // nm == typeid( sc_fifo_out_if<T> ).name()
	// only one writer can be connected
	if( m_writer != 0 ) {
	    SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 );
	}
	m_writer = &port_;
    }
}


// blocking read

template <class T>
inline
void
sc_fifo<T>::read( T& val_ )
{
    while( num_available() == 0 ) {
	sc_core::wait( m_data_written_event );
    }
    m_num_read ++;
    buf_read( val_ );
    request_update();
}

template <class T>
inline
T
sc_fifo<T>::read()
{
    T tmp;
    read( tmp );
    return tmp;
}

// non-blocking read

template <class T>
inline
bool
sc_fifo<T>::nb_read( T& val_ )
{
    if( num_available() == 0 ) {
	return false;
    }
    m_num_read ++;
    buf_read( val_ );
    request_update();
    return true;
}


// blocking write

template <class T>
inline
void
sc_fifo<T>::write( const T& val_ )
{
    while( num_free() == 0 ) {
	sc_core::wait( m_data_read_event );
    }
    m_num_written ++;
    buf_write( val_ );
    request_update();
}

// non-blocking write

template <class T>
inline
bool
sc_fifo<T>::nb_write( const T& val_ )
{
    if( num_free() == 0 ) {
	return false;
    }
    m_num_written ++;
    buf_write( val_ );
    request_update();
    return true;
}


template <class T>
inline
void
sc_fifo<T>::trace( sc_trace_file* tf ) const
{
#ifdef DEBUG_SYSTEMC
    char buf[32];
    std::string nm = name();
    for( int i = 0; i < m_size; ++ i ) {
	sprintf( buf, "_%d", i );
	sc_trace( tf, m_buf[i], nm + buf );
    }
#endif
}


template <class T>
inline
void
sc_fifo<T>::print( ::std::ostream& os ) const
{
    if( m_free != m_size ) {
        int i = m_ri;
        do {
            os << m_buf[i] << ::std::endl;
            i = ( i + 1 ) % m_size;
        } while( i != m_wi );
    }
}

template <class T>
inline
void
sc_fifo<T>::dump( ::std::ostream& os ) const
{
    os << "name = " << name() << ::std::endl;
    if( m_free != m_size ) {
        int i = m_ri;
        int j = 0;
        do {
	    os << "value[" << i << "] = " << m_buf[i] << ::std::endl;
	    i = ( i + 1 ) % m_size;
	    j ++;
        } while( i != m_wi );
    }
}


template <class T>
inline
void
sc_fifo<T>::update()
{
    if( m_num_read > 0 ) {
	m_data_read_event.notify_delayed();
    }

    if( m_num_written > 0 ) {
	m_data_written_event.notify_delayed();
    }

    m_num_readable = m_size - m_free;
    m_num_read = 0;
    m_num_written = 0;
}


// support methods

template <class T>
inline
void
sc_fifo<T>::init( int size_ )
{
    buf_init( size_ );

    m_reader = 0;
    m_writer = 0;

    m_num_readable = 0;
    m_num_read = 0;
    m_num_written = 0;
}


template <class T>
inline
void
sc_fifo<T>::buf_init( int size_ )
{
    if( size_ <= 0 ) {
	SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 );
    }
    m_size = size_;
    m_buf = new T[m_size];
    m_free = m_size;
    m_ri = 0;
    m_wi = 0;
}

template <class T>
inline
bool
sc_fifo<T>::buf_write( const T& val_ )
{
    if( m_free == 0 ) {
	return false;
    }
    m_buf[m_wi] = val_;
    m_wi = ( m_wi + 1 ) % m_size;
    m_free --;
    return true;
}

template <class T>
inline
bool
sc_fifo<T>::buf_read( T& val_ )
{
    if( m_free == m_size ) {
	return false;
    }
    val_ = m_buf[m_ri];
    m_ri = ( m_ri + 1 ) % m_size;
    m_free ++;
    return true;
}


// ----------------------------------------------------------------------------

template <class T>
inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_fifo<T>& a )
{
    a.print( os );
    return os;
}

} // namespace sc_core

#endif

// Taf!
