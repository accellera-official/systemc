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

  sc_bit.cpp -- Bit class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_bit.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.6  2006/04/12 20:17:52  acg
//  Andy Goodrich: enabled deprecation message for sc_bit.
//
// Revision 1.5  2006/01/25 00:31:15  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.4  2006/01/24 20:50:55  acg
// Andy Goodrich: added warnings indicating that sc_bit is deprecated and that
// the C bool data type should be used in its place.
//
// Revision 1.3  2006/01/13 18:53:53  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#include "sysc/datatypes/bit/sc_bit.h"
#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/utils/sc_utils_ids.h"
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
    std::sprintf( msg, "sc_bit( '%c' )", c );
    SC_REPORT_ERROR( sc_core::SC_ID_VALUE_NOT_VALID_, msg );
}

void
sc_bit::invalid_value( int i )
{
    char msg[BUFSIZ];
    std::sprintf( msg, "sc_bit( %d )", i );
    SC_REPORT_ERROR( sc_core::SC_ID_VALUE_NOT_VALID_, msg );
}


// constructors

sc_bit::sc_bit( const sc_logic& a )  // non-VSIA
    : m_val( a.to_bool() )
{
   sc_deprecated_sc_bit();
}


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

void sc_deprecated_sc_bit()
{
    static bool warn_sc_bit_deprecated=true;
    if ( warn_sc_bit_deprecated )
    {
        warn_sc_bit_deprecated=false;
	SC_REPORT_INFO(sc_core::SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_bit is deprecated, use bool instead");
    }
}

} // namespace sc_dt


// Taf!
