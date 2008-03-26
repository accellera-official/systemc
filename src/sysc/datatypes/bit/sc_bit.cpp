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

  sc_bit.cpp -- Bit class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sysc/datatypes/bit/sc_bit.h"
#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/datatypes/bit/sc_logic.h"

#include <stdio.h>


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_bit
//
//  Bit class.
//  Note: VSIA compatibility indicated.
// ----------------------------------------------------------------------------

// support methods

void
sc_bit::invalid_value( char c )
{
    char msg[BUFSIZ];
    sprintf( msg, "sc_bit( '%c' )", c );
    SC_REPORT_ERROR( sc_core::SC_ID_VALUE_NOT_VALID_, msg );
}

void
sc_bit::invalid_value( int i )
{
    char msg[BUFSIZ];
    sprintf( msg, "sc_bit( %d )", i );
    SC_REPORT_ERROR( sc_core::SC_ID_VALUE_NOT_VALID_, msg );
}


// constructors

sc_bit::sc_bit( const sc_logic& a )  // non-VSIA
    : m_val( a.to_bool() )
{}


// assignment operators

sc_bit&
sc_bit::operator = ( const sc_logic& b )  // non-VSIA
{
    return ( *this = sc_bit( b ) );
}


// other methods

void
sc_bit::scan( ::std::istream& is )
{
    bool b;
    is >> b;
    *this = b;
}

} // namespace sc_dt


// Taf!
