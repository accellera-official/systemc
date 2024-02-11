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

  5.7.3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>


class if_class : public sc_core::sc_interface {
  public:
    virtual const sc_core::sc_event& ev_func() const = 0;
};

class chan_class : public if_class, public sc_core::sc_prim_channel {
  public:
    virtual const sc_core::sc_event& ev_func() const { return an_event; }
  private:
    sc_core::sc_event an_event;
};

template<int N=1>
class port_class : public sc_core::sc_port<if_class,N> {
  public:
    sc_core::sc_event_finder& event_finder() const {
      return *new sc_core::sc_event_finder_t<if_class>(*this, &if_class::ev_func);
    }
};

SC_MODULE(mod_class)
{
    port_class<1> port_var;
    port_class<0> multiport;
  
    SC_CTOR(mod_class) {
        SC_METHOD(method);
        sensitive << port_var.event_finder();
    }

    void method() { std::cout << "method" << std::endl; }

    void end_of_elaboration() {
        SC_METHOD(method2);
	for ( size_t i = 0; i < multiport.size(); ++i ) {
	    sensitive << multiport[i]->ev_func();
	}
    }

    void method2() { std::cout << "method2" << std::endl; }
};
        

int sc_main( int argc, char* argv[] ) { 

    mod_class  mc("mc");
    chan_class sig0;
    chan_class sig1;
    chan_class sig2;

    mc.port_var(sig0);
    mc.multiport(sig1);
    mc.multiport(sig2);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    std::cout << "program completed" << std::endl;
    return 0;
}
