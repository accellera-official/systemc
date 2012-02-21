/**********************************************************************
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
 *********************************************************************/
//=====================================================================
///  @file report.cpp
//
///  @Details
///
//
//=====================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//=====================================================================

#include "reporting.h"                                // Reporting convenience macros

//static const char *filename = "report.cpp"; ///< filename for reporting

namespace report {
std::string print(const tlm::tlm_phase phase)
{
  std::string os;
  switch (phase)
  {
    case tlm::BEGIN_REQ:
      {
        os =  "BEGIN_REQ";
        break;
      }
    case tlm::END_REQ:
      {
        os = "END_REQ";
        break;
      }
    case tlm::BEGIN_RESP:
      {
        os = "BEGIN_RESP";
        break;
      }
    case tlm::END_RESP:
      {
        os = "END_RESP";
        break;
      }
    default:
      {
//        char buffer[8];
//        snprintf(buffer,8," %d",int(phase));
//        os += buffer;
        os += "UNKNOWN: " + phase;
        break;
      }
  }//end switch
  return os;
}

std::string print(const tlm::tlm_sync_enum status)
{ //
  std::string os ( "TLM_" );
  switch (status)
  {
    case tlm::TLM_COMPLETED:
      {
        os = "COMPLETED";
        break;
      }
    case tlm::TLM_UPDATED:
      {
        os = "UPDATED";
        break;
      }
    case tlm::TLM_ACCEPTED:
      {
        os = "ACCEPTED";
        break;
      }
    default:
      {
//        char buffer[8];
//        snprintf(buffer,8," %d",int(status));
//        os += buffer;
        os += "UNKNOWN: " + os;
        break;
      }
  }//end switch
  return os;
}
//=====================================================================
//
///  @brief helper function for printing memory transactions
//
//=====================================================================
void
print
( const int                 &ID              ///< Target ID/Initiator/?ID
, tlm::tlm_generic_payload  &gp          ///< transaction to be printed
, const char*               calling_filename
)
{
  std::ostringstream     msg;
  msg.str("");

  sc_dt::uint64 print_address  = gp.get_address();      // memory address
  unsigned char *print_data    = gp.get_data_ptr();    // data pointer
  unsigned int  print_length   = gp.get_data_length(); // data length
  tlm::tlm_command print_command = gp.get_command();     // memory command

  msg << "ID: "<< ID
      << " COMMAND: "
      << ((print_command == tlm::TLM_WRITE_COMMAND) ? "WRITE" : "READ")
      << " Length: " << internal << setw( 2 ) << setfill( '0' )
      << dec << print_length
      << endl
      << "      Addr: 0x" << internal << setw( sizeof(print_address) * 2 )
      << setfill( '0' ) << uppercase << hex << print_address
      << " Data: 0x"

      // For consistency across hosts (LE/BE), must interpret word as MSB:LSB rather than
      // a sequence of bytes in address order
      << internal << setw( 8 ) << setfill( '0' ) << uppercase << hex
      << *reinterpret_cast<unsigned int*>(print_data);

  REPORT_INFO(calling_filename, __FUNCTION__, msg.str());
}
//=====================================================================
//
///  @brief helper function for printing memory transactions
//
//=====================================================================
void
print_full
( const int                 &ID              ///< Target ID/Initiator/?ID
, tlm::tlm_generic_payload  &gp          ///< transaction to be printed
, const char*               caller_filename
)
{
  std::ostringstream     msg;
  msg.str("");

  sc_dt::uint64 print_address  = gp.get_address();      // memory address
  unsigned char *print_data    = gp.get_data_ptr();    // data pointer
  unsigned int  print_length   = gp.get_data_length(); // data length
  tlm::tlm_command print_command = gp.get_command();     // memory command

  msg << "ID: "<< ID
      << " COMMAND: "
      << ((print_command == tlm::TLM_WRITE_COMMAND) ? "WRITE" : "READ")
      << " Length: " << internal << setw( 2 ) << setfill( '0' ) << dec << print_length
      << endl
      << "      Addr: 0x" << internal << setw( sizeof(print_address) * 2 )
      << setfill( '0' ) << uppercase << hex << print_address
      << " Data: 0x"
      << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)print_data[3]
      << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)print_data[2]
      << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)print_data[1]
      << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)print_data[0]
      << endl
      << "      "
      << "RSP STATUS = " << gp.get_response_string()
      << " STREAMING WIDTH = " << gp.get_streaming_width()
      << endl << "      "
      << "DMI ALLOWED = " << gp.is_dmi_allowed()
      << endl << "      "
      << "BYTE ENABLE LENGTH = " << gp.get_byte_enable_length();

  REPORT_INFO(caller_filename, __FUNCTION__, msg.str());
}
void
print
( const int                 &ID              ///< Target ID/Initiator/?ID
, tlm::tlm_dmi              &dmi_properties     ///< dmi transaction to be printed
, const char*               calling_filename
)
{
   std::ostringstream     msg;
   msg.str("");
   msg << "Initiator: " << ID
//       << " &dmi_properties = " << &dmi_properties
       << endl << "      "
       << "start addr     = " << dmi_properties.get_start_address()
       << endl << "      "
       << "end addr       = " << dmi_properties.get_end_address()
       << endl << "      "
       << "read latency   = " << dmi_properties.get_read_latency()
       << endl << "      "
       << "write latency  = " << dmi_properties.get_write_latency()
       << endl << "      "
       << "granted access = " << dmi_properties.get_granted_access();

//   unsigned char*    temp_dmi_ptr = dmi_properties.get_dmi_ptr();
//   msg  << endl << "      "
//        << "dmi_ptr = " << temp_dmi_ptr;

   REPORT_INFO(calling_filename, __FUNCTION__, msg.str());

}

}//end namespace
