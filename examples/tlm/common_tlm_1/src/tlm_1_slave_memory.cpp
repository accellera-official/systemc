/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include "tlm_1_slave_memory.h"                       ///< TLM 1 slave memory
#include "reporting.h"                                ///< Reporting convenience macros

static const char *report_source = "tlm_1_mem_slave"; ///< filename for reporting

using basic_protocol::basic_status;

tlm_1_mem_slave::tlm_1_mem_slave( sc_module_name module_name , int k ) :
  sc_module( module_name ) ,
  target_port("iport")
{

  target_port( *this );

  memory = new ADDRESS_TYPE[ k * 1024 ];

}

basic_status tlm_1_mem_slave::write( const ADDRESS_TYPE &address , const DATA_TYPE &write_data )
{

  std::ostringstream msg;                         ///< log message
  msg << "W -";
  msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) 
      << setfill( '0' ) << uppercase << hex << address;
  msg << " D: 0x"<< hex << write_data ;
  REPORT_INFO(report_source, __FUNCTION__, msg.str());

  memory[address] = write_data;

  return basic_protocol::SUCCESS;
}

basic_status tlm_1_mem_slave::read( const ADDRESS_TYPE &address , DATA_TYPE &read_data )
{
  read_data = memory[address];

  std::ostringstream msg;                         ///< log message
  msg << "R -";
  msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) 
      << setfill( '0' ) << uppercase << hex << address;
  msg << " D: 0x"<< hex << read_data ;
  REPORT_INFO(report_source, __FUNCTION__, msg.str());

//  cout << name() << " reading from " << a << " value " << hex << d << endl;
  return basic_protocol::SUCCESS;
}
 
tlm_1_mem_slave::~tlm_1_mem_slave() {

  delete [] memory;

}
