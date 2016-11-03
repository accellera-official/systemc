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

  sender.cpp - This is the implementation file for the synchronous process
               "sender".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <stdlib.h>
#include <time.h>
#include "sender.h"

void sender:: entry()
{
   pkt pkt_data;
   sc_uint<4> dest;

   srand((unsigned)time(NULL));
   wait(8);
   while(true)
     {
       /////generate an 8-bit random value for data//////////
       pkt_data.data = rand()%255;

       ////stamp the sender's id
       pkt_data.id = source_id.read();

      
       /////send it to 1 or more(<=4) destinations//////////////
       dest = rand()%15 + 1;
       pkt_data.dest0 = dest[0];
       pkt_data.dest1 = dest[1]; 
       pkt_data.dest2 = dest[2];
       pkt_data.dest3 = dest[3];

       pkt_out.write(pkt_data);

        cout << ".........................." << endl;
	cout << "New Packet Sent" << endl;
	cout << "Destination Addresses: ";
        if (dest[0]) cout << 1 << " " ;
        if (dest[1]) cout << 2 << " ";
        if (dest[2]) cout << 3 << " ";
	if (dest[3]) cout << 4 << " ";
        cout << endl;
	cout << "Packet Value: " << (int)pkt_data.data << endl;
	cout << "Sender ID: " << (int)source_id.read() + 1 << endl;
	cout << ".........................." << endl;      
       
       wait(); 

       /////wait for 1 to 3 clock periods/////////////////////
       wait(1+(rand()%3));
     }

}
 
