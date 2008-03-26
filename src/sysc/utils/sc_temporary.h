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

  sc_temporary.h -- Temporary value pool classes.

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_temporary.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:39  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_TEMPORARY_H
#define SC_TEMPORARY_H

namespace sc_core {

//------------------------------------------------------------------------------
// sc_byte_heap - CLASS MANAGING A TEMPORARY HEAP OF BYTES
//
// This facility implements a heap of temporary byte allocations. Once an 
// request has been allocated it is not freed. However the entire heap 
// wraps and the storage is reused. This means that no allocations should
// be assumed as permanent. Allocations are double-word aligned. This is
// raw storage, so objects which contain virtual methods cannot be allocated
// with this object. See the sc_vpool object for that type of storage 
// allocation.
//
// char* allocate( int size )
//   This method returns a pointer to block of size bytes. The block
//   returned is the next available one in the heap. If the current heap
//   cannot fullfil the request it will be rewound and storage allocated from
//   its start. All allocations start on an 8-byte boundary.
//       size = number of bytes to be allocated.
//
// void initialize( int heap_size=0x100000 )
//   This method allocates the storage to be managed. If there is already
//   a block of storage under management it is freed. If no argument is
//   provided for the heap size, a megabyte will be allocated.
//       heap_size = number of bytes to allocate for the heap.
//
// unsigned int length()
//   This method returns the size of this object's heap in bytes.
//
// sc_byte_heap()
//   This is the non-initialized object instance constructor. It does not 
//   allocate the heap storage, that is done by the initialize() method.
//
// sc_byte_heap()
//   This is the initializing object instance constructor. It does allocates
//   a heap of the specified number of bytes.
//       heap_size = number of bytes to allocate for the heap.
//------------------------------------------------------------------------------
class sc_byte_heap {
  public:
    char*  m_bgn_p;  // Beginning of heap storage.
    char*  m_end_p;  // End of heap storage.
    char*  m_next_p; // Next heap location to be allocated.

    inline char* allocate( int bytes_n )
    {
        char*   result_p;
        bytes_n = (bytes_n + 7) & 0xfffffff8;
        result_p = m_next_p;
        m_next_p += bytes_n;
        if ( m_next_p >= m_end_p )
        {
            result_p = m_bgn_p;
            m_next_p = m_bgn_p + bytes_n;
        }
        return result_p; 
    }

    inline void initialize( int heap_size=0x100000 )
    {
		if ( m_bgn_p ) delete [] m_bgn_p;
        m_bgn_p = new char[heap_size];
        m_end_p = &m_bgn_p[heap_size];
        m_next_p = m_bgn_p;
    }
    
	inline unsigned int length()
	{
		return (unsigned int)(m_end_p - m_bgn_p);
	}

	inline sc_byte_heap()
	{
		m_bgn_p = 0;
	}

	inline sc_byte_heap( int heap_size )
	{
		m_bgn_p = 0;
		initialize( heap_size );
	}

	inline ~sc_byte_heap()
	{
		if ( m_bgn_p ) delete [] m_bgn_p;
	}

};


//------------------------------------------------------------------------------
// sc_vpool<T> - CLASS MANAGING A TEMPORARY VECTOR OF CLASS T INSTANCES
//
// This class implements a fixed pool of objects contained in a vector. These
// objects are allocated via the allocate() method. An index, m_pool_i, 
// indicates the next object to be allocated. The vector is a power of 2 in
// size, and this fact is used to wrap the list when m_pool_i reaches the
// end of the vector.  
// 
// sc_vpool( int log2, T* pool_p=0 )
//   This is the object instance constructor for this class. It configures
//   the object to manage a vector of 2**log2 entries. If a vector is 
//   not supplied one will be allocated.
//     log2   =  the log base two of the size of the vector.
//     pool_p -> vector of 2**log2 entries to be managed or 0.
//
// ~sc_vpool()
//   This is the object instance destructor for this class. It frees the
//   block of storage which was being managed.
//
// T* allocate()
//   This method returns the address of the next entry in the vector, m_pool_p, 
//   pointed to by the index, m_pool_i, and updates that index. The index
//   update consists of adding 1 to m_pool_i and masking it by m_wrap.
// 
// void reset()
//   This method resets the allocation index, m_pool_i, to point to the start
//   of the vector of objects under management. This call is not usually made
//   since there are a fixed number of entries and the index wraps. However,
//   for diagnostics tests it is convenient to be able to reset to the start
//   of the vector.
//
// int size()
//   This method returns the number of object instances contained in the
//   vector being managed by this object instance.
//------------------------------------------------------------------------------
template<class T>
class sc_vpool {
  protected:
	int m_pool_i;	// Index of next entry to m_pool_m to provide.
	T*  m_pool_p;	// Vector of temporaries.
	int m_wrap;		// Mask to wrap vector index.

  public:
	inline sc_vpool( int log2, T* pool_p=0 );
	inline ~sc_vpool();
	inline T* allocate();
	inline void reset();
	inline int size();
};

template<class T> sc_vpool<T>::sc_vpool( int log2, T* pool_p )
{
	// if ( log2 > 32 ) SC_REPORT_ERROR(SC_ID_POOL_SIZE_, "");
	m_pool_i = 0;
	m_pool_p = pool_p ? pool_p : new T[1 << log2];
	m_wrap = ~(-1 << log2);
}

template<class T> sc_vpool<T>::~sc_vpool()
{
	// delete [] m_pool_p;
}

template<class T> T* sc_vpool<T>::allocate()
{
	T* result_p;	// Entry to return.

	result_p = &m_pool_p[m_pool_i];
	m_pool_i = (m_pool_i + 1) & m_wrap;
	return result_p;
}

template<class T> void sc_vpool<T>::reset()
{
	m_pool_i = 0;
}

template<class T> int sc_vpool<T>::size()
{
	return m_wrap + 1;
}

} // namespace sc_core

#endif // SC_TEMPORARY_H



