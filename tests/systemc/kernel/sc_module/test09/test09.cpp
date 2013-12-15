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

  test09.cpp -- Test derivation from and to sc_module instances.

  Original Author: Andy Goodrich, Forte Design Systemc, Inc. 2003-10-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

// sc_module ---> non-sc_module

SC_MODULE(ModuleBase) 
{
  public:
    SC_CTOR(ModuleBase)
    {
    }
	void base_method()
	{
		cout << sc_time_stamp() << ": ModuleBase" << endl;
	}
	sc_in_clk m_clk;
};

class NonModuleDerived : public ModuleBase
{
  public:
	SC_HAS_PROCESS(NonModuleDerived);
	NonModuleDerived(sc_module_name name_) : ModuleBase(name_)
	{
		SC_METHOD(base_method)
		sensitive << m_clk;
		SC_METHOD(derived_method)
		sensitive << m_clk;
	}
	void derived_method()
	{
		cout << sc_time_stamp() << ": NonModuleDerived" << endl;
	}
};

// non-sc_module ---> sc_module

class NonModuleBase
{
  public:
	sc_in_clk m_clk;
};

SC_MODULE(ModuleDerived), public NonModuleBase
{
	SC_CTOR(ModuleDerived) : NonModuleBase()
	{
		SC_METHOD(derived_method)
		sensitive << m_clk;
	}
	void derived_method()
	{
		cout << sc_time_stamp() << ": ModuleDerived" << endl;
	}
};

int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	NonModuleDerived  non_derived("nonderived");
	ModuleDerived     derived("derived");
	non_derived.m_clk(clock);
	derived.m_clk(clock);

	sc_start(20, SC_NS);
	return 0;
}



