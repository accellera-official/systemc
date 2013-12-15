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

  select.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#define WIDTH 32
#define COUNT 100000

int sc_main( int ac, char* av[] )
{
  sc_uint<WIDTH> Bx, By;

  for(unsigned int i = 0; i < WIDTH; i++){
    cout << "i = " << i << endl;
    for(unsigned int j=0; j < COUNT; j++ ){
      
      /*    ( By.range(WIDTH-1, i+1), By.range(i,0) ) = Bx;
	    assert( By == Bx );
	    */
      
      Bx = j;
      assert( (bool) Bx[i] == (j & (1<<i)? 1 : 0) );
      By[i] = Bx[i];
      assert( (bool) By[i] == (j & (1<<i)? 1 : 0) );
    }
  }

  return 0;
}

