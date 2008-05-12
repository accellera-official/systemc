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

/*=============================================================================
  @file rw_bus.cpp
  
  @brief Implementation of rw_bus
=============================================================================*/
#include <sstream>

/*==============================================================================
  Constructor
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::rw_bus(sc_module_name name) : 
	sc_module(name)
{		
	for (int i=0; i<NR_OF_INITIATORS; i++)
	{		
		std::stringstream ss;
		
		ss << "initiator_socket_" << i;
				
		initiator_socket[i] = new rw_initiator_socket(ss.str().c_str());
		
		initiator_socket[i]->bind(*this);
	}
	
	for (int i=0; i<NR_OF_TARGETS; i++)
	{
		std::stringstream ss;
			
		ss << "target_socket_" << i;
			
		target_socket[i] = new rw_target_socket(ss.str().c_str());
		
		target_socket[i]->bind(*this);
	}
}

/*==============================================================================
  Destructor
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::~rw_bus()
{
	for (int i=0; i<NR_OF_INITIATORS; i++)
	{			
		delete initiator_socket[i];
	}
		
	for (int i=0; i<NR_OF_TARGETS; i++)
	{
		delete target_socket[i];
	}
}

/*==============================================================================
  Implementation of routing in blocking transport
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
void rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::b_transport(tlm::tlm_generic_payload & trans)
{	
	unsigned int initiatorSocketId = getSocketId(trans.get_address());
	
	if (initiatorSocketId >= NR_OF_INITIATORS)
	{
		cerr << name() << ": ERROR, no socketId corresponding to address 0x";
		cerr << hex << trans.get_address() << endl;
	}
		
	trans.set_address(trans.get_address() - getAddressOffset(initiatorSocketId));
	
	(*initiator_socket[initiatorSocketId])->b_transport(trans);
}

/*==============================================================================
  Implementation of routing in transport debug
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
unsigned int rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::transport_dbg(tlm::tlm_generic_payload & payload)
{
	unsigned int initiatorSocketId = getSocketId(payload.get_address());
		
	if (initiatorSocketId >= NR_OF_INITIATORS)
	{
		cerr << name() << ": ERROR, no socketId corresponding to address 0x";
		cerr << hex << payload.get_address() << endl;
	}
		
	payload.set_address( payload.get_address() - getAddressOffset(initiatorSocketId) );
	
	return (*initiator_socket[initiatorSocketId])->transport_dbg(payload);
}

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
bool rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::get_direct_mem_ptr(tlm::tlm_generic_payload  & trans, 
                                                                 tlm::tlm_dmi         & dmi_data)
{
	unsigned int initiatorSocketId = getSocketId(trans.get_address());
			
	if (initiatorSocketId >= NR_OF_INITIATORS)
	{
		cerr << name() << ": ERROR, no socketId corresponding to address 0x";
		cerr << hex << trans.get_address() << endl;
	}
		
	sc_dt::uint64 target_address = (trans.get_address() - getAddressOffset(initiatorSocketId));
	
	trans.set_address(target_address);
	
	bool result = (*initiator_socket[initiatorSocketId])->get_direct_mem_ptr(trans, dmi_data);
	
	sc_dt::uint64 start, end;
	start = dmi_data.get_start_address();
	end = dmi_data.get_end_address();
	
	limitRange(initiatorSocketId, start, end);
	
	dmi_data.set_start_address(start);
	dmi_data.set_end_address(end);
	
	return result;
}

/*==============================================================================
  Implementation of routing for backward DMI interface
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
void rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                                                        sc_dt::uint64 end_range)
{
	// not implemented for now
}
 
  
/*==============================================================================
  Decoding: target depends on bits 31-24 of the address
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
unsigned int rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::getSocketId(const sc_dt::uint64& address)
{
	return (unsigned int)address >> 29;
}

/*==============================================================================
  Returns the base address of the corresponding target 
  (following the decoding rule above)
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
sc_dt::uint64 rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::getAddressOffset(unsigned int socketId)
{
	return socketId << 29;
}

/*==============================================================================
  Returns the mask address of the corresponding target
  (following the decoding rule above)
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
sc_dt::uint64 rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::getAddressMask(unsigned int socketId)
{
	return 0xfffffff;
}

/*==============================================================================
  Utility function to limit range of address to the one of a specific target
==============================================================================*/

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
void rw_bus<NR_OF_INITIATORS, NR_OF_TARGETS>::limitRange(unsigned int    socketId, 
		                                                 sc_dt::uint64 & low, 
		                                                 sc_dt::uint64 & high)
{
	sc_dt::uint64 addressOffset = getAddressOffset(socketId);
	sc_dt::uint64 addressMask = getAddressMask(socketId);

	if ((low > addressMask) || (low > high))
	{
		// invalid region, we clip to the region corresponding
		// to the whole target address space
		low = addressOffset;
		high = addressOffset + addressMask;	  
	}	
	else
	{
		low += addressOffset;
		if (high > addressMask) 
		{
			high = addressOffset + addressMask;
		} 
		else
		{
			high += addressOffset;
		}
	}
}
