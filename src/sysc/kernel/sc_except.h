/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

  sc_except.h - Exception classes to be handled by SystemC.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Gene Bushuyev. Synopsys, Inc.
  Description of Modification: - Had to add empty public default and copy
                                 constructors to satisfy VC6.0.
    
      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_except.h,v $
// Revision 1.3  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.2  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_EXCEPT_H
#define SC_EXCEPT_H


namespace sc_core {

/*
 *  These classes are intentionally empty. Their raison d'etre is for
 *  the implemetation of various SystemC throws.
 */

class sc_user
{
    /*EMPTY*/
public:
    sc_user() {}
    sc_user( const sc_user& ) {}
};

class sc_halt
{
    /*EMPTY*/
public:
    sc_halt() {}
    sc_halt( const sc_halt& ) {}
};

class sc_kill
{
    /*EMPTY*/
public:
    sc_kill() {}
    sc_kill( const sc_kill& ) {}
};

} // namespace sc_core

#endif
