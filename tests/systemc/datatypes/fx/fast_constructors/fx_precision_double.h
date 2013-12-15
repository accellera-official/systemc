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

  fx_precision_double.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// ============================================================================
//
//  This confidential and proprietary software may be used only
//  as authorized by a licensing agreement from Synopsys, Inc.
//  In the event of publication, the following notice is applicable:
//
//            Copyright (c) 1999 by Synopsys, Inc.
//                     ALL RIGHTS RESERVED
//
//  The entire notice above must be reproduced on all authorized copies.
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//      File : fx_precision_double.h
//
//  Abstract : Define double precision.
//
//    Author : Martin Janssen
//
//   Created : 21-jun-1999
//
// ============================================================================

#include "fx_precision_arbitrary.h"

#define sc_fix    sc_fix_fast
#define sc_fixed  sc_fixed_fast
#define sc_fxval  sc_fxval_fast
#define sc_ufix   sc_ufix_fast
#define sc_ufixed sc_ufixed_fast

// That's all folks!

