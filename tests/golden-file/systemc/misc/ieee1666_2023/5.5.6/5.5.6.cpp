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

  5.5.6.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

int f() { std::cout << "f()" << std::endl; return 0; }

struct Functor {
    typedef int result_type;
    result_type operator() ();
};

Functor::result_type Functor::operator() () { return f(); }

int h(int a, int& b, const int& c) { std::cout << "h(a,b,c)" << std::endl; return 0; }

struct MyMod : sc_core::sc_module {
    sc_core::sc_signal<int> SC_NAMED(sig);
    void g() { std::cout << "MyMod::g()" << std::endl;}

    SC_CTOR(MyMod) {
        SC_THREAD(T);
    }

    int ret;

    void T() {
        using namespace sc_core;

	sc_spawn(&f);

	sc_process_handle handle = sc_spawn(&f);

	Functor fr;
	sc_spawn(&ret, fr);

	sc_spawn_options opt;
	    opt.spawn_method();
	    opt.set_sensitivity(&sig);
	    opt.dont_initialize();
        sc_spawn(f, "f1", &opt);
	sc_spawn(&ret,fr,"f2",&opt);
	sc_spawn(sc_bind(&MyMod::g,this));

	int A=0, B, C;
	sc_spawn(&ret,sc_bind(&h,A,sc_ref(B),sc_cref(C)));
    }
};
        

int sc_main( int argc, char* argv[] ) { 

    MyMod mm("mm");
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
