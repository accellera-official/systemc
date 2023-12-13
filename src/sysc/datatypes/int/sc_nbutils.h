/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_nbutils.h -- External and friend functions for both sc_signed and
                  sc_unsigned classes.

  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_nbutils.h,v $
// Revision 1.6  2011/09/08 16:12:15  acg
//  Philipp A. Hartmann: fix issue with Sun machines wrt real math libraries.
//
// Revision 1.5  2011/08/26 23:00:01  acg
//  Torsten Maehne: remove use of ieeefp.h.
//
// Revision 1.4  2011/08/15 16:43:24  acg
//  Torsten Maehne: changes to remove unused argument warnings.
//
// Revision 1.3  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.2  2010/09/06 16:35:48  acg
//  Andy Goodrich: changed i386 to __i386__ in ifdef's.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#ifndef SC_NBUTILS_H
#define SC_NBUTILS_H

#include <cmath>
#include <limits>

#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/datatypes/int/sc_int_ids.h"
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/utils/sc_string.h"
#include "sysc/utils/sc_report.h"

namespace sc_dt
{

// Parse a character string into its equivalent binary bits.
extern
SC_API void parse_binary_bits(
    const char* src_p, int dst_n, sc_digit* data_p, sc_digit* ctrl_p=0
);


// Parse a character string into its equivalent hexadecimal bits.
extern
SC_API void parse_hex_bits(
    const char* src_p, int dst_n, sc_digit* data_p, sc_digit* ctrl_p=0
);

// ----------------------------------------------------------------------------
//  Various utility functions.
// ----------------------------------------------------------------------------

// Concatenate the high part h and low part l. Assumes that h and l
// are less than or equal to HALF_DIGIT_MASK;
inline 
sc_digit 
concat(sc_digit h, sc_digit l)
{
  return ((h << BITS_PER_HALF_DIGIT) | l);
}

// Create a number with n 1's.
inline
sc_carry
one_and_ones(int n)
{
  return (((sc_carry) 1 << n) - 1);
}

// Create a number with one 1 and n 0's.
inline
sc_carry
one_and_zeros(int n)
{
  return ((sc_carry) 1 << n);
}

// ----------------------------------------------------------------------------
//  Functions to handle built-in types or signs.
// ----------------------------------------------------------------------------

// u = v
// - v is an unsigned long or uint64, and positive integer.
template< class Type >
inline
void
from_uint(int ulen, sc_digit *u, Type v)
{

#ifdef DEBUG_SYSTEMC
  // assert((ulen <= 0) || (u != NULL));
  assert((ulen > 0) && (u != NULL));
  assert(v >= 0);
#endif

  int i = 0;

  while (v && (i < ulen)) {
#ifndef WIN32
    u[i++] = static_cast<sc_digit>( v & DIGIT_MASK );
#else
    u[i++] = ((sc_digit) v) & DIGIT_MASK;
#endif
    v >>= BITS_PER_DIGIT;
  }

  for ( int digit_i = i; digit_i < ulen; ++digit_i ) { u[digit_i] = 0; }

}



// ----------------------------------------------------------------------------
//  Functions to test for errors and print out error messages.
// ----------------------------------------------------------------------------

template< class Type >
inline
void
div_by_zero(Type s)
{
  if (s == 0) {
      SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_,
		       "div_by_zero<Type>( Type ) : division by zero" );
  }
}


// ----------------------------------------------------------------------------
//  Functions for both signed and unsigned numbers to convert sign-magnitude
//  (SM) and 2's complement (2C) representations.
//  added = 1 => for signed.
//  added = 0 => for unsigned.
//  IF_SC_SIGNED can be used as 'added'.
// ----------------------------------------------------------------------------

// Trim the extra leading bits of a signed or unsigned number.
inline
void
trim(small_type added, int nb, int nd, sc_digit *d)
{
#ifdef DEBUG_SYSTEMC
  assert((nb > 0) && (nd > 0) && (d != NULL));
#endif

  d[nd - 1] &= one_and_ones(SC_BIT_INDEX(nb - 1) + added);    
}

// ----------------------------------------------------------------------------
//  Faster set(i, v), without bound checking.
// ----------------------------------------------------------------------------

