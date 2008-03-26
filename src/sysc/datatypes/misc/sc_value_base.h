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

  sc_value_base.h -- Base class for SystemC bit values.

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_VALUE_BASE_H
#define SC_VALUE_BASE_H


#include "sysc/datatypes/int/sc_nbdefs.h"

namespace sc_dt 
{

class sc_signed;
class sc_unsigned;

// ----------------------------------------------------------------------------
//  CLASS : sc_value_base
//
//  Abstract base class of all SystemC native variables. It provides
//  support for concatenation operations via a set of virtual methods. 
//  A general description of the methods appear with their default 
//  definitions in sc_object.cpp.
// ----------------------------------------------------------------------------

class sc_value_base 
{
    friend class sc_concatref;
  private: 
    virtual void concat_clear_data( bool to_ones=false );
    virtual bool concat_get_ctrl( unsigned long* dst_p, int low_i ) const;
    virtual bool concat_get_data( unsigned long* dst_p, int low_i ) const;
    virtual uint64 concat_get_uint64() const;
    virtual int concat_length(bool* xz_present_p=0) const;
    virtual void concat_set( int64 src, int low_i );
    virtual void concat_set( const sc_signed& src, int low_i );
    virtual void concat_set( const sc_unsigned& src, int low_i );
    virtual void concat_set( uint64 src, int low_i );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_generic_base
//
//  Proxy class for user-defined value classes and other classes that 
//  are defined outside of SystemC. 
//  The class is utilized as a base class for the arbitrary class:
//
//       class my_class : public sc_generic_base<my_class>
//
//  The purpose of the class is to allow to_XXXX methods defined within that 
//  class so that assignments and casts from the arbitrary class to native 
//  SystemC types are possible. To interact correctly with the SystemC library 
//  the class derived from sc_generic_base must implement the following 
//  methods: 
//    (1) uint64 to_uint64() const
//    (2) int64  to_int64() const
//    (3) void to_sc_unsigned( sc_unsigned& ) const
//    (4) void to_sc_signed( sc_signed& ) const
// ----------------------------------------------------------------------------
template< class T >
class sc_generic_base {
  public: 
    inline const T* operator-> () const
    {
        return (const T*)this;
    }
    inline T* operator-> () 
    {
        return (T*)this;
    }
};

} // namespace sc_dt

#endif
