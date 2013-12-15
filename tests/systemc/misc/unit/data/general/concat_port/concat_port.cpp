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

  concat_port.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /*********************************************/
                /* Implementation Filename:  concat_port.cc  */
                /*********************************************/
 
#include "concat_port.h"
 
void
concat_port::entry()
{
  bool_vector8	tmp_a;
  bool_vector8	tmp_b;

  while (true) {

    done.write(0);
    do { wait(); } while (ready != 1);

    tmp_a = a.read();
    tmp_b = b.read();

    switch (mode.read()) {

      case 0: 	c.write( (tmp_a.range(7,4), tmp_b.range(7,4)) );
       		d.write( (tmp_a, tmp_b) );
    		break;

      case 1: 	c.write( (tmp_a.range(0,7)) );
       		d.write( (tmp_a.range(0,7), tmp_b.range(0,7)) );
    		break;

      case 2: 	c.write( (tmp_a[0], tmp_b.range(1,4), tmp_a.range(7,5)) );
       		d.write( (tmp_a.range(7,4), tmp_b.range(7,4), 
		        tmp_a.range(3,0), tmp_b.range(3,0)) );
    		break;

      case 3: 	c.write( ("1", tmp_b.range(2,0), "0", tmp_a.range(2,0)) );
       		d.write( ("11", tmp_a.range(6,0), tmp_b.range(6,0)) );
    		break;

      default:	cout << "Error: Mode " << mode.read()
		     << " does not exist!" << endl; 
		break;

    }
    done.write(1);
    wait();
  }
}
