
#ifndef __TLM_ANALYSIS_FIFO_H__
#define __TLM_ANALYSIS_FIFO_H__

//#include "tlm_core.h"

#include "tlm_h/tlm_annotated/tlm_annotated_channels/tlm_annotated_fifo.h"
#include "tlm_h/tlm_analysis/tlm_analysis_if.h"
#include "tlm_h/tlm_analysis/tlm_analysis_triple.h"

namespace tlm {

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

} // namespace tlm

#endif
