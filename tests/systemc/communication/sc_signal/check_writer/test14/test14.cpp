/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test14.cpp -- sc_writer_policy: writer check with terminated writer

  Original Author: Philipp A. Hartmann, OFFIS, 2014-11-15

 *****************************************************************************/

// see https://github.com/OSCI-WG/systemc/issues/104

#include <systemc>

using sc_core::SC_NS;

SC_MODULE(dut)
{
    sc_core::sc_signal<int> sig;

    SC_CTOR(dut)
      : sig("sig")
    {
        SC_THREAD(p1);
        SC_THREAD(p2);
    }

    void p1() {
        wait( 10, SC_NS );
        sig.write(1);
    }

    void p2() {
        wait( 1, SC_NS );
        sig.write(0);
    }
};

int sc_main( int, char*[] )
{
    dut top("dut");
    sc_core::sc_start();
    std::cout << "Program completed" << std::endl;
    return 0;
}
