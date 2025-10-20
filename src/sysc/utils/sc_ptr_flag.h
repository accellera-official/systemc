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

  sc_ptr_flag.h - Helper to store a flag with a pointer

  Original Author: Philipp A. Hartmann, Intel

 *****************************************************************************/

#ifndef SC_PTR_FLAG_H_INCLUDED_
#define SC_PTR_FLAG_H_INCLUDED_

#include <cstdint>

#include "sysc/kernel/sc_cmnhdr.h"

namespace sc_core {

template<typename T>
class sc_ptr_flag
{
    typedef std::uintptr_t uintptr_type;
    static_assert( alignof(T) > 1
        , "Unsupported platform/type, need spare LSB of pointer to store flag" );
public:
    typedef T*  pointer;
    typedef T&  reference;

    sc_ptr_flag()
      : m_data() {}

    sc_ptr_flag(pointer p, bool f = false)
      : m_data(reinterpret_cast<uintptr_type>(p))
    {
        set_flag(f);
    }

    sc_ptr_flag& operator=(pointer p)
      { reset(p); return *this; }

    operator pointer() const    { return get(); }
    pointer  operator->() const { return get(); }
    reference operator*() const { return *get(); }

    pointer get() const
      { return reinterpret_cast<pointer>( m_data & pointer_mask ); }

    void reset(pointer p)
    {
        m_data = reinterpret_cast<uintptr_type>(p) | static_cast<uintptr_type>(get_flag());
    }

    bool get_flag() const
      { return ( m_data & flag_mask ) != 0x0; }

    void set_flag(bool f)
      { m_data = ( m_data & pointer_mask ) | static_cast<uintptr_type>(f); }

private:
    static const uintptr_type flag_mask     = 0x1;
    static const uintptr_type pointer_mask  = ~flag_mask;
    uintptr_type m_data;

}; // struct sc_ptr_flag

} // namespace sc_core
#endif // SC_PTR_FLAG_H_INCLUDED_
