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

  sc_nbutils.cpp -- External and friend functions for both sc_signed and
                    sc_unsigned classes.
 
  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_nbutils.cpp,v $
// Revision 1.4  2011/08/24 22:05:46  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.3  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.2  2007/11/04 21:26:40  acg
//  Andy Goodrich: added a buffer to the allocation of the q array to address
//  an issue with references outside the array by 1 byte detected by valgrind.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#include <cctype>
#include <cstdio>
#include <cstring>
#include <sstream>
#include "sysc/datatypes/int/sc_int_ids.h"
#include "sysc/datatypes/int/sc_nbutils.h"
#include "sysc/kernel/sc_macros.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  SECTION: Static Temporary storage.
//
// ----------------------------------------------------------------------------

sc_digit_heap sc_temporary_digits(0x100000);


// ----------------------------------------------------------------------------
//  SECTION: General utility functions.
// ----------------------------------------------------------------------------


//------------------------------------------------------------------------------
//"parse_binary_bits"
//
// This function parses the supplied string into the supplied vector as a
// right justified bit value.
//    src_p  -> character string representing the bits to be parsed.
//    dst_n  =  number of words in data_p and ctrl_p.
//    data_p -> words w/BITS_PER_DIGIT bits to receive the value's data bits.
//    ctrl_p -> words w/BITS_PER_DIGIT bits to receive the value's control bits,
//              or zero.
// Result is true if value was non-zero.
//------------------------------------------------------------------------------
void parse_binary_bits( 
    const char* src_p, int dst_n, sc_digit* data_p, sc_digit* ctrl_p )
{
    int      bit_i;    // Number of bit now processing.
    sc_digit ctrl;     // Control word now assembling.
    sc_digit data;     // Data word now assembling.
    int      delta_n;  // src_n - dst_n*BITS_PER_DIGIT.
    int      src_i;    // Index in src_p now accessing (left to right).
    int      src_n;    // Length of source that is left in bits.
    int      word_i;   // Bit within word now accessing (left to right).

    // MAKE SURE WE HAVE A STRING TO PARSE:

    if( src_p == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is zero" );
        return;
    }
    if( *src_p == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is empty" );
        return;
    }


    // INDEX INTO THE SOURCE TO A DEPTH THAT WILL ACCOMODATE OUR SIZE:
    //
    // If the source is smaller than our value initialize our value to zero.

    src_n = strlen(src_p);
    delta_n = src_n - (dst_n*BITS_PER_DIGIT);
    if ( delta_n > 0 ) 
    {
        src_p = &src_p[delta_n];
        src_n -= delta_n;
    }
    else
    {
        for ( word_i = 0; word_i < dst_n; word_i++ ) data_p[word_i] = 0;
	    if ( ctrl_p ) 
		for ( word_i = 0; word_i < dst_n; word_i++ ) ctrl_p[word_i] = 0;
    }


    // LOOP OVER THE SOURCE ASSEMBLING WORDS AND PLACING THEM IN OUR VALUE:
    //
    // We stride right to left through the source in BITS_PER_DIGIT chunks.
    // Each of those chunks is processed from left to right a bit at a time.
    // We process the high order word specially, since there are less bits.

    src_n = src_n - BITS_PER_DIGIT;
    for (word_i=0; word_i < dst_n; word_i++)
    {
        src_i = src_n;


        // PARTIAL LAST WORD TO ASSEMBLE:

        if ( src_i < 0 ) 
        {
            src_n += BITS_PER_DIGIT;
            src_i = 0;
            data = 0;
            ctrl = 0;
            for ( src_i = 0; src_i < src_n; src_i++ )
            {
                ctrl = ctrl << 1;
                data = data << 1;
                switch( src_p[src_i] )
                {
                  case 'X':
                  case 'x': ctrl = ctrl | 1; data = data | 1; break;
                  case '1': data = data | 1; break;
                  case 'Z':
                  case 'z': ctrl = ctrl | 1; break;
                  case '0':                  break;
                  default:
                    {
                        char msg[BUFSIZ];
                        std::snprintf(msg, sizeof(msg), "character string '%s' is not valid",
                        src_p );
                        SC_REPORT_ERROR(sc_core::SC_ID_CONVERSION_FAILED_, msg);
			return;
                    }
                    break;
                }
            }
            if ( ctrl_p ) ctrl_p[word_i] = ctrl;
            data_p[word_i] = data;
            break;
        }


        // FULL WORD TO BE ASSEMBLED:

        ctrl = 0;
        data = 0;
        for ( bit_i = 0; bit_i < BITS_PER_DIGIT; bit_i++ )
        {
            ctrl = ctrl << 1;
            data = data << 1;
            switch( src_p[src_i++] )
            {
              case 'X':
              case 'x': ctrl = ctrl | 1; data = data | 1; break;
              case '1': data = data | 1; break;
              case 'Z':
              case 'z': ctrl = ctrl | 1; break;
              case '0':                  break;
              default:
                {
                    char msg[BUFSIZ];
                    std::snprintf(msg, sizeof(msg), "character string '%s' is not valid",
                    src_p );
                    SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_, msg );
		    return;
                }
                break;
            }
        }
        if ( ctrl_p ) ctrl_p[word_i] = ctrl;
        data_p[word_i] = data;
        src_n = src_n - BITS_PER_DIGIT;
    }
}


