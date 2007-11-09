
#ifndef __TLM_PEQ_H__
#define __TLM_PEQ_H__

#include <map>
#include "tlm_h/tlm_analysis/tlm_analysis_if.h"
#include "tlm_h/tlm_analysis/tlm_analysis_port.h"

namespace tlm {

//
// No lt_sc_time operator required, since we have operator< defined for
// sc_time ?
//
// struct lt_sc_time {
//  bool operator<( const sc_core::sc_time &a , const sc_core::sc_time &b ) {
//    return a < b;
//  }
// };
//
//
//

//
// This peq implements a delayed write and has an analysis port
//
// If you post many transactions to the same time slot, this will result in
// many transactions coming out of the analysis port in a single delta
//
// If you want event driven semantics, stuff the output of the analysis port
// into an analysis fifo ( if you want to guarantee no losses ) or
// analysis buffer ( if you don't mind losses but you can't be bothered or are
// not able to clear the fifo out ).
//
// For example, you can arrive at a blocking get interface by doing get on
// an analysis fifo attached to the analysis port shown below
//
// We could even design a tlm_peq_fifo and/or tlm_peq_buffer with these
// channels built in
//

template< typename T>
class tlm_peq :
  public sc_core::sc_module ,
  public virtual tlm_delayed_analysis_if< T >
{
 public:
  sc_core::sc_export< tlm_delayed_analysis_if< T > > delayed_analysis_export;
  tlm_analysis_port< T > ap;

  SC_HAS_PROCESS( tlm_peq );

  tlm_peq( sc_core::sc_module_name nm ) : sc_core::sc_module( nm ) , ap("ap") {

    delayed_analysis_export( *this );

    SC_METHOD( wake_up_method );
    dont_initialize();
    sensitive << m_wake_up;

  }

  int size() const { return m_map.size(); }

  int posted_before( const sc_core::sc_time &time ) const {

    int i = 0;

    for( typename std::multimap< sc_core::sc_time , T>::const_iterator iter = m_map.begin();
	 iter != m_map.end();
	 ++iter ) {

      if( (*iter).first < time ) {
	i++;
      }

    }

    return i;

  }

  int posted_at( const sc_core::sc_time &time ) const {
    return m_map.count( time );
  }

  void write( const T &transaction , const sc_core::sc_time &time ) {

    m_map.insert( pair_type( time + sc_core::sc_time_stamp() , transaction ) );
    m_wake_up.notify( time );

  }

 private:
  typedef std::pair<sc_core::sc_time,T> pair_type;

  void wake_up_method() {

    pair_type p;
    sc_core::sc_time now = sc_core::sc_time_stamp();

    // must be something there, and it must be scheduled for now

    assert( m_map.size() > 0 );
    assert( (*(m_map.begin())).first == now );

    for( p = *(m_map.begin());
	 p.first == now;
	 p = *(m_map.begin()) )
    {

      ap.write( p.second );
      m_map.erase( m_map.begin() );

      if( m_map.size() == 0 ) {
	return;
      }

      p = *(m_map.begin());

    }

    m_wake_up.notify( p.first - now );

  }

  sc_core::sc_event m_wake_up;
  std::multimap< sc_core::sc_time , T> m_map;

};

} // namespace tlm

#endif
