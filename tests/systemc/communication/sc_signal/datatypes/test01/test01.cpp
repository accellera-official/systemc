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

 *****************************************************************************/

/*****************************************************************************

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of signals with user-defined datatypes
// -- operator =, operator ==, and operator << must be defined
// -- sc_trace() does not have to be defined (unless used)
// -- here, sc_trace() is not defined and not used

#include "systemc.h"

class my_datatype
{
public:
    my_datatype()
        : m_val( 0 ) {}
    my_datatype( int val_ )
        : m_val( val_ ) {}
    my_datatype( const my_datatype& a )
        : m_val( a.m_val ) {}
    ~my_datatype()
        {}
    my_datatype& operator = ( const my_datatype& a )
        { m_val = a.m_val; return *this; }
    friend bool operator == ( const my_datatype& a, const my_datatype& b )
        { return ( a.m_val == b.m_val ); }
    void print( ostream& os ) const
        { os << m_val; }
private:
    int m_val;
};

ostream&
operator << ( ostream& os, const my_datatype& a )
{
    a.print( os );
    return os;
}

int
sc_main( int, char*[] )
{
    my_datatype a( 123 );
    a.print( cout );
    cout << endl;

    sc_signal<my_datatype> sig;

    return 0;
}
