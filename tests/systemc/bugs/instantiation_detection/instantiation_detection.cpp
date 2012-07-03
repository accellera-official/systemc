/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
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

  end_of_elaboration.cpp -- Check instantiation detection 

  Original Author: Philipp A. Hartmann, OFFIS, 2011-02-14

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>

template< typename T >
T* create( const char* nm )
{
  try {
    return new T( sc_gen_unique_name(nm) );
  }
  catch ( sc_report const & x )
  {
      std::cout << "\n" << x.what() << std::endl;
  }
  return 0; // error detected, return NULL
}

SC_MODULE(sub_module)
{
  SC_CTOR(sub_module){}
};

SC_MODULE(top)
{
  SC_CTOR(top){
    SC_THREAD( create_things );
  }

  void create_things()
  {
    sc_assert( create<sub_module>("sub_module") == NULL );
    sc_assert( create<sc_in<bool> >("port") == NULL );
    sc_assert( create<sc_export<sc_signal_in_if<bool> > >("export") == NULL );
    sc_assert( create<sc_signal<bool> >("signal") == NULL );

    try {
      SC_THREAD(create_things);
    } catch ( sc_report const & x ) {
      std::cout << "\n" << x.what() << std::endl;
    }

    try {
      SC_METHOD(create_things);
    } catch ( sc_report const & x ) {
      std::cout << "\n" << x.what() << std::endl;
    }
  }

#if 1
  void start_of_simulation()
  {
      std:: cout << "\n---8<--- start_of_simulation ---8<---\n";
      create_things(); 
      std:: cout << "\n--->8--- start_of_simulation --->8---\n";
  } 
  void end_of_elaboration()
  {
      std:: cout << "\n---8<--- end_of_elaboration  ---8<---\n";
      create_things(); 
      std:: cout << "\n--->8--- end_of_elaboration  --->8---\n";
  } 
#endif
};

int sc_main( int, char*[] )
{
  sc_report_handler::set_actions( "object already exists", SC_DO_NOTHING );

  top dut("dut");
  sc_start( 1, SC_NS );
  std::cout << "\nSuccess" << std::endl;
  return 0;
}
