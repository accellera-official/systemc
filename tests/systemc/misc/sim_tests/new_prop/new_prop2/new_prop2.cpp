/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  new_prop2.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

static int (*print_func)(const char*, ...);

struct new_prop : sc_module {
    sc_in<bool> i[16];

    void my_print(const char*);

    void async0() { my_print("async0"); }
    void async1() { my_print("async1"); }
    void async2() { my_print("async2"); }
    void async3() { my_print("async3"); }

    void aproc0() { wait(); while (1) { my_print("aproc0"); wait(); } }
    void aproc1() { wait(); while (1) { my_print("aproc1"); wait(); } }
    void aproc2() { wait(); while (1) { my_print("aproc2"); wait(); } }
    void aproc3() { wait(); while (1) { my_print("aproc3"); wait(); } }

    SC_CTOR(new_prop) {
        SC_METHOD(async0);
        sensitive_neg << i[4];
        sensitive_neg << i[12];
        sensitive_neg << i[13];
        sensitive_neg << i[14];
        sensitive_neg << i[15];

        SC_METHOD(async1);
        sensitive_pos << i[1];
        sensitive_pos << i[5];
        sensitive_pos << i[7];
        sensitive_pos << i[9];

        SC_METHOD(async2);
        sensitive_neg << i[5];
        sensitive_neg << i[6];
        sensitive_pos << i[13];
        sensitive_pos << i[15];
        
        SC_METHOD(async3);
        sensitive_pos << i[3];
        sensitive_neg << i[5];
        sensitive_neg << i[7];
        sensitive_pos << i[11];

        SC_THREAD(aproc0);
        sensitive_pos << i[2];

        SC_THREAD(aproc1);
        sensitive_pos << i[3];
        sensitive_neg << i[12];
        sensitive_neg << i[13];
        sensitive_neg << i[14];
        sensitive_neg << i[15];

        SC_THREAD(aproc2);
        sensitive_neg << i[8];
        sensitive_neg << i[9];
        sensitive_neg << i[10];
        sensitive_neg << i[11];

        SC_THREAD(aproc3);
        sensitive_pos << i[6];
        sensitive_pos << i[7];
        sensitive_pos << i[10];
        sensitive_pos << i[11];
        sensitive_pos << i[14];
        sensitive_pos << i[15];
    }
};

void
new_prop::my_print(const char* p)
{
    (*print_func)("%s executed on:\n", p);
    for (int j = 0; j < 16; ++j) {
        if (i[j].posedge()) {
            (*print_func)("\tposedge i[%d]\n", j);
        } else if (i[j].negedge()) {
            (*print_func)("\tnegedge i[%d]\n", j);
        }
    }
}

static int
dont_print(const char* fmt, ...)
{
    return 0;
}

int
sc_main(int,char**)
{
    // sc_clock i[16];
    sc_signal<bool> i[16];

    new_prop np("np");

    for( int j = 0; j < 16; ++ j ) {
        np.i[j]( i[j] );
    }

    for (int j = 0; j < 16; ++j) {
        i[j] = 0;
    }

    print_func = &dont_print;
    sc_initialize();

    print_func = &printf;

    for (int k = 0; k < 16; ++k) {
        i[k] = ! i[k].read();
        sc_cycle(1);
        i[k] = ! i[k].read();
        sc_cycle(1);
    }

    fflush( stdout );

    return 0;
}
