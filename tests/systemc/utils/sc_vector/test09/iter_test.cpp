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

  iter_test.cpp -- sc_vector iterator comparisons

  Original Author: Philipp A. Hartmann, OFFIS, 2014-08-21

 *****************************************************************************/
#include <systemc.h>

SC_MODULE(mod)
{
    sc_in<bool> p;
    SC_CTOR(mod){}
};

int sc_main(int,char*[])
{
    typedef sc_vector<mod> module_vec;
    typedef sc_vector_assembly<mod,sc_in<bool> > module_port_vec;

    module_vec mv("sigs",5);
    module_vec::const_iterator citr = mv.begin();
    module_vec::iterator       itr = mv.begin();

    module_port_vec mpv = sc_assemble_vector(mv, &mod::p);
    module_port_vec::const_iterator cpitr = mpv.cbegin();
    module_port_vec::iterator       pitr  = mpv.begin();

    sc_assert(itr == citr);
    sc_assert(citr == itr);
    sc_assert(!(itr != citr));
    sc_assert(!(citr != itr));

    sc_assert(itr == cpitr);
    sc_assert(citr == pitr);
    sc_assert(cpitr == itr);
    sc_assert(pitr == citr);
    sc_assert(!(itr != cpitr));
    sc_assert(!(citr != pitr));
    sc_assert(!(cpitr != pitr));
    sc_assert(!(pitr != itr));

    sc_assert(itr < mv.end());
    sc_assert(!(itr > mv.cend()));
    sc_assert(citr < mv.end());
    sc_assert(!(citr > mv.cend()));

    ++citr;
    sc_assert(itr != citr);
    sc_assert(citr != itr);
    sc_assert(!(itr == citr));
    sc_assert(!(citr == itr));

    sc_assert(!(itr < itr));
    sc_assert(!(itr > itr));
    sc_assert(itr < citr);
    sc_assert(citr > itr);

    sc_assert(1 == citr - mv.begin());
    sc_assert(0 == pitr - mv.cbegin());
    itr += citr - mpv.begin();
    sc_assert(1 == itr - mpv.cbegin());

    sc_assert(citr == itr);
    sc_assert(itr <= citr);
    sc_assert(citr <= itr);
    sc_assert(cpitr <= itr);
    sc_assert(!(itr <= cpitr));

    itr++;
    cpitr = pitr += itr - mv.begin();
    sc_assert(itr == pitr);
    sc_assert(itr >= citr);
    sc_assert(!(citr >= pitr));

    cout << "\nSuccess" << endl;
    return 0;
}
