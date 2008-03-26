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

  sc_attribute.h -- Attribute classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/
// $Log: sc_attribute.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_ATTRIBUTE_H
#define SC_ATTRIBUTE_H

#include "sysc/utils/sc_string.h"
#include "sysc/utils/sc_vector.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_attr_base
//
//  Attribute base class.
// ----------------------------------------------------------------------------

class sc_attr_base
{
public:

    // constructors
    sc_attr_base( const std::string& name_ );
    sc_attr_base( const sc_attr_base& );

    // destructor (does nothing)
    virtual ~sc_attr_base();

    // get the name
    const std::string& name() const;

private:

    std::string m_name;

private:

    // disabled
    sc_attr_base();
    sc_attr_base& operator = ( const sc_attr_base& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_attr_cltn
//
//  Attribute collection class. Stores pointers to attributes.
//  Note: iterate over the collection by using iterators.
// ----------------------------------------------------------------------------

class sc_attr_cltn
{
public:

    // typedefs
    typedef sc_attr_base*    elem_type;
    typedef elem_type*       iterator;
    typedef const elem_type* const_iterator;

    // constructors
    sc_attr_cltn();
    sc_attr_cltn( const sc_attr_cltn& );

    // destructor
    ~sc_attr_cltn();

    // add attribute to the collection.
    // returns 'true' if the name of the attribute is unique,
    // returns 'false' otherwise (attribute is not added).
    bool push_back( sc_attr_base* );

    // get attribute by name.
    // returns pointer to attribute, or 0 if name does not exist.
          sc_attr_base* operator [] ( const std::string& name_ );
    const sc_attr_base* operator [] ( const std::string& name_ ) const;

    // remove attribute by name.
    // returns pointer to attribute, or 0 if name does not exist.
    sc_attr_base* remove( const std::string& name_ );

    // remove all attributes
    void remove_all();

    // get the size of the collection
    int size() const
        { return m_cltn.size(); }

    // get the begin iterator
    iterator begin()
        { return m_cltn.begin(); }
    const_iterator begin() const
        { return m_cltn.begin(); }

    // get the end iterator
    iterator end()
        { return m_cltn.end(); }
    const_iterator end() const
        { return m_cltn.end(); }

private:

    sc_pvector<sc_attr_base*> m_cltn;

private:

    // disabled
    sc_attr_cltn& operator = ( const sc_attr_cltn& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_attribute<T>
//
//  Attribute class.
//  Note: T must have a default constructor and copy constructor.
// ----------------------------------------------------------------------------

template <class T>
class sc_attribute
: public sc_attr_base
{
public:

    // constructors

    sc_attribute( const std::string& name_ )
        : sc_attr_base( name_ ), value()
        {}

    sc_attribute( const std::string& name_, const T& value_ )
        : sc_attr_base( name_ ), value( value_ )
        {}

    sc_attribute( const sc_attribute<T>& a )
        : sc_attr_base( a.name() ), value( a.value )
        {}


    // destructor (does nothing)

    virtual ~sc_attribute()
        {}

public:

    // public data member; for easy access
    T value;

private:

    // disabled
    sc_attribute();
    sc_attribute<T>& operator = ( const sc_attribute<T>& );
};

} // namespace sc_core

#endif


// Taf!
