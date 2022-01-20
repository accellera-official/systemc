/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
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
  std::stringstream os;
  switch (phase)
  {
    case tlm::BEGIN_REQ:
      {
        os <<  "BEGIN_REQ";
        break;
      }
    case tlm::END_REQ:
      {
        os << "END_REQ";
        break;
      }
    case tlm::BEGIN_RESP:
      {
        os << "BEGIN_RESP";
        break;
      }
    case tlm::END_RESP:
      {
        os << "END_RESP";
        break;
      }
    default:
      {
        os << "UNKNOWN_" << phase;
        break;
      }
  }//end switch
  return os.str();
}

std::string print(const tlm::tlm_sync_enum status)
{ //
  std::stringstream os;
  // os << "TLM_" ;
  switch (status)
  {
    case tlm::TLM_COMPLETED:
      {
        os << "COMPLETED";
        break;
      }
    case tlm::TLM_UPDATED:
      {
        os << "UPDATED";
        break;
      }
    case tlm::TLM_ACCEPTED:
      {
        os << "ACCEPTED";
        break;
      }
    default:
      {
        os << "UNKNOWN_" << status;
        break;
      }
  }//end switch
  return os.str();
}

std::string print(const tlm::tlm_response_status status)
{ 
  std::stringstream os;
  switch (status)
  {
    case tlm::TLM_OK_RESPONSE:
      {
        os << "OK_RESPONSE";
        break;
      }
    case tlm::TLM_INCOMPLETE_RESPONSE:
      {
        os << "INCOMPLETE_RESPONSE";
        break;
      }
    case tlm::TLM_GENERIC_ERROR_RESPONSE:
      {
        os << "GENERIC_ERROR_RESPONSE";
        break;
      }

    case tlm::TLM_ADDRESS_ERROR_RESPONSE:
      {
        os << "ADDRESS_ERROR_RESPONSE";
        break;
      }

    case tlm::TLM_COMMAND_ERROR_RESPONSE:
      {
        os << "COMMAND_ERROR_RESPONSE";
        break;
      }

    case tlm::TLM_BURST_ERROR_RESPONSE :

      {
        os << "BURST_ERROR_RESPONSE";
        break;
      }
    case tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE:
      {
        os << "BYTE_ENABLE_ERROR_RESPONSE";
        break;
      }

    default:
      {
        os << "UNKNOWN_" << status;
        break;
      }
  }//end switch
  return os.str();
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
