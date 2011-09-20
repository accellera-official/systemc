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

  sc_fxtype_params.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_fxtype_params.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:58  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#include "sysc/datatypes/fx/sc_fxtype_params.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_fxtype_params
//
//  Fixed-point type parameters class.
// ----------------------------------------------------------------------------

const std::string
sc_fxtype_params::to_string() const
{
    std::string s;

    char buf[BUFSIZ];

    s += "(";
    std::sprintf( buf, "%d", m_wl );
    s += buf;
    s += ",";
    std::sprintf( buf, "%d", m_iwl );
    s += buf;
    s += ",";
    s += sc_dt::to_string( m_q_mode );
    s += ",";
    s += sc_dt::to_string( m_o_mode );
    s += ",";
    std::sprintf( buf, "%d", m_n_bits );
    s += buf;
    s += ")";

    return s;
}


void
sc_fxtype_params::print( ::std::ostream& os ) const
{
    os << to_string();
}

void
sc_fxtype_params::dump( ::std::ostream& os ) const
{
    os << "sc_fxtype_params" << ::std::endl;
    os << "(" << ::std::endl;
    os << "wl     = " << m_wl << ::std::endl;
    os << "iwl    = " << m_iwl << ::std::endl;
    os << "q_mode = " << m_q_mode << ::std::endl;
    os << "o_mode = " << m_o_mode << ::std::endl;
    os << "n_bits = " << m_n_bits << ::std::endl;
    os << ")" << ::std::endl;
}

} // namespace sc_dt


// Taf!
