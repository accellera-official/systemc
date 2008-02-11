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

#include "ut_initiator.h"                             ///< our header file
#include "reporting.h"                                ///< reporting output
#include "mtrand.h"                                   ///< random number generation

static const char  *filename = "ut_initiator.cpp";    ///< filename string

/* ---------------------------------------------------------------------------------------
 @file ut_initiator.cpp
 
 @brief UT initiator
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

/*=============================================================================
  @fn ut_initiator::ut_initiator
  
  @brief ut_initiator constructor
  
  @details
    This routine initialized a ut_initiator class instance.
    
  @param name module name
  @param ID   initiator ID
  @param seed random number generator seed
  
  @retval none
=============================================================================*/
ut_initiator::ut_initiator                            ///< constructor
( sc_core::sc_module_name name                        ///< module name
, const unsigned int      ID                          ///< initiator ID
, const unsigned int      seed                        ///< random number generation seed
)
: sc_core::sc_module  (name)                          ///< module name
, socket              ("ut_initiator_socket")         ///< socket
, m_ID                (ID)                            ///< initiator ID
, m_seed              (seed)                          ///< random number generation seed
, mBaseAddress        (0x00000000)                    ///< base address
, mNrOfTransactions   (5)                             ///< transaction count
, mTransactionCount   (0)                             ///< transaction counter
{
  // allocate our data array
  m_data = new unsigned char[8];
  
  // Bind this initiator's interface to the initiator socket
  socket(*this);
  
  // Initiator thread
  SC_THREAD(initiator_thread);
}

/*=============================================================================
  @fn ut_initiator::~ut_initiator
  
  @brief ut_initiator destructor
  
  @details
    This routine terminates an ut_initiator class instance.
    
  @param void
  
  @retval none
=============================================================================*/
ut_initiator::~ut_initiator                           ///< destructor
( void
)
{
  // release our data array
  delete [] m_data;
}

/*=============================================================================
  @fn ut_initiator::setup_transaction
  
  @brief ut_initiator transaction setup
  
  @details
    This routine sets up a ut_initiator transaction.
    
  @param trans  transaction
  
  @retval bool  command generated
=============================================================================*/
bool                                                  ///< command generated true/false
ut_initiator::setup_transaction                       ///< setup_transaction
( tlm::tlm_generic_payload  &trans                    ///< transaction
)
{
  bool  generated_command = true;                     ///< command generated flag

  // set random generator seed
  const unsigned long length  = 4;
  const unsigned long init[4] = { ( m_seed >>  0 ) & 0x000000FF
                                , ( m_seed >>  8 ) & 0x000000FF
                                , ( m_seed >> 16 ) & 0x000000FF
                                , ( m_seed >> 24 ) & 0x000000FF
                                };
  
  static MTRand_int32 irand(init, length);             ///< 32-bit integer random number generator

  // setup the address and data
  trans.set_address   ((irand() % 4096) | ((irand() % 2) ? 0x10000000 : 0x00000000));
  trans.set_data_ptr  (m_data);
  
  // generate N writes, N reads, then nothing
  if (mTransactionCount < mNrOfTransactions)
  {
    // setup the data array
    for (unsigned int i = 0; i < 8; i++)
    {
      m_data [ i ] = (unsigned char) mTransactionCount;
    }
    
    trans.set_command(tlm::TLM_WRITE_COMMAND);
  }
  else if (mTransactionCount < 2 * mNrOfTransactions)
  {
    // clear data array before read
    memset (m_data, 0, 8);
    
    trans.set_command (tlm::TLM_READ_COMMAND);
  }
  else
  {
    generated_command = false;
  }

  mTransactionCount++;
  
  return generated_command;
}

/*=============================================================================
  @fn ut_initiator::report_start
  
  @brief ut_initiator transaction start report
  
  @details
    This routine reports the start of a ut_initiator transaction.
    
  @param trans  transaction
  
  @retval none
=============================================================================*/
void
ut_initiator::report_start                            ///< report_start
( tlm::tlm_generic_payload  &trans                    ///< transaction
)
{
  sc_dt::uint64       address = trans.get_address();   ///< address
  std::ostringstream  msg;                             ///< log message
  
  switch (trans.get_command())
  {
    case tlm::TLM_WRITE_COMMAND:
    {
      msg.str ("");
      msg << "W - " << m_ID << " -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << trans.get_data_length();
      msg << " D: 0x";
      for (unsigned int i = 0; i < trans.get_data_length (); i++)
      {
        msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)((trans.get_data_ptr())[i]);
      }
      
      REPORT_INFO(filename, __FUNCTION__, msg.str());

      break;
    }
  
    case tlm::TLM_READ_COMMAND:
    {
      msg.str ("");
      msg << "R - " << m_ID << " -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << trans.get_data_length();

      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      break;
    }
    
    default:
    {
      break;
    }
  }
}

/*=============================================================================
  @fn ut_initiator::report_end
  
  @brief ut_initiator transaction end report
  
  @details
    This routine reports the end of a ut_initiator transaction.
    
  @param trans  transaction
  
  @retval none
=============================================================================*/
void
ut_initiator::report_end                              ///< report_end
( tlm::tlm_generic_payload  &trans                    ///< transaction
)
{
  std::ostringstream msg;                             ///< log message

  switch (trans.get_response_status())
  {
    default:
    {
      msg.str ("");
      msg << m_ID << ": error response received";

      REPORT_INFO(filename, __FUNCTION__, msg.str());

      break;
    }
    
    case tlm::TLM_OK_RESPONSE:
    {
      msg.str ("");
      msg << m_ID << ": TLM_OK_RESPONSE received";
      
      if (trans.get_command() == tlm::TLM_READ_COMMAND)
      {
        msg << " D: 0x";
        
        for (unsigned int i = 0; i < trans.get_data_length (); i++)
        {
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)((trans.get_data_ptr())[i]);
        }
      }
      
      REPORT_INFO(filename, __FUNCTION__, msg.str());

      break;
    }
  }
}

/*=============================================================================
  @fn ut_initiator::initiator_thread
  
  @brief ut_initiator processing thread
  
  @details
    This routine handles the ut_initiator processing thread.
    
  @param none
  
  @retval none
=============================================================================*/
void
ut_initiator::initiator_thread                        ///< initiator thread
( void
)
{
  tlm::tlm_generic_payload trans;                     ///< transaction

  while (setup_transaction(trans))
  {
    report_start(trans);
    
    socket->b_transport(trans);
    
    report_end(trans);
  }
}

/*=============================================================================
  @fn ut_initiator::invalidate_direct_mem_ptr
  
  @brief invalidate DMI pointer
  
  @details
    This routine invalidates a DMI pointer.
    
  @note
    not implemented
    
  @param start_range  invalidation range start
  @param end_range    invalidation range end
  
  @retval none
=============================================================================*/
void
ut_initiator::invalidate_direct_mem_ptr               ///< invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                           ///< invalidate range start
, sc_dt::uint64 end_range                             ///< invalidate range end
)
{
  REPORT_INFO(filename, __FUNCTION__, "not implemented");
}
