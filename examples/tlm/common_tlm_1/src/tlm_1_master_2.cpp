/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#include "tlm_1_master_2.h"
#include <systemc>
#include "reporting.h"                      ///< Reporting convenience macros

static const char *report_source = "tlm_1_master_2";


tlm_1_master_2::tlm_1_master_2( sc_module_name module_name ) :
  sc_module( module_name ) , 
  initiator_port("iport")
{
  SC_THREAD( run );
}


void tlm_1_master_2::run()
{
  unsigned int data_seed = 0x01020304;
  while (true) 
  {
    unsigned int write_data = data_seed;

    for( ADDRESS_TYPE address = 0; address < 30; address++ ){

      std::ostringstream msg;                         ///< log message
      msg << "W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) 
          << setfill( '0' ) << uppercase << hex << address;
      msg << " D: 0x"<< hex << write_data ;
      REPORT_INFO(report_source, __FUNCTION__, msg.str());

      initiator_port.write( address , write_data );   // TLM 1 write request 

      write_data += 0x10101010;
//      wait(4, SC_NS);
    }

    cout << endl << "======================================================" << endl;

    unsigned int expected_data = data_seed;

    for( ADDRESS_TYPE address = 0; address < 30; address++ )
    {
      unsigned int read_data;
   
      initiator_port.read( address , read_data);      // TLM 1 read request 

      std::ostringstream msg;                         ///< log message
      msg << "R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) 
          << setfill( '0' ) << uppercase << hex << address;
      msg << "     Read D: 0x"<< hex << read_data ;
      msg << " Expected D: 0x"<< hex << expected_data ;
      REPORT_INFO(report_source, __FUNCTION__, msg.str());

      if (read_data != expected_data){
        cout << endl << "**** Read Data Error ****" << endl;
      }

      expected_data += 0x10101010;
//      wait(6, SC_NS);
    }
  }
}
