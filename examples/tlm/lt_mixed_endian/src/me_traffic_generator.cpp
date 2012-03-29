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
/// @file me_traffic_generator.cpp
//
/// @brief traffic generation routines
//
//=====================================================================
//  Authors:
//    Bill Bunton, ESLX
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//====================================================================



#include "reporting.h"                // reporting macros
#include "me_traffic_generator.h"          // traffic generator declarations

#ifdef _MSC_VER
  #if (_MSC_VER < 1300)
    typedef unsigned char     uint8_t;
    typedef unsigned short    uint16_t;
    typedef unsigned int      uint32_t;
  #else
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
  #endif
#elif defined( __sparc ) || defined( __sparc__ )
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned int      uint32_t;
#else
  #include <stdint.h>
#endif

using namespace std;

static const char *filename = "me_traffic_generator.cpp";  ///< filename for reporting

/// Constructor

SC_HAS_PROCESS(me_traffic_generator);
//-----------------------------------------------------------------------------
//
me_traffic_generator::me_traffic_generator            // constructor
( sc_core::sc_module_name name                  // instance name
, const unsigned int    ID                      // initiator ID
, sc_dt::uint64         base_address_1          // first base address
, sc_dt::uint64         base_address_2          // second base address
)
: sc_module           ( name            )       /// module name
, m_ID                ( ID              )       /// initiator ID
, m_base_address_1    ( base_address_1  )       /// first base address
, m_base_address_2    ( base_address_2  )       /// second base address
{
  SC_THREAD(me_traffic_generator_thread);
  me_ui_waiters.push_back(this);
  // odd IDs are big endian, even are little
  m_endianness = ((m_ID & 1) == 0 ? tlm::TLM_LITTLE_ENDIAN : tlm::TLM_BIG_ENDIAN);
  // host endianness
  m_host_endianness = tlm::get_host_endianness();
  std::ostringstream  tmp;
  tmp << m_ID << (m_endianness == tlm::TLM_LITTLE_ENDIAN ? " LE" : " BE") << "32 ";
  m_prompt = tmp.str();
}


/// SystemC thread for generation of GP traffic

//-----------------------------------------------------------------------------
//
void
me_traffic_generator::me_traffic_generator_thread
( void
 )
{
  std::ostringstream  msg;                      ///< log message
  msg.str ("");
  msg << "Initiator: " << m_ID << " Starting Traffic";
  REPORT_INFO(filename, __FUNCTION__, msg.str());

  bool running = true;
  while(running) {
    // traffic generator should own the user interface
    while(me_ui_waiters.front() != this) wait(me_ui_change_event);

    // get user input
    std::cout << m_prompt << " > ";
    std::string user_command;
    std::getline(std::cin, user_command);
    std::cout << std::endl;
    if(std::cin.bad() || std::cin.fail() || std::cin.bad()) user_command = string("q");

    // parse for control flow
    if(user_command.length() != 0) switch(user_command[0]) {
    case 'l':  // load
    case 's':  // store
      do_transaction(user_command);
      break;

    case 'w':  // wait
      me_ui_waiters.pop_front();
      me_ui_waiters.push_back(this);
      me_ui_change_event.notify();
      break;

    case 'q':  // quit
      me_ui_waiters.pop_front();
      me_ui_change_event.notify();
      running = false;
      break;

    case 'm':
      std::cout << "Memory map:\n";
      std::cout << "  " << hex << m_base_address_1 << " (16)" << dec << endl;
      std::cout << "  " << hex << m_base_address_2 << " (16)" << dec << endl;
      break;

    case 'h':   // help
    case 'H':   // help
    case '?':   // help
      std::cout << "enter one of the following commands (all numbers in hex):\n";
      std::cout << "  l8  addr nr_bytes          (load)\n";
      std::cout << "  l16 addr nr_halfwords      (load)\n";
      std::cout << "  l32 addr nr_words          (load)\n";
      std::cout << "  s8  addr d0 d1 ...         (store)\n";
      std::cout << "  s16 addr d0 d1 ...         (store)\n";
      std::cout << "  s32 addr d0 d1 ...         (store)\n";
      std::cout << "  m                          (memory map)\n";
      std::cout << "  w                          (wait/switch initiator)\n";
      std::cout << "  q                          (quit)\n";
      break;
    }
  }

  msg.str ("");
  msg << "Traffic Generator : " << m_ID << endl
  << "=========================================================" << endl
  << "            ####  Traffic Generator Complete  #### ";
  REPORT_INFO(filename, __FUNCTION__, msg.str());

} // end me_traffic_generator_thread


/// Synchronisation between mixed-endian traffic generators
std::list<me_traffic_generator *> me_traffic_generator::me_ui_waiters;
sc_core::sc_event me_traffic_generator::me_ui_change_event;


