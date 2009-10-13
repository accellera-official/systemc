/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
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
Nov/29/00 Ulli Holtmann

Assignment of values other than 0 or 1 to an sc_bit results in a core dump
on Sparc SC5.0 as well as g++. I used SystemC 1.0.1

I can understand that only 0 and 1 make sense, so please either forbid
assignment from an integer and cast the integer to bool first. A core dump is
a bit to drastic.

Example:
*/

#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
    sc_bit res;

    // works fine
    res = 0;           cout << res << "\n";
    res = 1;           cout << res << "\n";
    res = bool(2);     cout << res << "\n";

    // results in a core dump
    res = sc_bit(2);   cout << res << "\n";
    res = 2;          cout << res << "\n";

    return 0;
}


/*
Dec/7/00 ulrich

Hi Gene, 

I agree that the assignment of values other than 0,1 doesn't make much sense, so please go ahead
and forbid it in one way or another. However, such an illegal assignment may easily happen in a 
user-program because the compiler accepts it. It very easy to write. 

The point I dislike is that the class library immediately core dumps without any warning or
explanation. Does SystemC throw an exception? I don't know and I most likely will not write
an exception handler, therefore I will never know. I just see that the SystemC kernel core
dumps.

What about an assert statement such like
	assert(v==0 || v==1);
That should me as the user a precise and reasonable explanation that I made a mistake. I could
also accept an error message like E200x or so coming like when I enter illlegal bit characters,
e.g. sc_bv<10>="102abd00". But please, not just a core dump.



Jan/9/01 ulrich

Hi Gene, I still only ask that the program does not core dump and instead prints an error
message or warning like it does for sc_logic. I only object to the core dump itself. Example:


int main(int argc, char* arg[]) 
{
  sc_logic l (5);
  cout << l << "\n";

  sc_bit b(2);
  cout << b << "\n";
}

Both are invalid assignments. The first one prompt a warning (1006), the second a core
dump. Both should prompt warnings/run time errors.

I reduce the prioity to B2 because it's now only a matter of properly reporting an error.

Other than
this, I can share your view that assigning 2 is a user error. 
*/
