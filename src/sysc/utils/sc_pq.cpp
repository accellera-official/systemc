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

  sc_pq.cpp - Simple heap implementation of priority queue.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_pq.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:39  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

#include "sysc/utils/sc_pq.h"

namespace sc_core {

sc_ppq_base::sc_ppq_base( int sz, int (*cmp)( const void*, const void* ) )
    : m_size_alloc( sz ), m_heap_size( 0 ), m_compar( cmp )
{
    // m_size_alloc must be at least 2, otherwise resizing doesn't work
    if( m_size_alloc < 2 ) {
	m_size_alloc = 2;
    }
    // allocate
    m_heap = new void*[m_size_alloc + 1];
    // initialize
    for( int i = 0; i < m_size_alloc; ++ i ) {
	m_heap[i] = 0;
    }
}

sc_ppq_base::~sc_ppq_base()
{
    delete[] m_heap;
}

void*
sc_ppq_base::extract_top()
{
    assert( m_heap_size > 0 );
    void* topelem = m_heap[1];
    m_heap[1] = m_heap[m_heap_size];
    m_heap_size --;
    heapify( 1 );
    return topelem;
}

void
sc_ppq_base::insert( void* elem )
{
    m_heap_size ++;
    int i = m_heap_size;

    // resize the heap in case there's not enough memory
    if( m_heap_size > m_size_alloc ) {
        m_size_alloc += m_size_alloc / 2;
        void** new_heap = new void*[m_size_alloc + 1];
        for( int j = 1; j < m_heap_size; ++ j ) {
            new_heap[j] = m_heap[j];
        }
        delete[] m_heap;
        m_heap = new_heap;
    }

    while( (i > 1) && (m_compar( m_heap[parent( i )], elem ) < 0) ) {
        m_heap[i] = m_heap[parent( i )];
        i = parent( i );
    }
    m_heap[i] = elem;
}

void
sc_ppq_base::heapify( int i )
{
    int l;
    while( l = left( i ), l <= m_heap_size ) {
        int largest = (m_compar( m_heap[l], m_heap[i] ) > 0) ? l : i;

        int r = right( i );
        if( (r <= m_heap_size) &&
	    (m_compar( m_heap[r], m_heap[largest] ) > 0) ) {
            largest = r;
	}

        if( largest != i ) {
            void* tmp = m_heap[i];
            m_heap[i] = m_heap[largest];
            m_heap[largest] = tmp;
            i = largest;
        } else {
            break;
	}
    }
}

} // namespace sc_core
