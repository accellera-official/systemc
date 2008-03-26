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

  sc_lambda_friends.h_ - Friends of Lambdas.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*---------------------------------------------------------------------------*/
/*  NOTE: This header file is multiply included in sc_lambda.h               */
/*---------------------------------------------------------------------------*/

#ifdef SC_LAMBDA_H


#define L_DECL1(op, rator, ty1, exp1)                                         \
    friend inline sc_lambda_ptr operator op( ty1 );

#define L_DECL1SPEC(op, ty1, code)                                            \
    friend inline sc_lambda_ptr operator op( ty1 );

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2)                              \
    friend inline sc_lambda_ptr operator op( ty1, ty2 );

#define L_DECL2SPEC(op, ty1, ty2, code)                                       \
    friend inline sc_lambda_ptr operator op( ty1, ty2 );

#include "sysc/kernel/sc_lambda_exps.h"

#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC


#endif
