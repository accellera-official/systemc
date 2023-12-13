/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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

#include <sstream>

// explicit template instantiations
namespace sc_core {
template class SC_API sc_phash<void*, const sc_dt::sc_fxtype_params*>;
} // namespace sc_core

namespace sc_dt {

template class SC_API sc_global<sc_fxtype_params>;
template class SC_API sc_context<sc_fxtype_params>;

// ----------------------------------------------------------------------------
//  CLASS : sc_fxtype_params
//
//  Fixed-point type parameters class.
// ----------------------------------------------------------------------------

std::string
sc_fxtype_params::to_string() const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}


void
sc_fxtype_params::print( ::std::ostream& os ) const
{
    os << "("
       << m_wl << ","
       << m_iwl << ","
       << m_q_mode << ","
       << m_o_mode << ","
       << m_n_bits
       << ")";
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
