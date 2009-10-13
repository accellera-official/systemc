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

  arr_struct.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


struct arr_struct1 {
  char  a[4][4];

 
};

inline
bool
operator == ( const arr_struct1&, const arr_struct1& )
{
    // NOT IMPLEMENTED
    return false;
}

inline
void
sc_trace( sc_trace_file*, const arr_struct1&, const std::string& )
{
    // NOT IMPLEMENTED
}


struct arr_struct2 {

  sc_uint<8> b[4][4];


};

inline
bool
operator == ( const arr_struct2&, const arr_struct2& )
{
    // NOT IMPLEMENTED
    return false;
}

inline
void
sc_trace( sc_trace_file*, const arr_struct2&, const std::string& )
{
    // NOT IMPLEMENTED
}
