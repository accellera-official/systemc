/*****************************************************************************

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

 *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file tlm_helpers.h
 
 @brief
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __TLM_HELPERS_H__
#define __TLM_HELPERS_H__

//#include <sys/param.h>
//#include <cstring>

namespace tlm {

enum tlm_endianness { TLM_UNKNOWN_ENDIAN, TLM_LITTLE_ENDIAN, TLM_BIG_ENDIAN };

inline tlm_endianness get_host_endianness(void)
{
  static tlm_endianness host_endianness = TLM_UNKNOWN_ENDIAN;
  
  if (host_endianness == TLM_UNKNOWN_ENDIAN) {
    unsigned int number = 1;
    unsigned char *p_msb_or_lsb = (unsigned char*)&number;

    host_endianness = (p_msb_or_lsb[0] == 0) ? TLM_BIG_ENDIAN : TLM_LITTLE_ENDIAN;
  }
  return host_endianness;
}

inline bool host_has_little_endianness(void)
{
  static tlm_endianness host_endianness = TLM_UNKNOWN_ENDIAN;
  static bool host_little_endian = false;
  
  if (host_endianness == TLM_UNKNOWN_ENDIAN) {
    unsigned int number = 1;
    unsigned char *p_msb_or_lsb = (unsigned char*)&number;

    host_little_endian = (p_msb_or_lsb[0] == 0) ? false : true;
  }

  return host_little_endian;
}

inline bool has_host_endianness(tlm_endianness endianness)
{
  if (host_has_little_endianness()) {
    return endianness == TLM_LITTLE_ENDIAN;

  } else {
    return endianness == TLM_BIG_ENDIAN;
  }
} 

} // namespace tlm

#endif /* __TLM_HELPERS_H__ */
