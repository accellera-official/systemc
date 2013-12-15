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

  out_of_bounds.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

//----------------------------------------------------------
// test the out of range of sc_bv<> and sc_lv<>
//----------------------------------------------------------
#include "systemc.h"

template<class X>
void test(int W)
{
  X x(W);
  try  {    x.range(W+1,0); }
  catch(const sc_report& s)
  {
    cout<<s.what()<<"\n";
  }
  try  {    x.range(0,W+1); }
  catch(const sc_report& s)
  {
    cout<<s.what()<<"\n";
  }
  try  {    x.range(W-1,-1); }
  catch(const sc_report& s)
  {
    cout<<s.what()<<"\n";
  }
  try  {    x.range(-1,W-1); }
  catch(const sc_report& s)
  {
    cout<<s.what()<<"\n";
  }
  catch(...)
    { cout<<"couldn''t catch anything\n";}
}

int sc_main(int, char**)
{
  const unsigned N = 2000;

  test<sc_bv<N> >(N);
  test<sc_lv<N> >(N);

  return 0;
}
