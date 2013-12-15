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

  test02.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, Inc., 2005-12-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// Test automatic object name generation.

#include "systemc.h"

class Name : public sc_object {
  public:
  	Name(const char* name) : sc_object(name)
	{}
};
  	
class NoName : public sc_object {
  public:
  	NoName() : sc_object()
	{}
};
  	
SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
	}
};
int sc_main(int argc, char* argv[])
{
	Name            name(0);
	Name            name1("");
	NoName          no_name;

	cout << name.name() << endl;
	cout << name1.name() << endl;
	cout << no_name.name() << endl;
	cout << "Program completed" << endl;
	return 0;
}
