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

  sc_nbexterns.h -- External functions for both sc_signed and sc_unsigned
                    classes. These functions work on two parameters u and
                    v, and copy the result to the first parameter u. This
                    is also the reason that they are suffixed with _on_help.
 
                    The vec_* functions are called through either these
                    functions or those in sc_nbfriends.cpp. The functions in
                    sc_nbfriends.cpp perform their work on two inputs u and v,
                    and return the result object.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_NBEXTERNS_H
#define SC_NBEXTERNS_H


#include "sysc/datatypes/int/sc_nbutils.h"


namespace sc_dt
{

extern 
void add_on_help(small_type &us, 
                 int unb, int und, unsigned long *ud, 
                 small_type vs, 
                 int vnb, int vnd, const unsigned long *vd);

extern 
void mul_on_help_signed(small_type &us,
                        int unb, int und, unsigned long *ud, 
                        int vnb, int vnd, const unsigned long *vd);

void div_on_help_signed(small_type &us,
                        int unb, int und, unsigned long *ud, 
                        int vnb, int vnd, const unsigned long *vd);

extern 
void mod_on_help_signed(small_type &us,
                        int unb, int und, unsigned long *ud, 
                        int vnb, int vnd, const unsigned long *vd);

extern 
void mul_on_help_unsigned(small_type &us,
                          int unb, int und, unsigned long *ud, 
                          int vnb, int vnd, const unsigned long *vd);

void div_on_help_unsigned(small_type &us,
                          int unb, int und, unsigned long *ud, 
                          int vnb, int vnd, const unsigned long *vd);

extern 
void mod_on_help_unsigned(small_type &us,
                          int unb, int und, unsigned long *ud, 
                          int vnb, int vnd, const unsigned long *vd);

extern 
void and_on_help(small_type us, 
                 int unb, int und, unsigned long *ud, 
                 small_type vs,
                 int vnb, int vnd, const unsigned long *vd);

extern 
void or_on_help(small_type us, 
                int unb, int und, unsigned long *ud, 
                small_type vs,
                int vnb, int vnd, const unsigned long *vd);

extern 
void xor_on_help(small_type us, 
                 int unb, int und, unsigned long *ud, 
                 small_type vs,
                 int vnb, int vnd, const unsigned long *vd);

} // namespace sc_dt


#endif
