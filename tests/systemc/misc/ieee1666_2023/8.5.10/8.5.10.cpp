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

  8.5.10.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// BIND DOES NOT START AT THE POINT IT WAS PREVIOUSLY CALLED IN THE TARGET
//
// 
// We have the following binding:
// 
//        it = m2->ports.bind(hi_sigs);
//        sc_assert(it - m2->ports.begin() == 2);
//
//        it = m2->ports.bind(lo_sigs);
//        sc_assert(it - m2->ports.begin() == 4);
//
// The second assertion is failing, (it - m2->ports.begin()) has a value of 2. 
// This is because the bind() method starts filling in a the front of the vector:
//
//  template< typename BindableIterator >
//  iterator bind( BindableIterator first, BindableIterator last )
//    { return bind( first, last, this->begin() ); }  
//  
//  template< typename BindableIterator >
//  iterator bind( BindableIterator first, BindableIterator last
//               , iterator from )
//    { return sc_vector_do_bind( *this, first, last, from ); }
//
// The only information we have on the library side for the sc_vector instance 
// are begin() and end(). In this case begin() points to element 0, 
// end() points to element 5, since the sc_vector, ports, 
// is constructed to be 4 elements long. So we can’t start filling at the 
// third element, since there is no way to provide that starting point. 
//
// @@@@ ISSUE @@@@
//

#include <systemc.h>

typedef sc_core::sc_vector<sc_core::sc_inout<int> >  port_type;
typedef sc_core::sc_vector<sc_core::sc_signal<int> > signal_type;

struct M : public sc_core::sc_module {
    port_type ports;
    M(sc_core::sc_module_name name, int N) : ports("ports",N) {
    }
};

struct Top : sc_core::sc_module {
    signal_type sigs;
    signal_type hi_sigs;
    signal_type lo_sigs;

    M* m1;
    M* m2;

    Top(sc_core::sc_module_name name)
    : sigs("sigs", 4), hi_sigs("hi_sigs", 2), lo_sigs("lo_sigs",2) {
        m1 = new M("m1",4);
        m2 = new M("m2",4);

	port_type::iterator it;

        // Bind all 4 elements of ports vector to all 4 elements of sigs vector:
	it = m1->ports.bind(sigs);
	sc_assert(it - m1->ports.begin() == 4);
        
	// Bind first 2 elements of ports vector to 2 elements of hi_sigs vector:
	it = m2->ports.bind(hi_sigs);
	sc_assert(it - m2->ports.begin() == 2);

	// Explicit loop equivalent to the above vector bind:
#       if 0
            port_type::iterator   from;
	    signal_type::iterator to;
	    for ( from = m2->ports.begin(), to = hi_sigs.begin();
	        (from != m2->ports.end() && to != hi_sigs.end();
		++form, ++to ) {
		    (*from).bind(*to);
		}
	    }
#       endif

	// Bind last 2 elements of ports vector to 2 elements of lo_sigs vector:
	it = m2->ports.bind(lo_sigs);
        // @@@@ THE ASSERT BELOW SHOULD BE CHECKING FOR 4 WHEN WE FIGURE OUT HOW TO 
        // @@@@ FIX THIS.
	sc_assert(it - m2->ports.begin() == 2);
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
