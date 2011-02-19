/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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

/*****************************************************************************

  sc_length_param.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-19

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_length_param.cpp,v $
// Revision 1.2  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#include "sysc/datatypes/int/sc_length_param.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_length_param
//
//  Length parameter type.
// ----------------------------------------------------------------------------

const std::string
sc_length_param::to_string() const
{
    std::string s;

    char buf[BUFSIZ];

    s += "(";
    std::sprintf( buf, "%d", m_len );
    s += buf;
    s += ")";

    return s;
}


void
sc_length_param::print( ::std::ostream& os ) const
{
    os << to_string();
}

void
sc_length_param::dump( ::std::ostream& os ) const
{
    os << "sc_length_param" << ::std::endl;
    os << "(" << ::std::endl;
    os << "len = " << m_len << ::std::endl;
    os << ")" << ::std::endl;
}

} // namespace sc_dt


// Taf!
