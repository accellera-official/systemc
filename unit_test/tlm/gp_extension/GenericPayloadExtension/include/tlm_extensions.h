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

#ifndef __TLM_EXTENSIONS_H__
#define __TLM_EXTENSIONS_H__

#include "tlm.h"
#include <cassert>

// This header file defines a simple set of extension classes.
// The basic structure here is that the classes must be derived
// from tlm_extension<T>, with T being the extension class
// name.
//
// The extension classes must define a clone() method to allow for cloning
// of the full tlm_generic_payload class.

class tlm_extension1 :
   public tlm::tlm_extension<tlm_extension1>
{
public:
    tlm_extension1()
        : data1(0)
    {}
    int data1;

    tlm_extension_base* clone() const
    {
        return new tlm_extension1(*this);
    }
    void free()
    {
        delete this;
    }
    void copy_from(tlm_extension_base const & e)
    {
        assert(typeid(this) == typeid(e));
        data1 = static_cast<tlm_extension1 const &>(e).data1;
    }
};

class tlm_extension2 :
   public tlm::tlm_extension<tlm_extension2>
{
public:
    tlm_extension2()
        : data2(0)
    {}
    int data2;
    tlm_extension_base* clone() const
    {
        return new tlm_extension2(*this);
    }
    void free()
    {
        delete this;
    }
    void copy_from(tlm_extension_base const & e)
    {
        assert(typeid(this) == typeid(e));
        data2 = static_cast<tlm_extension2 const &>(e).data2;
    }
};

class tlm_extension3 :
  public tlm::tlm_extension<tlm_extension3>
{
public:
    tlm_extension3()
        : data3(0)
    {}
    int data3;

    tlm_extension_base* clone() const
    {
        return new tlm_extension3(*this);
    }
    void free()
    {
        delete this;
    }
    void copy_from(tlm_extension_base const & e)
    {
        assert(typeid(this) == typeid(e));
        data3 = static_cast<tlm_extension3 const &>(e).data3;
    }
};

#endif
