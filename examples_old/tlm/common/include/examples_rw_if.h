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
/*******************************************************************************
 * @file
 * examples_rw_if.h
 * $Id:$
 * @brief
 * Interface to provide access to read and write methods from the TLM socket.
 * @Details
 * Interface to provide access to read and write methods from the TLM socket.
 * This interface must be implemented by an initiator to support reading and
 * writing from/to the socket by another module.
 * 
 ********************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __EXAMPLES_RW_IF_H__
#define __EXAMPLES_RW_IF_H__

#include "tlm.h"                                      ///< TLM headers

class examples_rw_if : virtual public sc_core::sc_interface {
  public:
    /// read() - reads a transaction from the socket and places the result
    /// in the read buffer.
    virtual bool read(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    read_buffer     ///< pointer to read buffer
      ) = 0;                              ///< pure virtual function an interface


    /// write() - writes a transaction to the socket using the data in the
    /// write buffer.
    virtual bool write(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    write_buffer    ///< pointer to write buffer
       ) = 0;                             ///< pure virtual function an interface 
};
#endif /* __EXAMPLES_RW_IF_H__ */
