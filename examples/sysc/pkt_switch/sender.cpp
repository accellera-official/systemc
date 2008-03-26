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


#include "systemc.h"
#include "pkt.h"
#include "sender.h"
#include "stdlib.h"
#include "time.h"

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
 
