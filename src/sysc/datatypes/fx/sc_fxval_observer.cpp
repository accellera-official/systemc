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

  sc_fxval_observer.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_fxval_observer.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:58  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#include "sysc/datatypes/fx/sc_fxval_observer.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_observer
//
//  Abstract base class for fixed-point value type observers;
//  arbitrary precision.
// ----------------------------------------------------------------------------

sc_fxval_observer* (*sc_fxval_observer::default_observer) () = 0;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast_observer
//
//  Abstract base class for fixed-point value type observers;
//  limited precision.
// ----------------------------------------------------------------------------

sc_fxval_fast_observer* (*sc_fxval_fast_observer::default_observer) () = 0;

} // namespace sc_dt


// Taf!
