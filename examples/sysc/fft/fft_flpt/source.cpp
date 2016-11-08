/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  source.cpp - This is the implementation file for the synchronous process
               "source" .

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

  float tmp_val;

  fp_real = fopen("in_real", "r");
  fp_imag = fopen("in_imag", "r");

  data_valid.write(false);

  while(true)
  { 
    do { wait(); } while ( !(data_req == true) );
    if (fscanf(fp_real,"%f \n",&tmp_val) == EOF)
      { cout << "End of Input Stream: Simulation Stops" << endl;
        sc_stop();
        break;
	};
    out_real.write(tmp_val);
    if (fscanf(fp_imag,"%f \n",&tmp_val) == EOF)
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
