/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file get_host_endianness.h
 
 @brief get_host_endianness helper routine header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "tlm.h"                                    ///< TLM headers
#include "get_host_endianness.h"                    ///< our header

/*=============================================================================
  @fn get_host_endianness
  
  @brief determine host endianness
  
  @details
    This routine determines the host's endianness.
    
  @param none
  
  @retval tlm_endianness
=============================================================================*/

tlm::tlm_endianness                                 ///< endianness
get_host_endianness                                 ///< get_host_endianness
( void
)
{
  static tlm::tlm_endianness host_endianness = tlm::TLM_UNKNOWN_ENDIAN;
  
  if ( host_endianness == tlm::TLM_UNKNOWN_ENDIAN )
  {
    const unsigned char   endian_array [ 2 ] = { 1, 0 };
  
          unsigned short  endian_short       = *(unsigned short *) endian_array;
  
    host_endianness = ( endian_short == 1 ) ? tlm::TLM_LITTLE_ENDIAN : tlm::TLM_BIG_ENDIAN;
  }

  return host_endianness;
}
