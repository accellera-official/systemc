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

  pkt.h - This file defines a SystemC structure "pkt".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#ifndef PKT_H_INCLUDED
#define PKT_H_INCLUDED

#include "systemc.h"

struct pkt {
       sc_int<8> data;
       sc_int<4> id;
       bool dest0;
       bool dest1;
       bool dest2;
       bool dest3;

       inline bool operator == (const pkt& rhs) const
   {
     return (rhs.data == data && rhs.id == id && rhs.dest0 == dest0 && rhs.dest1 == dest1 && rhs.dest2 == dest2 && rhs.dest3 == dest3);
   }

};

inline
ostream&
operator << ( ostream& os, const pkt& /* a */ )
{
    os << "streaming of struct pkt not implemented";
    return os;
}

inline
void
#if defined(SC_API_VERSION_STRING)
    sc_trace( sc_trace_file* tf, const pkt& a, const std::string& name )
#else
    sc_trace( sc_trace_file* tf, const pkt& a, const sc_string& name )
#endif
{
  sc_trace( tf, a.data, name + ".data" );
  sc_trace( tf, a.id, name + ".id" );
  sc_trace( tf, a.dest0, name + ".dest0" );
  sc_trace( tf, a.dest1, name + ".dest1" );
  sc_trace( tf, a.dest2, name + ".dest2" );
  sc_trace( tf, a.dest3, name + ".dest3" );
}
#endif // PKT_H_INCLUDED
