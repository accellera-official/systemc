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

  sc_length_param.h - 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-19

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_LENGTH_PARAM_H
#define SC_LENGTH_PARAM_H


#include "sysc/datatypes/fx/sc_context.h"


namespace sc_dt
{

// classes defined in this module
class sc_length_param;


// ----------------------------------------------------------------------------
//  CLASS : sc_length_param
//
//  Length parameter type.
// ----------------------------------------------------------------------------

class sc_length_param
{
public:

             sc_length_param();
             sc_length_param( int );
             sc_length_param( const sc_length_param& );
    explicit sc_length_param( sc_without_context );

    sc_length_param& operator = ( const sc_length_param& );

    friend bool operator == ( const sc_length_param&,
                              const sc_length_param& );
    friend bool operator != ( const sc_length_param&,
			      const sc_length_param& );

    int len() const;
    void len( int );

    const std::string to_string() const;

    void print( ::std::ostream& = ::std::cout ) const;
    void dump( ::std::ostream& = ::std::cout ) const;

private:

    int m_len;
};


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_length_context
//
//  Context type for the length parameter type.
// ----------------------------------------------------------------------------

typedef sc_context<sc_length_param> sc_length_context;


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_length_param::sc_length_param()
{
    *this = sc_length_context::default_value();
}

inline
sc_length_param::sc_length_param( int len_ )
{
    SC_CHECK_WL_( len_ );
    m_len = len_;
}

inline
sc_length_param::sc_length_param( const sc_length_param& a )
    : m_len( a.m_len )
{}

inline
sc_length_param::sc_length_param( sc_without_context )
    : m_len( SC_DEFAULT_WL_ )
{}


inline
sc_length_param&
sc_length_param::operator = ( const sc_length_param& a )
{
    if( &a != this )
    {
	m_len = a.m_len;
    }
    return *this;
}


inline
bool
operator == ( const sc_length_param& a, const sc_length_param& b )
{
    return ( a.m_len == b.m_len );
}

inline
bool
operator != ( const sc_length_param& a, const sc_length_param& b )
{
    return ( a.m_len != b.m_len );
}


inline
int
sc_length_param::len() const
{
    return m_len;
}

inline
void
sc_length_param::len( int len_ )
{
    SC_CHECK_WL_( len_ );
    m_len = len_;
}


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_length_param& a )
{
    a.print( os );
    return os;
}

} // namespace sc_dt


#endif

// Taf!
