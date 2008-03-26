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

  main.cpp -- This example shows the use of the sc_export class.

  Original Author: 

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
In this example, module D contains a channel of type C which
implements an interface C_if. D makes the interface C visible to the
outside by an interface-port named "IFP". Module E contains an
instance of D and also contains another instance of C. E exports both
interfaces as interface-ports IFP1 and IFP2. Both IFP1 and IFP2 are
bound to ports P1 and P2 of module X.

              +-------------+             +------------------+
              |     X       |             |    E    +----+   |
              |             |P1       IFP1|         | C  |   |
              |            [ ]------------O---------@    |   |
              |             |             |         |    |   |
              |             |             |         +----+   |
              |             |             |                  |
              |             |             |     +----------+ |
              |             |             |     | D        | |
              |             |             |     |   +----+ | |
              |             |P2       IFP2|  IFP|   | C  | | |
              |            [ ]------------O-----O---@    | | |
              |             |             |     |   |    | | |
              |             |             |     |   +----+ | |
              |             |             |     |          | |
              |             |             |     +----------+ |
              +-------------+             +------------------+

 Legend:
   [ ]   port
    O    interface-port
    @    interface

*/

#include "systemc.h"

// Interface
class C_if : virtual public sc_interface
{ 
public:
    virtual void run() = 0;
};

// Channel
class C : public C_if, public sc_channel
{ 
public:
    SC_CTOR(C) { }
    virtual void run() 
    { 
        cout << sc_time_stamp() << " In Channel run() " << endl; 
    }
};

// --- D: export channel C through IFP --------
SC_MODULE( D )
{
    sc_export<C_if> IFP;

    SC_CTOR( D ) 
	: IFP("IFP"),   // explicit name
	  m_C("C")
    {                
	IFP( m_C );     // bind sc_export->interface by name
    }
 private:
    C m_C;            // channel
};

// --- E: module with two interface-ports ---
SC_MODULE( E )
{
 private:
    C m_C;
    D m_D;

 public:
    sc_export<C_if> IFP1;
    sc_export<C_if> IFP2;
				 
    SC_CTOR( E ) 		 
	: m_C("C"),
	  m_D("D"),
	  IFP1("IFP1") 		
    {				 
        IFP1( m_C );
	IFP2( m_D.IFP );          // bind sc_export->sc_export by name
        IFP1.get_interface();     // just to see whether it compiles
    }
};

// Module X connected to the channels through E
SC_MODULE( X )
{
    sc_port<C_if> P1;
    sc_port<C_if> P2;
    SC_CTOR(X) {
        SC_THREAD(run);
    }
    void run() {
        wait(10, SC_NS);
	P1->run();
	wait(10, SC_NS);
	P2->run();
    }
};

int sc_main(int argc, char** argv) {
  E the_E("E");
  X the_X("X");
  // port->IFP
  the_X.P1( the_E.IFP1 );
  the_X.P2( the_E.IFP2 );

  sc_start(17, SC_NS);
  the_E.IFP1->run();  // testing the operator-> of sc_export
  sc_start(50, SC_NS);

  return 0;
}
