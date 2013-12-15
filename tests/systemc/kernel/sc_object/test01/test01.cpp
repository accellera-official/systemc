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

  test01.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

//test of attributes 
#include "systemc.h"

SC_MODULE( mod_a )
{
  

  SC_CTOR( mod_a )
    {}
};
    

int
sc_main( int, char*[] )
{
   mod_a module("module");
   mod_a module_1();

   sc_attr_base a1("a1");
   sc_attr_base a2("a2");
   sc_attr_base a3("a3");
   
  module.add_attribute(a1);
  module.add_attribute(a2);
  module.add_attribute(a3);

  cout<<"attributes of object:\n";
  sc_attr_cltn& att_cltn = module.attr_cltn();
  sc_attr_cltn::const_iterator it = att_cltn.begin();
    for( ; it != att_cltn.end(); ++ it ) {
        cout << (*it)->name() << endl;
    }

  cout<<endl;
  module.dump(cout);
  module.dump();
  cout<<endl;

  module.print();

  cout<<endl<<endl<<"Module base name: "<<module.basename()<<endl;
  cout<<"number of attributes: "<< module.num_attributes()<<endl;
  cout<<"after removing attribute a1\n";
  module.remove_attribute("a1");
  cout<<"number of attributes: "<< module.num_attributes()<<endl;
  cout<<"removing all attributes\n";
  module.remove_all_attributes( );
  cout<<"number of attributes: "<< module.num_attributes()<<endl;
  cout<<endl;

  return 0;
}
