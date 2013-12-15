/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

  Author: Olaf Scheufen
*****************************************************************************/

#ifndef __TLM_ARRAY_H__
#define __TLM_ARRAY_H__

#include <systemc>
#include <exception>
// unused for the time being: #include <cassert>

namespace tlm {

//
// To the LRM writer: the below class is an artifact of the tlm_generic_payload
//                    implementation and not part of the core TLM standard
//


// This implements a lean and fast array class that supports array expansion on
// request. The class is primarily used in the tlm_generic_payload class for
// storing the pointers to the extensions.
//
// Individual array elements can be accessed through the [] operators, and the
// array length is returned by the size() method.
//
// The size can be dynamically expanded using the expand(uint) method. There
// is no shrinking mechanism implemented, because the extension mechanism
// does not require this feature. Bear in mind that calling the expand method
// may invalidate all direct pointers into the array.


//the tlm_array shall always be used with T=tlm_extension_base*
template <typename T>
class tlm_array
  : private std::vector<T>
{
    typedef std::vector<T>                base_type;
    typedef typename base_type::size_type size_type;
public:

    // constructor:
    tlm_array(size_type size = 0, T const & default_value = T() )
        : base_type(size,default_value)
        , m_entries()
        , m_default(default_value)
    {
        //m_entries.reserve(size); // optional
    }

    // copy constructor:
    // tlm_array(const tlm_array& orig) = default;

    // destructor:
    // ~tlm_array() = default;

    // operators for dereferencing:
    using base_type::operator[];

    // array size:
    using base_type::size;

    // expand the array if needed:
    void expand(size_type new_size)
    {
        if (new_size > size())
        {
            base_type::resize(new_size);
            //m_entries.reserve(new_size); // optional
        }
    }

    static const char* const kind_string;
    const char* kind() const { return kind_string; }

    //this function shall get a pointer to a array slot
    // it stores this slot in a cache of active slots
    void insert_in_cache(T* p)
    {
        //assert( (p-&(*this)[0]) < size() );
        m_entries.push_back( p-&(*this)[0] );
    }

    //this functions clears all active slots of the array
    void free_entire_cache()
    {
        while(m_entries.size())
        {
            if ((*this)[m_entries.back()])      //we make sure no one cleared the slot manually
              (*this)[m_entries.back()]->free();//...and then we call free on the content of the slot
            (*this)[m_entries.back()]=0;        //afterwards we set the slot to NULL
            m_entries.pop_back();
        }
    }

protected:
    std::vector<size_type> m_entries;
    T m_default;

    // disabled:
    tlm_array& operator=(const tlm_array<T>&);
};


template <typename T>
const char* const tlm_array<T>::kind_string = "tlm_array";

} // namespace tlm

#endif /* __TLM_ARRAY_H__ */
