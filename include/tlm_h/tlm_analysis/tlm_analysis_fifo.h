
#ifndef ANALYSIS_FIFO_HEADER
#define ANALYSIS_FIFO_HEADER

//#include "tlm_core.h"

#include "tlm_analysis_if.h"
#include "tlm_analysis_triple.h"

using tlm_core::tlm_fifo;

template< typename T >
class analysis_fifo :
  public tlm_fifo< T > ,
  public virtual analysis_if< T > ,
  public virtual analysis_if< analysis_triple< T > > {

 public:

 // analysis fifo is an unbounded tlm_fifo

  analysis_fifo( const char *nm ) : tlm_fifo<T>( nm , -16 ) {}
  analysis_fifo() : tlm_fifo<T>( -16 ) {}

  void write( const analysis_triple<T> &t ) {
    nb_put( t );
  }

  void write( const T &t ) {
    nb_put( t );
  }

};

#endif
