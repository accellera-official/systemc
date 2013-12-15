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

  mem.h -- 

  Original Author: Stan Liao, Synopsys, Inc., 2000-07-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

struct memory : sc_module {

  sc_in_clk            clk;
  sc_in<sc_uint<8> >   in_value1; 
  sc_in<bool>          in_valid1;     
  sc_out<bool>         out_valid1;    

  SC_HAS_PROCESS( memory );

  memory (const  char    *NAME)
        : sc_module (NAME)    
    {
      SC_CTHREAD( entry, clk.pos() );
      end_module();
    };

    void entry ();

};

