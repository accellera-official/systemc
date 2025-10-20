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

  5.13.7.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/


#include <systemc>

struct i_f : virtual sc_core::sc_interface {
    virtual void print() = 0;
};

struct Chan : sc_core::sc_channel, i_f {
    SC_CTOR(Chan) {}
    virtual void print() { std::cout << "I'm Chan.name=" << name() << std::endl; }

#ifndef IEEE_1666_STRICT
private:
    // Suppress warning about hiding sc_object::print() by Chan::print(), so we need to
    // determine if that is okay, or do we want to change the signature.
    using sc_core::sc_object::print;
#endif  // IEEE_1666_STRICT
};

struct Caller : sc_core::sc_module {
    sc_core::sc_port<i_f> p;
    SC_CTOR(Caller) { }
};

struct Bottom : sc_core::sc_module {
    sc_core::sc_export<i_f> xp;
    Chan ch;
    SC_CTOR(Bottom) : xp("xp"), ch("ch") {
        xp.bind(ch);
    }
};

struct Middle : sc_core::sc_module {
    sc_core::sc_export<i_f> xp;
    Bottom* b_p;
    SC_CTOR(Middle) : xp("xp") {
        b_p = new Bottom("b");
	xp.bind(b_p->xp);
	b_p->xp->print();
    }
};

struct Top : sc_core::sc_module {
    Caller* c;
    Middle* m;
    SC_CTOR(Top) {
        c = new Caller("c");
	m = new Middle("m");
	c->p(m->xp);
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;

    return 0;
}
