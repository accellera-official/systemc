/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef _TLM_TYPE_H_
#define _TLM_TYPE_H_

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
//#include "systemc.h"

  /** \defgroup byte_length_function Helper function: returns the number of byte represented by a datatype
   * @{ 
   */
  
  //-------------------------------------------------   
  /// Computes the number of byte of DATA type 
  template<typename DATA>
  inline unsigned int get_nb_byte() {
    return(sizeof(DATA));      
  }
  
  // Computes the number of byte of DATA type inlined specializations
  template<>
  inline unsigned int get_nb_byte<sc_lv<32> >() {
    return(4);
  }
  
  template<>
  inline unsigned int get_nb_byte<sc_lv<64> >() {
    return(8);
  }
  
  template<>
  inline unsigned int get_nb_byte<sc_lv<128> >() {
    return(16);
  }

  template<>
  inline unsigned int get_nb_byte<sc_uint<32> >() {
    return(4);
  }

  template<>
  inline unsigned int get_nb_byte<sc_uint<64> >() {
    return(8);
  }
  
  template<>
  inline unsigned int get_nb_byte<sc_biguint<32> >() {
    return(4);
  }
  template<>
  inline unsigned int get_nb_byte<sc_biguint<64> >() {
    return(8);
  }

  template<>
  inline unsigned int get_nb_byte<sc_biguint<128> >() {
    return(16);
  }

  /* @} */

  /**
   * \defgroup tlm_endianness TLM endianness type
   * Defines the two endianness.
   *  @{ 
   **/
  enum tlm_endianness {
    TLM_LITTLE_ENDIAN,
    TLM_BIG_ENDIAN
  };
  /* @} */


  /** \defgroup data_mode tlm_bus_request and tlm_bus_reponse mode
   * request and response data (and byte_enable array) management
   *  
   * 
   * @{
   **/
  enum tlm_data_mode {
    TLM_PASS_BY_COPY,    ///< The data (and byte_enable array ) are passed by effective copy.
    TLM_PASS_BY_POINTER  ///< The data (and byte_enable array ) are passed using the pointer member copy only
  };

  /* @} */


/** Host arch. identification and associated typedef and define.
 **/

#if defined(__sparc) || defined(macintosh) || defined(__hppa)

/* Workstation endianness */
#define HOST_ENDIAN_BIG

/* SOC Endianness default value = host endianness */
#define TLM_HOST_ENDIAN tlm_bus::TLM_BIG_ENDIAN 

#elif defined(__acorn) || defined(__mvs) || defined(_WIN32) || \
  (defined(__alpha) && defined(__osf__)) || defined(__i386)

/* Workstation endianness */
#define HOST_ENDIAN_LITTLE

/* SOC Endianness default value = host endianness */
#define TLM_HOST_ENDIAN tlm_bus::TLM_LITTLE_ENDIAN

#else
/* Workstation endianness */
#define HOST_ENDIAN_UNKNOWN

#endif



/**
 * \defgroup tlm_type common type definition
 *  @{ 
 **/
#if defined(_WIN32) && defined(_MSC_VER)

typedef unsigned __int64 tlm_uint64_t;
typedef unsigned __int32 tlm_uint32_t;
typedef unsigned __int16 tlm_uint16_t;
typedef unsigned __int8  tlm_uint8_t;

typedef __int64  tlm_sint64_t;
typedef __int32  tlm_sint32_t;
typedef __int16  tlm_sint16_t;
typedef __int8   tlm_sint8_t;

#else

typedef unsigned long long tlm_uint64_t;
typedef unsigned long tlm_uint32_t;
typedef unsigned short tlm_uint16_t;
typedef unsigned char tlm_uint8_t;

typedef long long  tlm_sint64_t;
typedef long  tlm_sint32_t;
typedef short tlm_sint16_t;
typedef char  tlm_sint8_t;

#endif

/* @} */

#endif /* _TLM_TYPE_H_ */


