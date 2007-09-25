
#ifndef ANALYSIS_PORT_HEADER
#define ANALYSIS_PORT_HEADER

#include "tlm_analysis_if.h"
#include <deque>


template < typename T>
class analysis_port :
  public sc_object ,
  public virtual analysis_if< T >
{
 public:
  analysis_port() : sc_object() {}
  analysis_port( const char *nm ) : sc_object( nm ) {}

  // bind and () work for both interfaces and analysis ports, since 
  // analysis ports implement the analysis interface

  void bind( analysis_if<T> &_if ) {
    m_interfaces.push_back( &_if );
  }

  void operator() ( analysis_if<T> &_if ) { bind( _if ); }

  bool unbind( analysis_if<T> &_if ) {

    typename std::deque< analysis_if<T> *>::iterator i;

    for( i = m_interfaces.begin(); 
	 i != m_interfaces.end();
	 i++ ) {

      if( *i == &_if ) {

	m_interfaces.erase( i );
	return 1;

      }

    }

    return 0;

  }

  void write( const T &t ) {
    typename std::deque< analysis_if<T> * >::iterator i;
 
    for( i = m_interfaces.begin(); 
	 i != m_interfaces.end();
	 i++ ) {

      (*i)->write( t );

    }

  }

 private:
  std::deque< analysis_if<T> * > m_interfaces;

};

#endif
