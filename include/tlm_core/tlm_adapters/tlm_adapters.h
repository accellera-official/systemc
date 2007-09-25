
#ifndef TLM_ADAPTERS_HEADER
#define TLM_ADAPTERS_HEADER

#include "tlm_core/tlm_interfaces/tlm_master_slave_ifs.h"

template< typename REQ , typename RSP >
class tlm_transport_to_master :
  public sc_module ,
  public virtual tlm_transport_if< REQ , RSP >
{
public:
  sc_export< tlm_transport_if< REQ , RSP > > target_export;
  sc_port< tlm_master_if< REQ , RSP > > master_port;

  tlm_transport_to_master( sc_module_name nm ) :
    sc_module( nm ) {

    target_export( *this );

  }

  tlm_transport_to_master() :
    sc_module( sc_module_name( sc_gen_unique_name( "transport_to_master" ) ) ){

    target_export( *this );

  }

  RSP transport( const REQ &req ) {

    mutex.lock();

    master_port->put( req );
    rsp = master_port->get();

    mutex.unlock();
    return rsp;

  }

private:
  sc_mutex mutex;
  RSP rsp;

};

template< typename REQ , typename RSP >
class tlm_slave_to_transport : public sc_module
{
public:

  SC_HAS_PROCESS( tlm_slave_to_transport );
  
  sc_port< tlm_slave_if< REQ , RSP > > slave_port;
  sc_port< tlm_transport_if< REQ , RSP > > initiator_port;

  tlm_slave_to_transport( sc_module_name nm ) : sc_module( nm )
  {}

  tlm_slave_to_transport() :
    sc_module( sc_module_name( sc_gen_unique_name("slave_to_transport") ) )
  {}
  
private:
  void run() {
 
    REQ req;
    RSP rsp;

    while( true ) {

     slave_port->get( req );
     rsp = initiator_port->transport( req );
     slave_port->put( rsp );

    }

  }
 
};
#endif
