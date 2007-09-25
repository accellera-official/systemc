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


#ifndef _TLM_BYTE_ENABLE_H_
#define _TLM_BYTE_ENABLE_H_

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "tlm_type.h"


  /*------------------------------------------------------------------------------
   * Constants
   *----------------------------------------------------------------------------*/

  /** \defgroup byte_enable_values Predefined byte_enable values
   * Some predefined byte_enable values (up to 64 bits data)
   *@{
   **/

  /** This byte_enable value selects all bytes.
   * This value is used as default value.
   **/ 
  static const unsigned int NO_BE = 0xffffffff; 

  static const unsigned int BYTE_LANE_0 = 0x1;       ///< This byte_enable value selects the byte at word address + 0
  static const unsigned int BYTE_LANE_1 = 0x2;       ///< This byte_enable value selects the byte at word address + 1
  static const unsigned int BYTE_LANE_2 = 0x4;       ///< This byte_enable value selects the byte at word address + 2
  static const unsigned int BYTE_LANE_3 = 0x8;       ///< This byte_enable value selects the byte at word address + 3
  static const unsigned int BYTE_LANE_4 = 0x10;      ///< This byte_enable value selects the byte at word address + 4
  static const unsigned int BYTE_LANE_5 = 0x20;      ///< This byte_enable value selects the byte at word address + 5
  static const unsigned int BYTE_LANE_6 = 0x40;      ///< This byte_enable value selects the byte at word address + 6
  static const unsigned int BYTE_LANE_7 = 0x80;      ///< This byte_enable value selects the byte at word address + 7
  
  static const unsigned int SHORT_LANE_0 = 0x3;      ///< This byte_enable value selects the halfword (short) at word address + 0
  static const unsigned int SHORT_LANE_1 = 0xc;      ///< This byte_enable value selects the halfword (short) at word address + 2
  static const unsigned int SHORT_LANE_2 = 0x30;     ///< This byte_enable value selects the halfword (short) at word address + 4
  static const unsigned int SHORT_LANE_3 = 0xc0;     ///< This byte_enable value selects the halfword (short) at word address + 6
  
  static const unsigned int WORD_LANE_0 = 0x0f;      ///< This byte_enable value selects the word at word address + 0
  static const unsigned int WORD_LANE_1 = 0xf0;      ///< This byte_enable value selects the word at word address + 4
  /* @} */


  /*------------------------------------------------------------------------------
   * Functions
   *----------------------------------------------------------------------------*/

  /** \defgroup byte_enable_functions Byte enable related template functions
   * \n
   * \n The tlm protocol provides the following semantic for the byte enable capability. 
   * Consequently, all the IP models that are compatible with the tlm standard
   * should conform to the byte enable approach as described below, to ensure a 
   * correct interaction between models when integrated in system platforms. 
   * \n
   * \n  The rule to conform with is as follows (example with 64 bits data):
   - A byte is identified thanks to its address offset with respect to the word address, e.g:
   <table>
   <tr><td>byte location</td> <td> byte_enable value in TLM mode</td></tr>
   <tr><td>word_address + 0ffset 0</td><td>0x1</td></tr>
   <tr><td>word_address + 0ffset 1</td><td>0x2</td></tr>
   <tr><td>word_address + 0ffset 2</td><td>0x4</td></tr>
   <tr><td>word_address + 0ffset 3</td><td>0x8</td></tr>
   <tr><td>word_address + 0ffset 4</td><td>0x10</td></tr>
   <tr><td>word_address + 0ffset 5</td><td>0x20</td></tr>
   <tr><td>word_address + 0ffset 6</td><td>0x40</td></tr>
   <tr><td>word_address + 0ffset 7</td><td>0x80</td></tr>
   </table>
    
   A bit in the byte_enable denotes a valid byte. All combinations are allowed, e.g. byte_enable = 0x9. In 
   this case, the byte_enable attribute can interpreted as a byte mask. 
    
    
   For the simulated endianness:
   <table>
   <tr><td>Endianness</td><td>byte_enable value</td><td>Corresponding address offset</td><td>significance</td></tr>
   <tr><td> Little endian</td>               <td> 0x1 </td><td> address+0 </td><td> Less Significant Byte </td></tr>
   <tr> <td> </td>                       <td> 0x2 </td><td> address+1 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x4 </td><td> address+2 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x8 </td><td> address+3 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x10 </td><td> address+4 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x20 </td><td> address+5 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x40 </td><td> address+6 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x80 </td><td> address+7 </td><td> Most Significant Byte</td></tr>
    
   <tr><td> Big endian </td>          <td> 0x1 </td><td> address+0 </td><td> Most Significant Byte </td></tr>
   <tr> <td> </td>                       <td> 0x2 </td><td> address+1 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x4 </td><td> address+2 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x8 </td><td> address+3 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x10 </td><td> address+4 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x20 </td><td> address+5 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x40 </td><td> address+6 </td><td> </td></tr>
   <tr> <td> </td>                       <td> 0x80 </td><td> address+7 </td><td> Less Significant Byte</td></tr>
    
   </table>
   - As a consequence, master modules set the byte_enable value according to the required byte address offset.
   Slave modules that are byte-addressable must be aware of the simulated endianness, and 
   should be independent on the workstation endianness.
   \n The association byte_enable/byte significance is done according to the simulated endianness.
   The master might re-align the extracted byte on the LSB according to the simulated endianness if needed 
   (for word-based applications and byte usage).
    
   Note that for performance reasons, effective storage in the slave module might be done in the workstation endianness.
        
   Static template functions define the predefined legal values for the byte_enable
   attribute for the TLM protocol
   - be_word, be_short, be_byte    
   
   \n These template functions on addresss and data type calculate the byte enable value using the
   address as entry parameter

   * @{ 
   **/

  /// Calculates the minimum  "no byte enable" value for specified data type 
  template <typename DATA>
  inline unsigned int get_no_be() {
    return( (BYTE_LANE_0 << get_nb_byte<DATA>()) - 1);
  }

  /// Returns true if the given byte enable value is "no byte_enable" for the datatype bit length
  template <typename DATA>
  inline unsigned int is_no_be(unsigned int byte_enable) {
    unsigned int no_be = get_no_be<DATA>();
    return((byte_enable & no_be) == no_be);
  }


  /// Calculates the byte_enable for BYTE
  template <typename ADDRESS, typename DATA>
  inline unsigned int be_byte(ADDRESS addr) {
    return(BYTE_LANE_0  << ((addr) & (get_nb_byte<DATA>() - 1)));
  }

  /// Calculates the byte_enable for SHORT
  template <typename ADDRESS, typename DATA>
  inline unsigned int be_short(ADDRESS addr) {
    return(SHORT_LANE_0  << ((addr) & (get_nb_byte<DATA>() - 2)));
  }
  
  /// Calculates the byte_enable for WORD
  template <typename ADDRESS, typename DATA>
  inline unsigned int be_word(ADDRESS addr) {
    return(WORD_LANE_0  << ((addr) & (get_nb_byte<DATA>() - 4)));
  }

  // Calculates the byte_enable for BYTE (data type with WIDTH bit length)
  template <typename ADDRESS,int WIDTH>
  inline unsigned int be_byte(ADDRESS addr) {
    return(BYTE_LANE_0  << ((addr) & (WIDTH/8 - 1)));
  }

  // Calculates the byte_enable for SHORT (data type with WIDTH bit length)
  template <typename ADDRESS, int WIDTH>
  inline unsigned int be_short(ADDRESS addr) {
    return(SHORT_LANE_0  << ((addr) & (WIDTH/8 - 2)));
  }

  // Calculates the byte_enable for WORD (data type with WIDTH bit length)
  template <typename ADDRESS,int WIDTH>
  inline unsigned int be_word(ADDRESS addr) {
    return(WORD_LANE_0  << ((addr) & (WIDTH/8 - 4)));
  }

  /// Calculates the byte_enable for BYTE, SHORT and WORD using the size parameter (byte number)
  template <typename ADDRESS,typename DATA>
  inline unsigned int get_be(ADDRESS addr,unsigned int size) {
    if (size != get_nb_byte<DATA>()) {      
      switch(size) {
      case 1:
	return(BYTE_LANE_0 << ((addr) & (get_nb_byte<DATA>() - 1)));
	break;
      case 2:
	return(SHORT_LANE_0 << ((addr) & (get_nb_byte<DATA>() - 2)));
	break;
      case 4: 
	return(WORD_LANE_0 << ((addr) & (get_nb_byte<DATA>() - 4)));
	break;
      default:
	fprintf(stderr,"ERROR\t\tCan't compute byte_enable on a word with %d bytes\n",size);       
      }
    }
    return(NO_BE);
  }


  /* @} */


  /** \defgroup alignment_functions Data alignment related template functions
   * Template functions on addresss and data type which return aligned sub word data depending on 
   * the given address and simulated endianness
   * The first set of functions (align_xxx_to_tlm) is used to shift the data according to 
   * the endianness and the address before send it using the tlm_bus_request
   * The second set of functions (align_xxx_from_tlm) is used to shift back the data according to 
   * the endianness and the address after receiving it from the tlm_bus_response

   * @{ 
   **/
  /// Returns aligned byte value to write on tlm protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_byte_to_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 1);
    DATA tmp = data & 0xff;
    return(((endian==TLM_BIG_ENDIAN)?(tmp << ((~(address & addr_mask)) & addr_mask)*8):(tmp << (address & addr_mask)*8)));
  }

  /// Returns aligned short to write on TLM protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_short_to_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 2);
    DATA tmp = data & 0xffff;
    return(((endian==TLM_BIG_ENDIAN)?(tmp << ((~(address & addr_mask)) & addr_mask)*8):(tmp << (address & addr_mask)*8)));
  }

  /// Returns aligned word read from TLM protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_word_to_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 4);
    DATA tmp = data & 0xffffffff;
    return (((endian==TLM_BIG_ENDIAN)?(tmp << ((~(address & addr_mask)) & addr_mask)*8):(tmp << (address & addr_mask)*8)));
  }

  /// Aligns the byte value read from TLM protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_byte_from_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 1);
    DATA tmp = ((endian==TLM_BIG_ENDIAN)?data >> ((~(address & addr_mask)) & addr_mask)*8:data >> (address & addr_mask)*8);
    return((tmp & 0xff));
  }

  /// Aligns the short value read from TLM protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_short_from_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 2);
    DATA tmp = ((endian==TLM_BIG_ENDIAN)?data >> ((~(address & addr_mask)) & addr_mask)*8:data >> (address & addr_mask)*8);
    return((tmp & 0xffff));
  }

  /// Aligns the word value read from TLM protocol
  template <typename ADDRESS, typename DATA>
  inline DATA align_word_from_tlm(const ADDRESS& address,const DATA& data,const tlm_endianness endian) {
    ADDRESS addr_mask = (ADDRESS)(get_nb_byte<DATA>() - 4);
    DATA tmp = ((endian==TLM_BIG_ENDIAN)?data >> ((~(address & addr_mask)) & addr_mask)*8:data >> (address & addr_mask)*8);
    return((tmp & 0xffffffff));
  }  

  /* @} */

#endif /* _TLM_BYTE_ENABLE_H_ */


