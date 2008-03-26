/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

/*****************************************************************************

  scfx_mant.cpp - 

  Original Author: Robert Graulich, Synopsys, Inc.
                   Martin Janssen,  Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sysc/datatypes/fx/scfx_mant.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  word memory management
// ----------------------------------------------------------------------------

static inline
int
next_pow2_index( size_t size )
{
    int index = scfx_find_msb( size );
    if( ~ ( 1 << index ) & size )
    {
        index ++;
    }
    return index;
}

static word* free_words[32] = { 0 };

word*
scfx_mant::alloc_word( size_t size )
{
    const int ALLOC_SIZE = 128;

    int slot_index = next_pow2_index( size );

    int alloc_size = ( 1 << slot_index );

    word*& slot = free_words[slot_index];

    if( ! slot )
    {
        slot = new word[ALLOC_SIZE * alloc_size];
	int i;
	for( i = 0; i < ( ALLOC_SIZE - 1 ) * alloc_size; i += alloc_size )
	{
	    slot[i] = reinterpret_cast<word>( slot + i + alloc_size );
	}
	slot[i] = 0;
    }

    word* result = slot;
    slot = reinterpret_cast<word*>( slot[0] );
    return result;
}

void
scfx_mant::free_word( word* array, size_t size )
{
    if( array && size )
    {
        int slot_index = next_pow2_index( size );

	word*& slot = free_words[slot_index];

	array[0] = reinterpret_cast<word>( slot );
	slot = array;
    }
}

} // namespace sc_dt


// Taf!