//------------------------------------------------------------------------------
//"parse_hex_bits"
//
// This function parses the supplied string into the supplied vector as a
// right justified bit value.
//    src_p  -> character string representing the bits to be parsed.
//    dst_n  =  number of words in data_p and ctrl_p.
//    data_p -> words w/32 bits to receive the value's data bits.
//    ctrl_p -> words w/32 bits to receive the value's control bits,
//              or zero.
// Result is true if value was non-zero.
//------------------------------------------------------------------------------
void parse_hex_bits( 
	const char* src_p, int dst_n, sc_digit* data_p, sc_digit* ctrl_p )
{
    sc_digit ctrl;     // Control word now assembling.
    sc_digit data;     // Data word now assembling.
    int      delta_n;  // src_n - dst_n*BITS_PER_DIGIT.
    int      digit_i;  // Number of digit now processing.
    int      src_i;    // Index in src_p now accessing (left to right).
    int      src_n;    // Length of source that is left in bits.
    int      word_i;   // Bit within word now accessing (left to right).

    // MAKE SURE WE HAVE A STRING TO PARSE:

    if( src_p == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is zero" );
        return;
    }
    if( *src_p == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is empty" );
        return;
    }


    // INDEX INTO THE SOURCE TO A DEPTH THAT WILL ACCOMODATE OUR SIZE:
    //
    // If the source is smaller than our value initialize our value to zero.

    src_n = strlen(src_p);
    delta_n = src_n - (dst_n*8);
    if ( delta_n > 0 ) 
    {
        src_p = &src_p[delta_n];
        src_n -= delta_n;
    }
    else
    {
        for ( word_i = 0; word_i < dst_n; word_i++ ) data_p[word_i] = 0;
		if ( ctrl_p ) 
			for ( word_i = 0; word_i < dst_n; word_i++ ) ctrl_p[word_i] = 0;
    }


    // LOOP OVER THE SOURCE ASSEMBLING WORDS AND PLACING THEM IN OUR VALUE:
    //
    // We stride right to left through the source in BITS_PER_DIGIT chunks.
    // Each of those chunks is processed from left to right a bit at a time.
    // We process the high order word specially, since there are less bits.

    src_n = src_n - 8;
    for (word_i=0; word_i < dst_n; word_i++)
    {
        src_i = src_n;


        // PARTIAL LAST WORD TO ASSEMBLE:

        if ( src_i < 0 ) 
        {
            src_n += 8;
            src_i = 0;
            data = 0;
            ctrl = 0;
            for ( src_i = 0; src_i < src_n; src_i++ )
            {
                ctrl = ctrl << 4;
                data = data << 4;
                switch( src_p[src_i] )
                {
                  case 'X':
                  case 'x': ctrl = ctrl | 15; data = data | 15; break;
		  case 'F':
                  case 'f': data = data | 15; break;
		  case 'E':
                  case 'e': data = data | 14; break;
		  case 'D':
                  case 'd': data = data | 13; break;
		  case 'C':
                  case 'c': data = data | 12; break;
		  case 'B':
                  case 'b': data = data | 11; break;
		  case 'A':
                  case 'a': data = data | 10; break;
                  case '9': data = data |  9; break;
                  case '8': data = data |  8; break;
                  case '7': data = data |  7; break;
                  case '6': data = data |  6; break;
                  case '5': data = data |  5; break;
                  case '4': data = data |  4; break;
                  case '3': data = data |  3; break;
                  case '2': data = data |  2; break;
                  case '1': data = data |  1; break;
                  case '0':                  break;
                  case 'Z':
                  case 'z': ctrl = ctrl | 15; break;
                  default:
                    {
                        char msg[BUFSIZ];
                        std::snprintf(msg, sizeof(msg), "character string '%s' is not valid",
                        src_p );
                        SC_REPORT_ERROR(sc_core::SC_ID_CONVERSION_FAILED_, msg);
		        return;
                    }
                    break;
                }
            }
            if ( ctrl_p ) ctrl_p[word_i] = ctrl;
            data_p[word_i] = data;
            break;
        }


        // FULL WORD TO BE ASSEMBLED:

        ctrl = 0;
        data = 0;
        for ( digit_i = 0; digit_i < 8; digit_i++ )
        {
            ctrl = ctrl << 4;
            data = data << 4;
            switch( src_p[src_i++] )
            {
	      case 'X':
	      case 'x': ctrl = ctrl | 15; data = data | 15; break;
	      case 'F':
	      case 'f': data = data | 15; break;
	      case 'E':
	      case 'e': data = data | 14; break;
	      case 'D':
	      case 'd': data = data | 13; break;
	      case 'C':
	      case 'c': data = data | 12; break;
	      case 'B':
	      case 'b': data = data | 11; break;
	      case 'A':
	      case 'a': data = data | 10; break;
	      case '9': data = data |  9; break;
	      case '8': data = data |  8; break;
	      case '7': data = data |  7; break;
	      case '6': data = data |  6; break;
	      case '5': data = data |  5; break;
	      case '4': data = data |  4; break;
	      case '3': data = data |  3; break;
	      case '2': data = data |  2; break;
	      case '1': data = data |  1; break;
	      case '0':                  break;
	      case 'Z':
	      case 'z': ctrl = ctrl | 15; break;
              default:
                {
                    char msg[BUFSIZ];
                    std::snprintf(msg, sizeof(msg), "character string '%s' is not valid",
                    src_p );
                    SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_, msg );
		    return;
                }
                break;
            }
        }
        if ( ctrl_p ) ctrl_p[word_i] = ctrl;
        data_p[word_i] = data;
        src_n = src_n - BITS_PER_DIGIT;
    }
}
        
} // namespace sc_dt


// End of file.
