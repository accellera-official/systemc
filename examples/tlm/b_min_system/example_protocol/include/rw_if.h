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
 * rw_if.h
 * $Id:$
 * @brief
 * Interface to provide access to read and write methods.
 * @Details
 * Interface to provide access to read and write methods.
 * This interface is used by initiators and implemented by targets
 * to perform transactions.
 * 
 ********************************************************************************/
#ifndef __RW_IF_H__
#define __RW_IF_H__

#include "systemc.h"

class rw_if
{
  public:
    /// read() - reads a transaction from the port and places the result
    /// in the read buffer.
	/// returns true if there are no errors, else false
    virtual bool read(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    read_buffer     ///< pointer to read buffer
      ) = 0;                              ///< pure virtual function an interface


    /// write() - writes a transaction to the port using the data in the
    /// write buffer.
    /// returns true if there are no errors, else false
    virtual bool write(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    write_buffer    ///< pointer to write buffer
       ) = 0;                             ///< pure virtual function an interface 
};

#endif /* __RW_IF_H__ */
