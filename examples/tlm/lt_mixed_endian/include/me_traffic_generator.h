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
/// @file me_traffic_generator.h
//
/// @brief traffic_generator class header
//
//=====================================================================
//  Authors:
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Jack Donovan, ESLX
//====================================================================

#ifndef __ME_TRAFFIC_GENERATOR_H__
#define __ME_TRAFFIC_GENERATOR_H__

#include "tlm.h"                              	// TLM headers
#include <queue>                                // queue header from std lib
#include <list>

class me_traffic_generator                       	// traffic_generator
: public sc_core::sc_module                    	// sc_module
{

// Member Methods  ====================================================

  public:
  //=====================================================================
  ///  @fn me_traffic_generator
  //
  ///  @brief me_traffic_generator constructor
  //
  ///  @details
  ///    Initializes Traffic Generator
  //
  //=====================================================================

  me_traffic_generator
  ( sc_core::sc_module_name name            	  ///< module name for SC
  , const unsigned int      ID               	  ///< initiator ID
  , sc_dt::uint64           base_address_1    	///< first base address
  , sc_dt::uint64           base_address_2    	///< second base address
  );


  //=============================================================================
  ///  @fn me_traffic_generator_thread
  //
  ///  @brief traffic_generator processing thread
  //
  ///  @details
  ///    Method actually called by SC simulator to generate and
  ///    check traffic. Generate Writes then Reads to check the
  ///    Writes
  //
  //=============================================================================

  void
  me_traffic_generator_thread
  ( void
  );

  // Transaction Pool (queue)
  class pool_c : public tlm::tlm_mm_interface {
  public:
    pool_c() {
      m_size = m_initial_size;
      m_available = 0;
      m_stack = new tlm::tlm_generic_payload* [m_size];
    }

    ~pool_c() {
      for(std::list<tlm::tlm_generic_payload *>::iterator
        i = all_payloads.begin(); i != all_payloads.end(); i++) {
        delete *i;
      }
      delete [] m_stack;
    }

    tlm::tlm_generic_payload *pop() {
      tlm::tlm_generic_payload *transaction_ptr;
      if(m_available == 0) {
        transaction_ptr = new tlm::tlm_generic_payload(this);
        all_payloads.push_back(transaction_ptr);
      } else {
        transaction_ptr = m_stack[--m_available];
      }
      transaction_ptr->acquire();
      return transaction_ptr;
    }

    void push(tlm::tlm_generic_payload *transaction_ptr) {
      transaction_ptr->release();
    }

    void free (tlm::tlm_generic_payload *transaction_ptr) {
      transaction_ptr->reset();
      if(m_available == m_size) {
        // stack is full.  make a new one twice as big
        m_size *= 2;
        tlm::tlm_generic_payload
          **new_stack = new tlm::tlm_generic_payload* [m_size];
        for(unsigned i=0; i<m_available; i++) new_stack[i] = m_stack[i];
        delete [] m_stack;
        m_stack = new_stack;
      }
      m_stack[m_available++] = transaction_ptr;
    }

  private:
    tlm::tlm_generic_payload **m_stack;   /// stack of available payloads
    std::list<tlm::tlm_generic_payload *> all_payloads;
    unsigned m_size, m_available;
    static const unsigned m_initial_size = 1;
  };  // pool_c class definition

  private:
    void do_transaction(std::string &user_command);
    void do_load(std::istringstream &iss);
    void do_store(std::istringstream &iss);
    template<typename T> void do_do_load(std::istringstream &iss);
    template<typename T> void do_do_store(std::istringstream &iss);


//=============================================================================
// Member Variables

  private:

  const unsigned int  m_ID;                   	    // initiator ID
  std::string m_prompt;

  sc_dt::uint64       m_base_address_1;      	      // first base address
  sc_dt::uint64       m_base_address_2;       	    // second base address

  pool_c        m_txn_pool;                   // transaction pool

  tlm::tlm_endianness m_endianness, m_host_endianness;

  static const int m_buffer_size = 1024;
  unsigned char m_buffer[m_buffer_size];

  /// Synchronisation between mixed-endian traffic generators
  static std::list<me_traffic_generator *> me_ui_waiters;
  static sc_core::sc_event me_ui_change_event;

  public:

  /// Port for requests to the initiator
  sc_core::sc_port<sc_core::sc_fifo_out_if <tlm::tlm_generic_payload *> > request_out_port;

  /// Port for responses from the initiator
  sc_core::sc_port<sc_core::sc_fifo_in_if  <tlm::tlm_generic_payload *> > response_in_port;
};
#endif /* __ME_TRAFFIC_GENERATOR_H__ */
