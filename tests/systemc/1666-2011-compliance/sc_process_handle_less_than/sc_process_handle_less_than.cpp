
//******************************************************************************
//
//  The following code is derived, directly or indirectly, from the SystemC
//  source code Copyright (c) 1996-2014 by all Contributors.
//  All Rights reserved.
//
//  The contents of this file are subject to the restrictions and limitations
//  set forth in the SystemC Open Source License (the "License");
//  You may not use this file except in compliance with such restrictions and
//  limitations. You may obtain instructions on how to receive a copy of the
//  License at http://www.accellera.org/. Software distributed by Contributors
//  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
//  ANY KIND, either express or implied. See the License for the specific
//  language governing rights and limitations under the License.
//******************************************************************************

// sc_process_handle_less_than.cpp -- test for 
//
//  Original Author: John Aynsley, Doulos, Inc.
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: sc_process_handle_less_than.cpp,v $
// Revision 1.2  2011/05/08 19:18:46  acg
//  Andy Goodrich: remove extraneous + prefixes from git diff.
//

// sc_process_handle::operator< and swap

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
#include <map>

using namespace sc_core;
using std::cout;
using std::endl;

struct Top: sc_module
{
  Top(sc_module_name _name)
  {
    SC_THREAD(T1);
    SC_THREAD(T2);
  }
  
  void T1()
  {
    sc_process_handle a, b;                // Two empty handles
    sc_assert( !a.valid() && !b.valid() ); // Both are invalid
    sc_assert( a != b );
    sc_assert( !(a < b) && !(b < a) );
    
    a = sc_spawn(sc_bind(&Top::f, this));
    b = sc_spawn(sc_bind(&Top::f, this));

    sc_assert( a != b );
    sc_assert( (a < b) || (b < a) );       // Two handles to different processes

    sc_process_handle c = b;

    sc_assert( b == c );
    sc_assert( !(b < c) && !(c < b) );     // Two handles to the same process
    
    std::map<sc_process_handle, int> m;
    m[a] = 1;
    m[b] = 2;
    m[c] = 3;
    
    sc_assert( m.size() == 2 );
    sc_assert( m[a] == 1 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );

    a.swap(b);
    sc_assert( m[a] == 3 );
    sc_assert( m[b] == 1 );
    sc_assert( m[c] == 3 );
    b.swap(a);
       
    wait( a.terminated_event() & b.terminated_event() );
    
    sc_assert( (a < b) || (b < a) ); // Same ordering whether handles are valid or not

    if ( b.valid() ) // Handles may or may not have been invalidated
      sc_assert( b == c );
    else
      sc_assert( b != c );

    sc_assert( b.valid() == c.valid() ); // Invalidation is consistent
    sc_assert( !(b < c) && !(c < b) ); // Two handles to the same process, whether valid or not
    sc_assert( c.terminated() );
    
    sc_process_handle d = a;
    m[d] = 4;
    
    sc_assert( m.size() == 2 );
    sc_assert( m[a] == 4 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );
    sc_assert( m[d] == 4 );
    
    sc_process_handle e;
    m[e] = 5;
    
    sc_assert( m.size() == 3 );
    sc_assert( m[a] == 4 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );
    sc_assert( m[d] == 4 );
    sc_assert( m[e] == 5 );
    
    sc_process_handle f;
    m[f] = 6;
    
    sc_assert( m.size() == 3 );
    sc_assert( m[a] == 4 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );
    sc_assert( m[d] == 4 );
    sc_assert( m[e] == 6 );
    sc_assert( m[f] == 6 );    
    
    sc_process_handle g;
    m[g] = 7;
    
    sc_assert( m.size() == 3 );
    sc_assert( m[a] == 4 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );
    sc_assert( m[d] == 4 );
    sc_assert( m[e] == 7 );
    sc_assert( m[f] == 7 );    
    sc_assert( m[g] == 7 );  
    
    g = sc_spawn(sc_bind(&Top::f, this));
    m[g] = 8;
 
    sc_assert( m.size() == 4 );
    sc_assert( m[a] == 4 );
    sc_assert( m[b] == 3 );
    sc_assert( m[c] == 3 );
    sc_assert( m[d] == 4 );
    sc_assert( m[e] == 7 );
    sc_assert( m[f] == 7 );    
    sc_assert( m[g] == 8 );  

    cout << "T1 completed" << endl;
  }
  
  void T2()
  {  
    sc_process_handle a, b = sc_get_current_process_handle();
    sc_assert( b.valid() );
    
    a.swap( b );
    sc_assert( a == sc_get_current_process_handle() );
    sc_assert( !b.valid() );
    
    cout << "T2 completed" << endl;
  }
  
  void f() {}
    
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start();
  
  cout << endl << "Success" << endl;
  return 0;
}
  
