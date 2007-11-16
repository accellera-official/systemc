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

#ifndef __TLM_DATA_SERIALIZERS_H__
#define __TLM_DATA_SERIALIZERS_H__

#include "tlm_helpers.h"

namespace tlm {

// ///////////////////////////////////////////////////////////////////////// 
// -- Address helpers --
//
// Calculate the necessary offset in the GP array to retrieve the sub-word
// pointed by the address, taking into account model and host endianness.
//
// Takes as input the following parameters:
//  - address: GP address
//  - bus_width: size in bytes of the interconnect (socket)
//  - subword_size: size in bytes of the data to retrieve from GP data array
//  - endianness: endianness of the model
//
// ///////////////////////////////////////////////////////////////////////// 

inline unsigned int get_subword_offset(sc_dt::int64 address,
                                       unsigned int bus_width,
                                       unsigned int subword_size,
                                       tlm_endianness endianness)
{
    unsigned int a = (unsigned int)address;
    unsigned int offset = ((a%bus_width)/subword_size)*subword_size;
    if(hasHostEndianness(endianness)) 
        return offset;
    else
        return bus_width - offset - subword_size;
}


// ///////////////////////////////////////////////////////////////////////// 
//
// -- Serialisation helper functions  --
//
// Copy a word from/to a byte array (GP) taking into account byte-enables
// 
// These functions work for :
//  - standard SystemC data types :
//    sc_int<W>, sc_uint<W>, sc_bigint<W>, sc_biguint<W>
//  - standard C++ data types (up to 64 bits):
//     char, short, int, long, long long
//  - char* as used in tlm_memory
//
// Serializers take as input the following parameters:
//  - data_word: SystemC type, C++ type or char* (but its size has to be
//    smaller than the bus width)
//  - offset:  offset in bytes on the GP data array
//  - nr_bytes:  number of bytes that are copied (normally ok if equal to
//    data_word size)
//  - m_data:    GP data array 
//  - m_be:      GP byte-enable pointer
//  - m_be_length: GP byte-enable length attribute
//
// ///////////////////////////////////////////////////////////////////////// 

template<class T> 
void copy_word_from_array( T& data_word, 
                           unsigned int offset, 
                           unsigned int nr_bytes,  
                           unsigned char *m_data,
                           bool *m_be, 
                           unsigned int m_be_length); 

template<class T>
void copy_word_to_array( T& data_word, 
                         unsigned int offset, 
                         unsigned int nr_bytes,  
                         unsigned char *m_data,
                         bool *m_be, 
                         unsigned int m_be_length); 

// ///////////////////////////////////////////////////////////////////////// 
// SystemC data types : sc_int<W>, sc_uint<W>, sc_bigint<W>, sc_biguint<W>
// ///////////////////////////////////////////////////////////////////////// 
//
template< class T >
inline void copy_word_from_array( T& data_word, 
                                  unsigned int offset, 
                                  unsigned int nr_bytes,  
                                  unsigned char *m_data,
                                  bool *m_be, 
                                  unsigned int m_be_length) 
{
    unsigned int data_size = data_word.length()/8;
    assert(nr_bytes <= data_size);
    if(m_be == 0) 
        for(unsigned int b=0; b<nr_bytes; b++)
            data_word.range(b*8+7, b*8) = sc_dt::sc_uint<8>(m_data[offset+b]); 
    else
        for(unsigned int b=0; b<nr_bytes; b++)
            if(m_be[(offset + b) % m_be_length]) 
                data_word.range(b*8+7, b*8) = sc_dt::sc_uint<8>(m_data[offset+b]);
}

template< class T >
inline void copy_word_to_array( T& data_word, 
                                unsigned int offset, 
                                unsigned int nr_bytes,  
                                unsigned char *m_data,
                                bool *m_be, 
                                unsigned int m_be_length)
{
    unsigned int data_size = data_word.length()/8;	
    assert(nr_bytes <= data_size);
    if(m_be == 0) 
        for(unsigned int b=0; b<nr_bytes; b++)
            m_data[offset+b] = (unsigned char)sc_dt::sc_uint<8>(data_word.range(b*8+7, b*8)).value(); 
    else
        for(unsigned int b=0; b<nr_bytes; b++)
            if(m_be[(offset + b) % m_be_length]) 
                m_data[offset+b] = (unsigned char)sc_dt::sc_uint<8>(data_word.range(b*8+7, b*8)).value();
}
//
// ///////////////////////////////////////////////////////////////////////// 

// ///////////////////////////////////////////////////////////////////////// 
// Standard C++ data types (up to 64 bits): char, short, int, long, long long
// ///////////////////////////////////////////////////////////////////////// 
//
#define TLM_COPY_WORD_FROM_ARRAY( otype ) \
template<> inline \
void copy_word_from_array( otype& data_word, \
                           unsigned int offset, \
                           unsigned int nr_bytes, \
                           unsigned char *m_data, \
                           bool *m_be, \
                           unsigned int m_be_length) \
{ \
    unsigned int data_size = sizeof(otype); \
    assert(nr_bytes <= data_size); \
    if(m_be == 0) \
        for(unsigned int b=0; b<nr_bytes; b++) \
            reinterpret_cast<unsigned char*>(&data_word)[b] = m_data[offset+b]; \
    else \
        for(unsigned int b=0; b<nr_bytes; b++) \
            if(m_be[(offset + b) % m_be_length]) \
                reinterpret_cast<unsigned char*>(&data_word)[b] = m_data[offset+b]; \
}

#define TLM_COPY_WORD_TO_ARRAY( otype )  \
template<> inline \
void copy_word_to_array( otype& data_word, \
                         unsigned int offset, \
                         unsigned int nr_bytes, \
                         unsigned char *m_data, \
                         bool *m_be, \
                         unsigned int m_be_length) \
{ \
    unsigned int data_size = sizeof(otype); \
    assert(nr_bytes <= data_size); \
    if(m_be == 0) \
        for(unsigned int b=0; b<nr_bytes; b++) \
            m_data[offset+b] = reinterpret_cast<unsigned char*>(&data_word)[b]; \
    else \
        for(unsigned int b=0; b<nr_bytes; b++) \
            if(m_be[(offset+b) % m_be_length]) \
                m_data[offset+b] = reinterpret_cast<unsigned char*>(&data_word)[b]; \
}

TLM_COPY_WORD_FROM_ARRAY( signed char )
TLM_COPY_WORD_FROM_ARRAY( signed short )
TLM_COPY_WORD_FROM_ARRAY( signed int )
TLM_COPY_WORD_FROM_ARRAY( signed long )
TLM_COPY_WORD_FROM_ARRAY( signed long long )
TLM_COPY_WORD_FROM_ARRAY( unsigned char )
TLM_COPY_WORD_FROM_ARRAY( unsigned short )
TLM_COPY_WORD_FROM_ARRAY( unsigned int )
TLM_COPY_WORD_FROM_ARRAY( unsigned long )
TLM_COPY_WORD_FROM_ARRAY( unsigned long long )

TLM_COPY_WORD_TO_ARRAY( signed char )
TLM_COPY_WORD_TO_ARRAY( signed short )
TLM_COPY_WORD_TO_ARRAY( signed int )
TLM_COPY_WORD_TO_ARRAY( signed long )
TLM_COPY_WORD_TO_ARRAY( signed long long )
TLM_COPY_WORD_TO_ARRAY( unsigned char )
TLM_COPY_WORD_TO_ARRAY( unsigned short )
TLM_COPY_WORD_TO_ARRAY( unsigned int )
TLM_COPY_WORD_TO_ARRAY( unsigned long )
TLM_COPY_WORD_TO_ARRAY( unsigned long long )

//
// ///////////////////////////////////////////////////////////////////////// 


// ///////////////////////////////////////////////////////////////////////// 
// char* specialization
// ///////////////////////////////////////////////////////////////////////// 
//
inline void copy_word_from_array( unsigned char* data_word, 
                                  unsigned int offset, 
                                  unsigned int nr_bytes, 
                                  unsigned char *m_data, 
                                  bool *m_be, 
                                  unsigned int m_be_length) 
{
    if(m_be == 0) 
        for(unsigned int b=0; b<nr_bytes; b++)
            data_word[b] = m_data[offset+b]; 
    else
        for(unsigned int b=0; b<nr_bytes; b++)
            if(m_be[(offset+b) % m_be_length]) 
                data_word[b] = m_data[offset+b];
}

inline void copy_word_to_array( unsigned char* data_word, 
                                unsigned int offset, 
                                unsigned int nr_bytes, 
                                unsigned char *m_data, 
                                bool *m_be, 
                                unsigned int m_be_length) 
{
    if(m_be == 0) 
        for(unsigned int b=0; b<nr_bytes; b++)
            m_data[offset+b] = data_word[b]; 
    else
        for(unsigned int b=0; b<nr_bytes; b++)
            if(m_be[(offset+b) % m_be_length]) 
                m_data[offset+b] = data_word[b];
}
//
// ///////////////////////////////////////////////////////////////////////// 

} // namespace tlm

#endif /* __TLM_DATA_SERIALIZERS_H__ */
