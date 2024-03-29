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

  5.12.8.4.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// If the binding was done correctly there should be no error messages. Note that the
// LRM contains no definition for bus_interface, and a null body was added to properly
// compile.

#include <systemc>


class bus_interface {};

class peripheral_interface : virtual public sc_core::sc_interface {
  public:
    virtual void peripheral_write( int addr, int data ) =  0;
    virtual void peripheral_read( int addr, int& data ) =  0;
};

class bus_channel : public bus_interface, public sc_core::sc_module {
  public:
    sc_core::sc_port<peripheral_interface,0> peripheral_port;

    SC_CTOR(bus_channel) {
        SC_THREAD(action);
    }

  private:
    void action() {
        for ( size_t i = 0; i < peripheral_port.size(); ++i ) {
	    peripheral_port[i]->peripheral_write(0,i);
	    int j;
	    peripheral_port[i]->peripheral_read(0,j);
	    if ( i != j ) {
	        std::cout << "memory read/write mismatch " << j << " != " << i << std::endl;
	    }
	}
    }
};

class memory : public peripheral_interface, public sc_core::sc_module {
  public:
    SC_CTOR(memory) {}

    virtual void peripheral_write( int addr, int data ) { storage[addr] = data; }
    virtual void peripheral_read( int addr, int& data ) { data = storage[addr]; }
    int storage[20];
};

SC_MODULE(top_level) {
    bus_channel bus;
    memory ram0, ram1, ram2, ram3;

    SC_CTOR(top_level) : bus("bus"), ram0("ram0"), ram1("ram1"),  ram2("ram2"), ram3("ram3")
    {
        bus.peripheral_port(ram0);
        bus.peripheral_port(ram1);
        bus.peripheral_port(ram2);
        bus.peripheral_port(ram3);
    }
};
   
int sc_main( int argc, char* argv[] ) { 

    top_level tlm("tlm");
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
