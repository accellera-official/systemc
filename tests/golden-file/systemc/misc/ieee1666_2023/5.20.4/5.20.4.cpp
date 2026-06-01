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

  5.20.4.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

using namespace sc_core;

int sc_main( int argc, char* argv[] ) { 

    sc_signal<int> sig;

    sc_attribute<int> a("number",42);
    sig.add_attribute(a);

    sc_attr_cltn& c = sig.attr_cltn();
    for ( sc_attr_cltn::iterator i = c.begin(); i < c.end(); ++i ) {
        sc_attribute<int>* ap = dynamic_cast<sc_attribute<int>*>(*i);
	if ( ap ) {
	    std::cout << ap->name() << "  " << ap->value << std::endl;
	}
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
