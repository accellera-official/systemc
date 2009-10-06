/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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


// $Log: scfx_mant.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:58  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#include "sysc/datatypes/fx/scfx_mant.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  word memory management
// ----------------------------------------------------------------------------

class word_list { // Entry in free_words bucket.
  public:
    word_list* m_next_p;
};

static inline
int
next_pow2_index( std::size_t size )
{
    int index = scfx_find_msb( size );
    // If this was a power of 2 we are one bucket too low.
    if( ~ (1 << index) & size ) index ++;
    // If this is a 64-bit machine and we are using 32-bit words go down
	// one slot size, as all the slots are 2x in size.
    if ( index != 0 && ( sizeof(word_list) != sizeof(word) ) ) 
	{
		index -= 1;
	}
    return index;
}

static word_list* free_words[32] = { 0 };
    
word*
scfx_mant::alloc_word( std::size_t size )
{
    const int ALLOC_SIZE = 128;

    int slot_index = next_pow2_index( size );

    int alloc_size = ( 1 << slot_index );

    word_list*& slot = free_words[slot_index];

    if( ! slot )
    {
        slot = new word_list[ALLOC_SIZE * alloc_size];
	int i;
	for( i = 0; i < alloc_size*(ALLOC_SIZE-1) ; i+=alloc_size )
	{
	    slot[i].m_next_p = &slot[i+alloc_size];
	}
	slot[i].m_next_p = 0;
    }

    word* result = (word*)slot;
    free_words[slot_index] = slot[0].m_next_p;
    return result;
}

void
scfx_mant::free_word( word* array, std::size_t size )
{
    if( array && size )
    {
        int slot_index = next_pow2_index( size );
	word_list* wl_p = (word_list*)array;

	wl_p->m_next_p = free_words[slot_index];
	free_words[slot_index] = wl_p;
    }
}

} // namespace sc_dt


// Taf!
