
#ifndef TLM_ANNOTATED_REQ_RSP_HEADER
#define TLM_ANNOTATED_REQ_RSP_HEADER

#include "tlm_annotated/tlm_annotated_channels/tlm_annotated_fifo.h"
#include "tlm_annotated/tlm_annotated_channels/tlm_annotated_put_get_imp.h"


template < typename REQ , typename RSP ,
	   typename REQ_CHANNEL = tlm_annotated_fifo<REQ> ,
	   typename RSP_CHANNEL = tlm_annotated_fifo<RSP> >
class tlm_annotated_req_rsp_channel : public sc_module
{
public:
  // uni-directional slave interface

  sc_export< tlm_annotated_get_peek_if< REQ > > get_request_export;
  sc_export< tlm_annotated_put_if< RSP > > put_response_export;

  // uni-directional master interface

  sc_export< tlm_annotated_put_if< REQ > > put_request_export;
  sc_export< tlm_annotated_get_peek_if< RSP > > get_response_export;

  // master / slave interfaces

  sc_export< tlm_annotated_master_if< REQ , RSP  > > master_export;
  sc_export< tlm_annotated_slave_if< REQ , RSP > > slave_export;


  tlm_annotated_req_rsp_channel( int req_size = 1 , int rsp_size = 1 ) :
    sc_module( sc_module_name( sc_gen_unique_name("tlm_req_rsp_channel") ) ) ,
    request_fifo( "req_fifo" , req_size ) ,
    response_fifo( "rsp_fifo" , rsp_size ) ,
    master( request_fifo , response_fifo ) , 
    slave( request_fifo , response_fifo )
  {

    bind_exports();
    
  }

  tlm_annotated_req_rsp_channel( sc_module_name module_name ,
				 int req_size = 1 , int rsp_size = 1 ) :
    sc_module( module_name  ) , 
    request_fifo( "req_fifo" , req_size ) ,
    response_fifo( "rsp_fifo" , rsp_size ) ,
    master( request_fifo , response_fifo ) , 
    slave( request_fifo , response_fifo )
  {

    bind_exports();
    
  }

private:
  void bind_exports() {

    put_request_export( request_fifo );
    get_request_export( request_fifo );
    
    put_response_export( response_fifo );
    get_response_export( response_fifo );

    master_export( master );
    slave_export( slave );

  }

protected:
  REQ_CHANNEL request_fifo;
  RSP_CHANNEL response_fifo;

  tlm_annotated_master_imp< REQ , RSP > master;
  tlm_annotated_slave_imp< REQ , RSP > slave;
};


#endif
