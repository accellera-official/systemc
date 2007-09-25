
#ifndef TLM_BUS_ANALYSIS_HEADER
#define TLM_BUS_ANALYSIS_HEADER

#include "analysis.h"
#include "tlm_bus.h"

//
// This file disables analysis_fifo ( and analysis_buffer, when we've done 
// that ) for ptr versions of tlm_request, tlm_response, and the analysis
// triple versions of those data structures
//
// We also provide widgets to convert from the by ptr version to the by val
// version
//

namespace analysis {

  using tlm_bus::tlm_request;
  using tlm_bus::tlm_response;

  using tlm_bus::TLM_PASS_BY_COPY;
  using tlm_bus::TLM_PASS_BY_POINTER;

  template< typename ADDRESS , typename DATA>
  class analysis_fifo<
    tlm_bus::tlm_request< ADDRESS , DATA , tlm_bus::TLM_PASS_BY_POINTER >
  > {};

  template< typename DATA>
  class analysis_fifo<
    tlm_bus::tlm_response< DATA , tlm_bus::TLM_PASS_BY_POINTER >
  > {};

  template< typename ADDRESS , typename DATA>
  class analysis_fifo<
    analysis_triple<
      tlm_bus::tlm_request< ADDRESS , DATA , tlm_bus::TLM_PASS_BY_POINTER >
    >
  > {};

  template< typename DATA>
  class analysis_fifo<
    analysis_triple<
      tlm_bus::tlm_response< DATA , tlm_bus::TLM_PASS_BY_POINTER >
    >
  > {};

  template< typename FROM , typename TO >
  class standard_converter {
  public:
    static TO convert( const FROM &from ) { return from; }
  };

  template< typename FROM , typename TO ,
            typename CONVERTER = standard_converter<FROM,TO> >
  class analysis_converter :
    public sc_module ,
    public virtual analysis_if< FROM > ,
    public virtual analysis_if< analysis_triple< FROM > > {

    analysis_port< TO > ap;
    analysis_port< analysis_triple< TO > > triple_ap;

    analysis_converter( sc_module_name nm ) :
      sc_module( nm ) ,
      ap("ap") ,
      triple_ap("triple_ap") {}

    void write( const analysis_triple<FROM> &t ) {
      TO to_val = CONVERTER( t );

      triple_ap.write( analysis_triple<TO>( t.start_time ,
					    to_val ,
					    t.from_time ) );
      ap.write( to_val );
      
    }

    void write( const FROM &f ) {
      ap.write( CONVERTER::convert( f ) );
    }

  };

};
#endif
