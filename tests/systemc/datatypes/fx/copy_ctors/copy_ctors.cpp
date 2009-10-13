/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  copy_ctors.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_[u]fix constructors with fixed-point type argument

#define SC_INCLUDE_FX
#include "systemc.h"

#define WRITE(a) \
    cout << a.type_params() << endl

int
sc_main( int, char*[] )
{
    sc_fixed<1,2,SC_RND,SC_SAT> fxd;
    sc_fixed_fast<3,4,SC_RND_ZERO,SC_SAT_ZERO> fxdf;
    sc_ufixed<5,6,SC_RND_MIN_INF,SC_SAT_SYM> ufxd;
    sc_ufixed_fast<7,8,SC_RND_INF,SC_WRAP> ufxdf;

    sc_fix fx( 9, 10, SC_RND_CONV, SC_SAT );
    sc_fix_fast fxf( 11, 12, SC_TRN, SC_SAT_ZERO );
    sc_ufix ufx( 13, 14, SC_TRN_ZERO, SC_SAT_SYM );
    sc_ufix_fast ufxf( 15, 16, SC_RND, SC_WRAP );

    WRITE(  fxd );
    WRITE(  fxdf );
    WRITE( ufxd );
    WRITE( ufxdf );

    WRITE(  fx );
    WRITE(  fxf );
    WRITE( ufx );
    WRITE( ufxf );

    // sc_fix
    sc_fix fx01(  fxd );
    sc_fix fx02(  fxdf );
    sc_fix fx03( ufxd );
    sc_fix fx04( ufxdf );
    sc_fix fx05(  fx );
    sc_fix fx06(  fxf );
    sc_fix fx07( ufx );
    sc_fix fx08( ufxf );

    cout << endl;
    WRITE( fx01 );
    WRITE( fx02 );
    WRITE( fx03 );
    WRITE( fx04 );
    WRITE( fx05 );
    WRITE( fx06 );
    WRITE( fx07 );
    WRITE( fx08 );

    // sc_fix_fast
    sc_fix_fast fxf01(  fxd );
    sc_fix_fast fxf02(  fxdf );
    sc_fix_fast fxf03( ufxd );
    sc_fix_fast fxf04( ufxdf );
    sc_fix_fast fxf05(  fx );
    sc_fix_fast fxf06(  fxf );
    sc_fix_fast fxf07( ufx );
    sc_fix_fast fxf08( ufxf );

    cout << endl;
    WRITE( fxf01 );
    WRITE( fxf02 );
    WRITE( fxf03 );
    WRITE( fxf04 );
    WRITE( fxf05 );
    WRITE( fxf06 );
    WRITE( fxf07 );
    WRITE( fxf08 );

    // sc_ufix
    sc_ufix ufx01(  fxd );
    sc_ufix ufx02(  fxdf );
    sc_ufix ufx03( ufxd );
    sc_ufix ufx04( ufxdf );
    sc_ufix ufx05(  fx );
    sc_ufix ufx06(  fxf );
    sc_ufix ufx07( ufx );
    sc_ufix ufx08( ufxf );

    cout << endl;
    WRITE( ufx01 );
    WRITE( ufx02 );
    WRITE( ufx03 );
    WRITE( ufx04 );
    WRITE( ufx05 );
    WRITE( ufx06 );
    WRITE( ufx07 );
    WRITE( ufx08 );

    // sc_ufix_fast
    sc_ufix_fast ufxf01(  fxd );
    sc_ufix_fast ufxf02(  fxdf );
    sc_ufix_fast ufxf03( ufxd );
    sc_ufix_fast ufxf04( ufxdf );
    sc_ufix_fast ufxf05(  fx );
    sc_ufix_fast ufxf06(  fxf );
    sc_ufix_fast ufxf07( ufx );
    sc_ufix_fast ufxf08( ufxf );

    cout << endl;
    WRITE( ufxf01 );
    WRITE( ufxf02 );
    WRITE( ufxf03 );
    WRITE( ufxf04 );
    WRITE( ufxf05 );
    WRITE( ufxf06 );
    WRITE( ufxf07 );
    WRITE( ufxf08 );

    // misc
    sc_fix fx09( fx, 123, 456 );
    sc_fix_fast fxf09( fxf, SC_RND, SC_SAT );
    sc_ufix ufx09( ufx, 456, 123 );
    sc_ufix_fast ufxf09( ufxf, SC_TRN, SC_WRAP );

    cout << endl;
    WRITE(  fx09 );
    WRITE(  fxf09 );
    WRITE( ufx09 );
    WRITE( ufxf09 );

    return 0;
}
