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
  class pool_queue_c                                      /// memory pool queue class
  : public tlm::tlm_mm_interface                          /// implements memory management IF
  {
    public:

    pool_queue_c() {
      m_size = m_initial_size;
      m_available = 0;
      m_queue = new tlm::tlm_generic_payload* [m_size];
    }

    ~pool_queue_c() {
      for(unsigned i=0; i<m_available; i++) delete m_queue[i];
      delete [] m_queue;
    }

    tlm::tlm_generic_payload *                            /// transaction pointer
    pop                                                   /// get a payload object
    ( void
    )
    {
      tlm::tlm_generic_payload *transaction_ptr;
      if(m_available == 0) {
        transaction_ptr = new tlm::tlm_generic_payload(this);
      } else {
        transaction_ptr = m_queue[--m_available];
      }
      transaction_ptr->acquire();
      return transaction_ptr;
    }

    void
    push                                                  /// give back a payload object
    ( tlm::tlm_generic_payload *transaction_ptr           /// transaction pointer
    )
    {
      transaction_ptr->release();
    }

    void
    free                                                  /// callback on transaction completion
    ( tlm::tlm_generic_payload *transaction_ptr           /// transaction pointer
    )
    {
      transaction_ptr->reset();
      if(m_available == m_size) {
        // queue is full.  make a new one twice as big
        m_size *= 2;
        tlm::tlm_generic_payload **new_queue = new tlm::tlm_generic_payload* [m_size];
        for(unsigned i=0; i<m_available; i++) new_queue[i] = m_queue[i];
        delete [] m_queue;
        m_queue = new_queue;
      }
      m_queue[m_available++] = transaction_ptr;
    }

    private:

    tlm::tlm_generic_payload **m_queue;      /// queue
    unsigned m_size, m_available;
    static const unsigned m_initial_size = 1;
  };

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

  pool_queue_c        m_txn_pool;                   // transaction pool

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