// A version of set(i, v) without bound checking.
inline
void
safe_set(int i, bool v, sc_digit *d)
{

#ifdef DEBUG_SYSTEMC
  assert((i >= 0) && (d != NULL));
#endif

  int bit_num = SC_BIT_INDEX(i);
  int digit_num = SC_DIGIT_INDEX(i);
  
  if (v)
    d[digit_num] |= one_and_zeros(bit_num);      
  else
    d[digit_num] &= ~(one_and_zeros(bit_num));
  
}


// ----------------------------------------------------------------------------
//  Function to check if a double number is bad (NaN or infinite).
// ----------------------------------------------------------------------------

inline
bool
is_nan( double v )
{
    return std::numeric_limits<double>::has_quiet_NaN && (v != v);
}

inline
bool
is_inf( double v )
{
    return v ==  std::numeric_limits<double>::infinity()
        || v == -std::numeric_limits<double>::infinity();
}

inline
void
is_bad_double(double v)
{
// Windows throws exception.
    if( is_nan(v) || is_inf(v) )
        SC_REPORT_ERROR( sc_core::SC_ID_VALUE_NOT_VALID_,
		         "is_bad_double( double v ) : "
		         "v is not finite - NaN or Inf" );
}

//--------------------------------------------------------------------------------------------------
// sc_digit_heap - CLASS MANAGING A TEMPORARY HEAP OF BYTES
//
// This facility implements a heap of temporary sc_digit allocations. Once a
// request has been allocated it is not freed. However the entire heap 
// wraps and the storage is reused. This means that no allocations should
// be assumed as permanent. Allocations are double-word aligned. This is
// raw storage, so objects which contain virtual methods cannot be allocated
// with this object. See the sc_vpool object for that type of storage 
// allocation.
//
// sc_digit* allocate( int digits_n )
//   This method returns a pointer to block of digit_n sc_digits. The block
//   returned is the next available one in the heap. If the current heap
//   cannot fullfil the request it will be rewound and storage allocated from
//   its start. All allocations start on an 8-byte boundary.
//       digits_n = number of sc_digits to be allocated.
//
// void initialize( int heap_size=0x100000 )
//   This method allocates the storage to be managed. If there is already
//   a block of storage under management it is freed. If no argument is
//   provided for the heap size, 0x10000 sc_digit instances will be allocated.
//       heap_size = number of sc_digit instances to allocate for the heap.
//
// unsigned int length()
//   This method returns the size of this object's heap in bytes.
//
// sc_digit_heap()
//   This is the non-initialized object instance constructor. It does not 
//   allocate the heap storage, that is done by the initialize() method.
//
// sc_digit_heap(int heap_size)
//   This is the initializing object instance constructor. It does allocates
//   a heap of the specified number of sc_digit instances.
//       heap_size = number of sc_digit instances to allocate for the heap.
//--------------------------------------------------------------------------------------------------
class SC_API sc_digit_heap {
  public:
    sc_digit*  m_bgn_p;  // Beginning of heap storage.
    sc_digit*  m_end_p;  // End of heap storage.
    sc_digit*  m_next_p; // Next heap location to be allocated.

    inline sc_digit* allocate( size_t digits_n )
    {
        sc_digit*   result_p;
        result_p = m_next_p;
        m_next_p += digits_n;
        if ( m_next_p >= m_end_p )
        {
            result_p = m_bgn_p;
            m_next_p = m_bgn_p + digits_n;
        }
        return result_p; 
    }

    inline void initialize( size_t heap_size=0x100000 )
    {
        delete [] m_bgn_p;
        m_bgn_p = new sc_digit[heap_size];
        m_end_p = &m_bgn_p[heap_size];
        m_next_p = m_bgn_p;
    }

    inline size_t length()
    {
	return (size_t)(m_end_p - m_bgn_p);
    }

    inline sc_digit_heap() : m_bgn_p(0), m_end_p(0), m_next_p(0)
    {
    }

    inline sc_digit_heap( size_t heap_size ) : m_bgn_p(0), m_end_p(0), m_next_p(0)
    {
	initialize( heap_size );
    }

    inline ~sc_digit_heap()
    {
	    delete [] m_bgn_p;
    }

};

// Reference to the sc_digit_heap instance present in sc_nbutils.cpp:

extern sc_digit_heap SC_API sc_temporary_digits;

} // namespace sc_dt

#endif
