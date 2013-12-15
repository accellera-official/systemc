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

  array_range.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /*******************************************/
                /* Implementation Filename:  array_range.cc  */
                /*******************************************/
 
#include "array_range.h"
 
void
array_range::entry()
{
  bool_vector8	a;
  bool_vector4	b;
  bool_vector4	c;
  bool_vector8	d;
  bool_vector8	e;
  bool_vector8	f;
  // bool_vector0 	nullbv;	// Null vector to make scalar concat work

  wait();

  a = in1.read();

  b = a.range(7,4);	c = a.range(3,0);			// sub vectors
 
  d = a.range(0,7);						// bit reverse

  // e = (nullbv, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);	
  e = (a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);	
       						// bit reverse concat

  f = (a.range(3,1), a.range(7,6), a[0], a.range(4,5));		// shuffle

  o1.write(b);
  o2.write(c);
  o3.write(d);
  o4.write(e);
  o5.write(f);

  wait();

}
