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

  sc_fxcast_switch.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_fxcast_switch.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:53:57  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_FXCAST_SWITCH_H
#define SC_FXCAST_SWITCH_H


#include "sysc/datatypes/fx/sc_context.h"


namespace sc_dt
{

// classes defined in this module
class sc_fxcast_switch;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxcast_switch
//
//  Fixed-point cast switch class.
// ----------------------------------------------------------------------------

class sc_fxcast_switch
{

public:

             sc_fxcast_switch();
             sc_fxcast_switch( sc_switch );
             sc_fxcast_switch( const sc_fxcast_switch& );
    explicit sc_fxcast_switch( sc_without_context );

    sc_fxcast_switch& operator = ( const sc_fxcast_switch& );

    friend bool operator == ( const sc_fxcast_switch&,
			      const sc_fxcast_switch& );
    friend bool operator != ( const sc_fxcast_switch&,
			      const sc_fxcast_switch& );

    const std::string to_string() const;

    void print( ::std::ostream& = ::std::cout ) const;
    void dump( ::std::ostream& = ::std::cout ) const;

private:

    sc_switch m_sw;

};


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_fxcast_context
//
//  Context type for the fixed-point cast switch parameter.
// ----------------------------------------------------------------------------

typedef sc_context<sc_fxcast_switch> sc_fxcast_context;


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_fxcast_switch::sc_fxcast_switch()
{
    *this = sc_fxcast_context::default_value();
}

inline
sc_fxcast_switch::sc_fxcast_switch( sc_switch sw_ )
: m_sw( sw_ )
{}

inline
sc_fxcast_switch::sc_fxcast_switch( const sc_fxcast_switch& a )
: m_sw( a.m_sw )
{}

inline
sc_fxcast_switch::sc_fxcast_switch( sc_without_context )
: m_sw( SC_DEFAULT_CAST_SWITCH_ )
{}


inline
sc_fxcast_switch&
sc_fxcast_switch::operator = ( const sc_fxcast_switch& a )
{
    if( &a != this )
    {
        m_sw = a.m_sw;
    }
    return *this;
}


inline
bool
operator == ( const sc_fxcast_switch& a, const sc_fxcast_switch& b )
{
    return ( a.m_sw == b.m_sw );
}


inline
bool
operator != ( const sc_fxcast_switch& a, const sc_fxcast_switch& b )
{
    return ( a.m_sw != b.m_sw );
}


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_fxcast_switch& a )
{
    a.print( os );
    return os;
}

} // namespace sc_dt


#endif

// Taf!
