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

  new_struct.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef test_struct
#define test_struct

struct a_new_struct {
  char              in_value1;     
  char              in_value2;     
  sc_logic              in_valid;    

  inline bool operator == (const a_new_struct & rhs) const
  {
    return (rhs.in_value1 == in_value1 && rhs.in_value2 == in_value2
	    && rhs.in_valid == in_valid);
}
};

inline
void
sc_trace( sc_trace_file*, const a_new_struct&, const std::string& )
{
    // NOT IMPLEMENTED
}


struct b_new_struct {
  sc_lv<9>                    out_value1;    
  int                    out_value2;    

inline bool operator == (const b_new_struct & rhs) const
  {
    return (rhs.out_value1 == out_value1 && rhs.out_value2 == out_value2);
}
};

inline
void
sc_trace( sc_trace_file*, const b_new_struct&, const std::string& )
{
    // NOT IMPLEMENTED
}


#endif
