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

#ifndef TLM_DATA_SERIALIZERS_HEADER
#define TLM_DATA_SERIALIZERS_HEADER

#include "tlm_helpers.h"

//
// Copy from/to a byte array in host endianness
//

//
// serialisation helper functions 
// for standard SystemC data types : sc_int<W>, sc_uint<W>, sc_bigint<W>, sc_biguint<W>
// for these, the index is not in bytes but in units of W (bit lenght)
// (number of bytes read from the array is rounded to W/8)
//
template< class T >
inline void copy_from_array( T& data, unsigned int index, unsigned char* m_data)
{
  const int nr_bytes = data.length() / 8;

  if (hostHasLittleEndianness()) {
    for(int b = 0; b < nr_bytes; ++b) {
      data.range(b*8+7, b*8) = sc_uint<8>(m_data[nr_bytes*index + b]); 
    }

  } else {
    for(int b = 0; b < nr_bytes; ++b) {
      data.range(b*8+7, b*8) = sc_uint<8>(m_data[nr_bytes*index + (nr_bytes - b - 1)]); 
    }
  }
}

template< class T >
inline void copy_to_array( const T& data, unsigned int index, unsigned char* m_data)
{
  const int nr_bytes = data.length() / 8;

  if (hostHasLittleEndianness()) {
    for(int b = 0; b < nr_bytes; ++b) {
      m_data[nr_bytes*index + b] = sc_uint<8>(data.range(b*8+7,b*8));
    }

  } else {
    for(int b = 0; b < nr_bytes; ++b) {
      m_data[nr_bytes*index + (nr_bytes - b - 1)] = sc_uint<8>(data.range(b*8+7,b*8));
    }
  }
}


//
// serialisation helper functions 
// for standard C++ data types (up to 64 bits): char, short, int, long, long long
// for these, the index is not in bytes but in units of sizeof(T)
//
#define TLM_COPY_FROM_ARRAY( otype )  \
  template<> inline \
  void copy_from_array( otype& data, unsigned int index, unsigned char* m_data) \
  { \
    data = reinterpret_cast<otype*>(m_data)[index]; \
  }

#define TLM_COPY_TO_ARRAY( otype ) \
  template<> inline \
  void copy_to_array( const otype& data, unsigned int index, unsigned char* m_data) \
  { \
    reinterpret_cast<otype*>(m_data)[index] = data; \
  }

TLM_COPY_FROM_ARRAY( signed char );
TLM_COPY_FROM_ARRAY( signed short );
TLM_COPY_FROM_ARRAY( signed int );
TLM_COPY_FROM_ARRAY( signed long );
TLM_COPY_FROM_ARRAY( signed long long );
TLM_COPY_FROM_ARRAY( unsigned char );
TLM_COPY_FROM_ARRAY( unsigned short );
TLM_COPY_FROM_ARRAY( unsigned int );
TLM_COPY_FROM_ARRAY( unsigned long );
TLM_COPY_FROM_ARRAY( unsigned long long );

TLM_COPY_TO_ARRAY( signed char );
TLM_COPY_TO_ARRAY( signed short );
TLM_COPY_TO_ARRAY( signed int );
TLM_COPY_TO_ARRAY( signed long );
TLM_COPY_TO_ARRAY( signed long long );
TLM_COPY_TO_ARRAY( unsigned char );
TLM_COPY_TO_ARRAY( unsigned short );
TLM_COPY_TO_ARRAY( unsigned int );
TLM_COPY_TO_ARRAY( unsigned long );
TLM_COPY_TO_ARRAY( unsigned long long );
   
#endif /* TLM_DATA_SERIALIZERS_HEADER */
