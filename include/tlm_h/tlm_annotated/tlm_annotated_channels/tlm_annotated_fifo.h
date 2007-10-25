
#ifndef __TLM_ANNOTATED_FIFO_H__
#define __TLM_ANNOTATED_FIFO_H__

#include "tlm_h/tlm_core/tlm_fifo/tlm_fifo.h"
#include "tlm_h/tlm_annotated/tlm_annotated_ifs/tlm_annotated_ifs.h"
#include "tlm_peq.h"

namespace tlm {

template< typename T >
class tlm_internal_annotated_fifo :
public tlm_fifo<T> ,
  public virtual analysis_if<T> ,
  public virtual analysis_if< tlm_tag< T > * > {

 public:
  typedef tlm_tag<T> *my_tag_ptr;

  tlm_internal_annotated_fifo( const char *nm , int size ) :
    tlm_fifo<T>( nm , size ) {}

  void write( const T &t ) {
    assert( nb_can_put( (my_tag_ptr)0 ) );
    nb_put( t );
  }

  void write( const my_tag_ptr & ) {
    read_event( (my_tag_ptr)0 ).notify( sc_core::SC_ZERO_TIME );
  }

};

//
// From the put point of view, delayed puts are immediate. The transaction T is
// considered to be in the fifo from the time at which nb_put( T , t ) was
// called.
//
// However, from the get point of view, the effect of the delayed put is not
// seen until t has expired. We cannot see the new data on the get side until
// time t.
//
// The same rules apply to a delayed get : a get( T , t ) gets T immediately,
// but the put side does not see the extra space in the fifo until time t.
//
// In summary : put->put and get->get is immediate, but put->get and get->put
// are delayed.
//
// Although the precise effects of put and get may vary from channel to channel
// these observability rules are part of the definition of the annotated
// interfaces.
//

template< typename T >
class tlm_annotated_fifo :
public sc_core::sc_module ,
public virtual tlm_annotated_put_if< T  > ,
public virtual tlm_annotated_get_peek_if < T  > {

 public:

   tlm_annotated_fifo( sc_core::sc_module_name nm , int size = 1 ) :
     sc_core::sc_module( nm ) ,
     m_size( size ) ,
     m_fifo("internal_fifo" , size ) ,
     m_put_peq("put_peq") ,
     m_get_peq("pop_peq") {

     m_put_peq.ap( m_fifo );
     m_get_peq.ap( m_fifo );

  }

   tlm_annotated_fifo( int size = 1 ) :
     sc_core::sc_module( sc_core::sc_gen_unique_name("annotated_fifo") ) ,
     m_size( size ) ,
     m_fifo("internal_fifo" , size ) ,
     m_put_peq("put_peq") ,
     m_get_peq("pop_peq") {

     m_put_peq.ap( m_fifo );
     m_get_peq.ap( m_fifo );

  }
  // non annotated nb_can_* methods

  bool nb_can_put( tlm_tag<T> *t = 0 ) const {
    return m_fifo.nb_can_put( m_put_peq.size() + m_get_peq.size() + 1 );
  }

  bool nb_can_get( tlm_tag<T> *t = 0 ) const {
    return m_fifo.nb_can_get();
  }

  bool nb_can_peek( tlm_tag<T> *t = 0 ) const {
    return m_fifo.nb_can_peek();
  }

  //
  // annotated nb_can_* methods
  //
  // these are virtual because we may want to add some additional time based
  // restrictions re: what can can be put and popped when : for example, we
  // might want to insist that we only ever post to the end of both peqs, to
  // preserve the ordering of the transactions.
  //

  // nb_can_put is part of the annotated non blocking put interface

  virtual bool nb_can_put( const sc_core::sc_time &time , tlm_tag<T> *t = 0 ) const {
    return nb_can_put();
  }

  virtual bool nb_can_get( const sc_core::sc_time &time , tlm_tag<T> *t = 0 ) const {
    return nb_can_get();
  }

  // ok_to_* methods

  const sc_core::sc_event &ok_to_put(  tlm_tag<T> *t = 0 ) const {
    return m_fifo.ok_to_put();
  }

  const sc_core::sc_event &ok_to_get(  tlm_tag<T> *t = 0 ) const {
    return m_fifo.ok_to_get();
  }

  const sc_core::sc_event &ok_to_peek(  tlm_tag<T> *t = 0 ) const {
    return m_fifo.ok_to_peek();
  }

  // non annotated nonblocking put, get + peek

  bool nb_put( const T &t ) {

    if( !nb_can_put() ) {
      return false;
    }

    assert( m_fifo.nb_can_put() );
    m_fifo.nb_put( t );
    return true;

  }

  bool nb_get( T &t ) {

    if( !nb_can_get() ) {
      return false;
    }

    assert( m_fifo.nb_can_get() );
    m_fifo.nb_get( t );
    return true;

  }

  bool nb_peek( T &t ) const {
    return m_fifo.nb_peek( t );
  }

  // annotated nonblocking methods


  bool nb_put( const T &transaction , const sc_core::sc_time &time ) {

    if( !nb_can_put( time ) ) {
      return false;
    }

    m_put_peq.write( transaction , time );
    return true;

  }

  virtual bool nb_get( T &transaction , const sc_core::sc_time &time ) {
    static tlm_tag<T> *t;

    if( !m_fifo.nb_get_no_notify( transaction ) ) {
      return false;
    }

    m_get_peq.write( t , time );
    return true;

  }

  // blocking put, get and peek

  void put( const T &t ) {
    while( !nb_put( t ) ) {
      wait( ok_to_put() );
    }
  }

  T get( tlm_tag<T> *t = 0 ) {

    T trans;

    while( !nb_get( trans ) ) {
      wait( ok_to_get() );
    }

    return trans;
  }

  T peek( tlm_tag<T> *t = 0 ) const {
    return m_fifo.peek();
  }

  int size() { return m_size; }

 private:
  int m_size;

 protected: // to allow for overriding of annotated can_put and can_get

  tlm_internal_annotated_fifo<T> m_fifo;

  tlm_peq<T> m_put_peq;
  tlm_peq< tlm_tag<T> *> m_get_peq;

};

} // namespace tlm

#endif