/// Helper functions
void me_traffic_generator::do_transaction(std::string &user_command) {
  std::cout << m_prompt << ":: cmd: " << user_command << std::endl;
  std::istringstream iss(user_command);

  char ldst;
  iss >> ldst;

  if(ldst == 'l') do_load(iss);
  else do_store(iss);
}

void me_traffic_generator::do_load(std::istringstream &iss) {
  int datatype;
  iss >> datatype;

  switch(datatype) {
  case 8:
    do_do_load<uint8_t>(iss);
    break;
  case 16:
    do_do_load<uint16_t>(iss);
    break;
  case 32:
  default:
    do_do_load<uint32_t>(iss);
    break;
  }
}

void me_traffic_generator::do_store(std::istringstream &iss) {
  int datatype;
  iss >> datatype;

  switch(datatype) {
  case 8:
    do_do_store<uint8_t>(iss);
    break;
  case 16:
    do_do_store<uint16_t>(iss);
    break;
  case 32:
  default:
    do_do_store<uint32_t>(iss);
    break;
  }
}

template<typename T> void me_traffic_generator::do_do_load(std::istringstream &iss) {
  tlm::tlm_generic_payload  *req_transaction_ptr = m_txn_pool.pop();

  sc_dt::uint64 addr;
  iss >> hex >> addr >> dec;
  int len;
  iss >> hex >> len >> dec;
  if(int(len * sizeof(T)) > m_buffer_size) return;

  req_transaction_ptr->set_command(tlm::TLM_READ_COMMAND);
  req_transaction_ptr->set_address(addr);
  req_transaction_ptr->set_data_ptr(m_buffer);
  req_transaction_ptr->set_data_length(len * sizeof(T));
  req_transaction_ptr->set_streaming_width(len * sizeof(T));
  req_transaction_ptr->set_byte_enable_ptr(0);
  req_transaction_ptr->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  if(m_endianness != m_host_endianness)
    tlm::tlm_to_hostendian_word<T>(req_transaction_ptr,4);

  request_out_port->write(req_transaction_ptr);
  tlm::tlm_generic_payload  *resp_transaction_ptr = response_in_port->read();

  if (resp_transaction_ptr != req_transaction_ptr) {
    std::ostringstream msg;
    msg << m_ID << "Response to wrong request";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }
  if (resp_transaction_ptr->get_response_status() != tlm::TLM_OK_RESPONSE) {
    std::ostringstream msg;
    msg << m_ID << "Transaction ERROR";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }

  if(m_endianness != m_host_endianness)
    tlm::tlm_from_hostendian_word<T>(req_transaction_ptr,4);

  std::cout << m_prompt << ":: " << "read transaction length " << len
            << " (10) x " << sizeof(T)*8 << "-bit completed, returning:" << std::endl;

  T *buffer = (T *)m_buffer;
  std::cout << m_prompt << "::";
  for(int i=0; i<len; i++) {
    std::cout << " " << hex << unsigned(*(buffer++));
  }
  std::cout << dec << std::endl;

  m_txn_pool.push(resp_transaction_ptr);
}

template<typename T> void me_traffic_generator::do_do_store(std::istringstream &iss) {
  tlm::tlm_generic_payload  *req_transaction_ptr = m_txn_pool.pop();

  sc_dt::uint64 addr;
  iss >> hex >> addr >> dec;

  T *buffer = (T *)m_buffer;
  int len;
  for(len=0; len < m_buffer_size; len++) {
    unsigned tmp;
    iss >> hex >> tmp >> dec;
    if(iss.fail()) break;
    *(buffer++) = T(tmp);
  }
  if(len == 0) return;

  req_transaction_ptr->set_command(tlm::TLM_WRITE_COMMAND);
  req_transaction_ptr->set_address(addr);
  req_transaction_ptr->set_data_ptr(m_buffer);
  req_transaction_ptr->set_data_length(len * sizeof(T));
  req_transaction_ptr->set_streaming_width(len * sizeof(T));
  req_transaction_ptr->set_byte_enable_ptr(0);
  req_transaction_ptr->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  if(m_endianness != m_host_endianness)
    tlm::tlm_to_hostendian_word<T>(req_transaction_ptr,4);

  request_out_port->write(req_transaction_ptr);
  tlm::tlm_generic_payload  *resp_transaction_ptr = response_in_port->read();

  if (resp_transaction_ptr != req_transaction_ptr) {
    std::ostringstream msg;
    msg << m_ID << "Response to wrong request";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }
  if (resp_transaction_ptr->get_response_status() != tlm::TLM_OK_RESPONSE) {
    std::ostringstream msg;
    msg << m_ID << "Transaction ERROR";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }

  if(m_endianness != m_host_endianness)
    tlm::tlm_from_hostendian_word<T>(req_transaction_ptr,4);

  std::cout << m_prompt << ":: " << "write transaction length " << len
            << " (10) x " << sizeof(T)*8 << "-bit completed" << std::endl;

  m_txn_pool.push(resp_transaction_ptr);
}

