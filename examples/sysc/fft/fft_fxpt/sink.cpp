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

  sink.cpp - This is the implementation file for the synchronous process
             "sink".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "sink.h"

void sink::entry()
{
 sc_int<16> tmp;
 int tmp_out;
 fp_real = fopen("out_real","w");
 fp_imag = fopen("out_imag","w");

 data_ack.write(false);

 while(true)
 { 
   do { wait(); } while ( !(data_ready == true) );
   tmp = in_real.read();
   tmp_out = tmp.to_int();
   fprintf(fp_real,"%d  \n",tmp_out);
   tmp = in_imag.read();
   tmp_out = tmp.to_int();
   fprintf(fp_imag,"%d  \n",tmp_out);
   data_ack.write(true);
   do { wait(); } while ( !(data_ready == false) );
   data_ack.write(false); 
 }
}
