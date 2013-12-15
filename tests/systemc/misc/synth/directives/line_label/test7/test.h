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

  test.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Common interface file for test cases 
   Author: PRP 
   */

SC_MODULE( test )
{
    SC_HAS_PROCESS( test );

    sc_in_clk clk;

        // Input Reset Port
        const sc_signal<bool>& reset_sig;

        // Input Data Ports
	const sc_signal<int>& i1;
	const sc_signal<int>& i2;
	const sc_signal<int>& i3;
	const sc_signal<int>& i4;
	const sc_signal<int>& i5;

        // Input Control Ports
	const sc_signal<bool>& cont1;
	const sc_signal<bool>& cont2;
	const sc_signal<bool>& cont3;

        // Output Data Ports
	sc_signal<int>& o1;
	sc_signal<int>& o2;
	sc_signal<int>& o3;
	sc_signal<int>& o4;
	sc_signal<int>& o5;

	// Constructor
	test (	
        sc_module_name NAME, 
	sc_clock& CLK,

        const sc_signal<bool>& RESET_SIG,

	const sc_signal<int>& I1,
	const sc_signal<int>& I2,
	const sc_signal<int>& I3,
	const sc_signal<int>& I4,
	const sc_signal<int>& I5,

	const sc_signal<bool>& CONT1,
	const sc_signal<bool>& CONT2,
	const sc_signal<bool>& CONT3,

	sc_signal<int>& O1,
	sc_signal<int>& O2,
	sc_signal<int>& O3,
	sc_signal<int>& O4,
	sc_signal<int>& O5)
	  : reset_sig(RESET_SIG), i1(I1),  i2(I2),  
	    i3(I3),  i4(I4), i5(I5), cont1 (CONT1), cont2 (CONT2), 
	    cont3 (CONT3), o1(O1),  o2(O2),  o3(O3),  o4(O4),  o5(O5) 
        {
	  clk(CLK);
          SC_CTHREAD( entry, clk.pos() );
	  reset_signal_is(reset_sig,true);
	}

  void entry();
};
