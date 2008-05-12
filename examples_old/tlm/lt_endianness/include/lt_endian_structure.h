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
 @file lt_endian_structure.h
 
 @brief endian read/write structure header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_ENDIAN_STRUCTURE_H__
#define __LT_ENDIAN_STRUCTURE_H__

#include "tlm.h"                                    ///< TLM headers

const unsigned int  buffer_size = 16;               ///< buffer size

union transaction_structure
{
  unsigned char   buffer [ buffer_size ];           ///< character buffer
  
  struct
  {
    sc_dt::uint64   d_int64u;                       ///< int64u
    unsigned long   d_int32u;                       ///< int32u
    unsigned short  d_int16u;                       ///< int16u
    unsigned char   d_int8u_1;                      ///< int8u 
    unsigned char   d_int8u_2;                      ///< int8u
  }
                  data;                             ///< structured data
};

#endif /* __LT_ENDIAN_STRUCTURE_H__ */
