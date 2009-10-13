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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
Subject: range() of sc_biguint
From: "Masahiro Taniguchi" <taniguchi.masahiro@lsi.melco.co.jp>
Date: Mon, 12 Nov 2001 16:29:04 +0900
To: "systemc-forum" <systemc-forum@systemc.org>

Hi all,

I found a strange simulation result regarding sc_biguint.
I attached the example about it.
The example described 3 case.
 1. Write non-0
 2. Write 0 without cast
 3. Write 0 with cast (sc_biguint<8>)
The wrong result appears in the 2nd case.
Is this a bug of SystemC?

-------Environment-------
 SystemC V1.0.2 and V2.0
 GCC 2.95.2
 Sun Solaris
-------------------------

Regards,

Masahiro Taniguchi
Mitsubishi Electric Corporation
JAPAN

------------------------------------------------------------
*/

#include "systemc.h"
 
int
sc_main( int, char*[] )
{
    int i;
    int index0,index1;
    sc_biguint<128> A;
    sc_uint<16> B;
    sc_biguint<128> Y;
 
    A = 0;
    Y = 0;
    cout << "A = " << A.to_string(SC_HEX) << endl;
    cout << "Y = " << Y.to_string(SC_HEX) << endl << endl;

    // 1st Case 
    A = "0xffffffffffffffffffffffffffffffff";
    B = 0;
 
    for(i=15;i>=0;i--)
    {
        index0 = 8*(i+1)-1;
        index1 = 8* i;
        if(B[i] == 0) {
            Y.range(index0,index1) = A.range(index0,index1);
        }
    }
 
    cout << "A = " << A.to_string(SC_HEX) << endl;
    cout << "B = " << hex << B << endl;
    cout << "Y = " << Y.to_string(SC_HEX) << endl << endl;

    // 2nd Case 
    A = 0;
    B = 0;

    for(i=15;i>=0;i--)
    {
        index0 = 8*(i+1)-1;
        index1 = 8* i;
        if(B[i] == 0) {
            Y.range(index0,index1) = A.range(index0,index1);
        }
    }
 
    cout << "A = " << A.to_string(SC_HEX) << endl;
    cout << "B = " << hex << B << endl;
    cout << "Y = " << Y.to_string(SC_HEX) << endl << endl;

    // 3rd Case
    A = 0;
    B = 0;
 
    for(i=15;i>=0;i--)
    {
        index0 = 8*(i+1)-1;
        index1 = 8* i;
        if(B[i] == 0) {
            Y.range(index0,index1) = (sc_biguint<8>)A.range(index0,index1);
        }
    }
 
    cout << "A = " << A.to_string(SC_HEX) << endl;
    cout << "B = " << hex << B << endl;
    cout << "Y = " << Y.to_string(SC_HEX) << endl << endl;
 
    return(0);
}
