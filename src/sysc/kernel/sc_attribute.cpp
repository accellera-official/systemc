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

  sc_attribute.cpp -- Attribute classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

// $Log: sc_attribute.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.4  2006/01/16 22:27:08  acg
// Test of $Log comment.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.


#include "sysc/kernel/sc_attribute.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_attr_base
//
//  Attribute base class.
// ----------------------------------------------------------------------------

// constructors

sc_attr_base::sc_attr_base( const std::string& name_ )
: m_name( name_ )
{}

sc_attr_base::sc_attr_base( const sc_attr_base& a )
: m_name( a.m_name )
{}


// destructor (does nothing)

sc_attr_base::~sc_attr_base()
{}


// get the name
const std::string&
sc_attr_base::name() const
{
    return m_name;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_attr_cltn
//
//  Attribute collection class. Stores pointers to attributes.
//  Note: iterate over the collection by using iterators.
// ----------------------------------------------------------------------------

// constructors

sc_attr_cltn::sc_attr_cltn()
{}

sc_attr_cltn::sc_attr_cltn( const sc_attr_cltn& a )
: m_cltn( a.m_cltn )
{}


// destructor
sc_attr_cltn::~sc_attr_cltn()
{
    remove_all();
}


// add attribute to the collection.
// returns 'true' if the name of the attribute is unique,
// returns 'false' otherwise (attribute is not added).

bool
sc_attr_cltn::push_back( sc_attr_base* attribute_ )
{
    if( attribute_ == 0 ) {
	return false;
    }
    for( int i = m_cltn.size() - 1; i >= 0; -- i ) {
	if( attribute_->name() == m_cltn[i]->name() ) {
	    return false;
	}
    }
    m_cltn.push_back( attribute_ );
    return true;
}


// get attribute by name.
// returns pointer to attribute, or 0 if name does not exist.

sc_attr_base*
sc_attr_cltn::operator [] ( const std::string& name_ )
{
    for( int i = m_cltn.size() - 1; i >= 0; -- i ) {
	if( name_ == m_cltn[i]->name() ) {
	    return m_cltn[i];
	}
    }
    return 0;
}

const sc_attr_base*
sc_attr_cltn::operator [] ( const std::string& name_ ) const
{
    for( int i = m_cltn.size() - 1; i >= 0; -- i ) {
	if( name_ == m_cltn[i]->name() ) {
	    return m_cltn[i];
	}
    }
    return 0;
}


// remove attribute by name.
// returns pointer to attribute, or 0 if name does not exist.

sc_attr_base*
sc_attr_cltn::remove( const std::string& name_ )
{
    for( int i = m_cltn.size() - 1; i >= 0; -- i ) {
	if( name_ == m_cltn[i]->name() ) {
	    sc_attr_base* attribute = m_cltn[i];
	    m_cltn[i] = m_cltn[m_cltn.size() - 1];
	    m_cltn.decr_count();
	    return attribute;
	}
    }
    return 0;
}


// remove all attributes

void
sc_attr_cltn::remove_all()
{
    m_cltn.erase_all();
}

} // namespace sc_core
// Taf!
