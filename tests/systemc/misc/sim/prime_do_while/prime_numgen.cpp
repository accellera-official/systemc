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

  prime_numgen.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/******************************************************************************/
/***			Prime Number Generator				    ***/
/*** 		Implementation Filename:  prime_numgen.cc     		    ***/
/******************************************************************************/

#include "prime_numgen.h"

void prime_numgen::entry()
{

  static unsigned int	p2 = 1,
                	p3 = 0, 
			p5 = 0, 
			p7 = 0, 
			p11 = 0, 
			p13 = 0, 
			p17 = 0, 
			p19 = 0, 
			p23 = 0, 
			p29 = 0;

  while (true) {

    if (p2 && !p3 && !p5 && !p7 && !p11 && !p13 && !p17 && !p19 && !p23 && !p29)
    { 
      prime.write(p2);
      wait();
    }

    if (p7 && p13) { p7--, p13--, p17++; }
    else if (p5 && p17) { p5--, p17--, p2++, p3++, p13++; }
    else if (p3 && p17) { p3--, p17--, p19++; }
    else if (p2 && p19) { p2--, p19--, p23++; }
    else if (p3 && p11) { p3--, p11--, p29++; }
    else if (p29)       { p29--, p7++, p11++; }
    else if (p23)       { p23--, p5++, p19++; }
    else if (p19)       { p19--, p7++, p11++; }
    else if (p17)       { p17 = 0; }
    else if (p13)       { p13--, p11++; }
    else if (p11)       { p11--, p13++; }
    else if (p2 && p7)  { p2--, p7--, p3++, p5++; }
    else if (p2)        { p2--, p3++, p5++; }
    else 		{ p5++, p11++; }
  }
}
