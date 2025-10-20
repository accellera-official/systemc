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

  6.25.3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>
	
class U {
  public:
#ifndef IEEE_1666_STRICT
    U(int val=0) : ptr(new int{val}) {}
    U(const U& that) : U(*that.ptr) {}
    ~U() { delete ptr; }
#else
    U(int val=0) {
        ptr = new int;
	*ptr = val;
    }
#endif

    int get() const { return *ptr; }
    void set(int i) { *ptr = i; }

    const U& operator = ( const U& arg ) {
        *(this->ptr) = *arg.ptr;
        return *this;
    }

  private:
    int* ptr;
};

std::ostream& operator << ( std::ostream& os, const U& arg ) { return os << arg.get(); }

SC_MODULE(M1) {
    sc_core::sc_fifo_out<U> fifo_out;

    SC_CTOR(M1) {
        SC_THREAD(producer);
    }

    void producer() {
        U u;
	for ( int i = 0; i < 4; ++i ) {
	    u.set(i);
	    bool status;
	    do {
	        wait(1.0, sc_core::SC_NS);
		status = fifo_out.nb_write(u);
	    } while (!status);
	}
    }
};

SC_MODULE(M2) {
    sc_core::sc_fifo_in<U> fifo_in;

    SC_CTOR(M2) {
        SC_THREAD(consumer);
    }

    void consumer() {
        for (;;) {
	    wait(fifo_in.data_written_event());
	    U u;
	    bool status = fifo_in.nb_read(u);
	    std::cout << u << " ";
	}
    } 
};

SC_MODULE(Top) {
    sc_core::sc_fifo<U> fifo;
    M1 m1;
    M2 m2;

    SC_CTOR(Top) : m1("m1"), m2("m2") {
        m1.fifo_out(fifo);
	m2.fifo_in(fifo);
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");
    sc_core::sc_start();

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
