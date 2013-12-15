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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>


SC_MODULE(xyz) {
  
    sc_in<sc_int<8> > A;
    sc_out<sc_lv<8> > X;

    void convert(const sc_lv<8>& I, sc_int<8>& O) {
	O=I;
    }

    void convert2(const sc_int<8>& I, sc_lv<8>& O) {
	O=I;
    }

    void entry() {
	sc_int<8> tmp;
	sc_lv<8> tmp2;

	convert(sc_lv<8>(A.read()),tmp);
	convert2(tmp,tmp2);
	X.write(tmp2);
    }

    SC_CTOR(xyz) {
	SC_METHOD(entry);
	sensitive << A;
    }
};


#define NS * 1e-9

int sc_main(int ac, char *av[])
{
  //Signals
  sc_signal<sc_int<8> > A;
  sc_signal<sc_lv<8> > X;

  xyz UUT("UUT");
  UUT.A(A);
  UUT.X(X);

  sc_start(0, SC_NS);

  A.write(1);
  sc_start( 10, SC_NS );
  cout << X.read() << endl;

  A.write(2);
  sc_start( 10, SC_NS );
  cout << X.read() << endl;

  A.write(3);
  sc_start( 10, SC_NS );
  cout << X.read() << endl;

  return 0;
}
