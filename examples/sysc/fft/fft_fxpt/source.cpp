/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  source.cpp - This is the implementation file for the synchronous process
               "source".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "source.h"

void source::entry()
{ FILE *fp_real, *fp_imag;

    int tmp_val;

    fp_real = fopen("in_real", "r");
    fp_imag = fopen("in_imag", "r");

  data_valid.write(false);

  while(true)
  { 
    do { wait(); } while ( !(data_req == true) );
    if (fscanf(fp_real,"%d", &tmp_val) == EOF)
    { cout << "End of Input Stream: Simulation Stops" << endl;
        sc_stop();
        break;
	};
    out_real.write(tmp_val);
    if (fscanf(fp_imag,"%d", &tmp_val) == EOF)
   { cout << "End of Input Stream: Simulation Stops" << endl;
        sc_stop();
	break;
	};
    out_imag.write(tmp_val);
    data_valid.write(true);
    do { wait(); } while ( !(data_req == false) );
    data_valid.write(false);
    wait();
  }
}
