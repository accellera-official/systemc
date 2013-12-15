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

  test_bitref.cpp -- Test using bitrefs in boolean contexts

  Original Author: Philipp A. Hartmann, OFFIS, 2013-10-28

*****************************************************************************/

#include <systemc.h>

#define sc_nassert( expr ) \
  sc_assert( !(expr) )

using sc_dt::sc_bitref;
using sc_dt::sc_bitref_r;
using sc_dt::sc_concref;
using sc_dt::sc_concref_r;
using sc_dt::sc_subref;
using sc_dt::sc_subref_r;

int sc_main(int,char*[])
{
  sc_bv<8> bv = "00101010";
  sc_lv<8> lv = "1XZ01010";

  // plain bitrefs
  {
    sc_nassert( bv[0] );
    sc_assert( ~bv[0] );
    sc_assert ( !bv[0] );
    sc_nassert( bv[0] == true );
    sc_assert ( bv[0] != true );
    sc_assert ( bv[0] == SC_LOGIC_0 );
    sc_nassert( bv[0].to_bool() );
    sc_assert( bv[1].to_bool() );
    sc_nassert( bv[0] != 0 );
    sc_nassert( bv[0] == bv[1] );
    sc_nassert( SC_LOGIC_1 != bv[1] );

    sc_assert( bv[0] == lv[0] );
    sc_assert( bv[0] != lv[6] );

    sc_assert( bv.or_reduce() );
    sc_assert( !bv.nor_reduce() );

    sc_assert ( lv[0] == false );
    sc_assert ( lv[5] == SC_LOGIC_Z );
    sc_assert ( lv[6] == SC_LOGIC_X );
    sc_assert ( lv[6].value() == SC_LOGIC_X );

    // sc_assert( ~lv[0] ); // could not convert ... to ‘bool’
    // sc_assert( lv[7] );  // could not convert ... to ‘bool’

    sc_assert( !lv[0].to_bool() );
    sc_assert( (~lv[0]).to_bool() );

    // with warnings
    sc_assert( lv[5].to_bool() );
    sc_assert( lv[6].to_bool() );
  }

  // bitrefs to subrefs
  {
    /* auto */ sc_subref_r< sc_bv_base > bv_range_r = bv.range(5,1);
    /* auto */ sc_subref_r< sc_lv_base > lv_range_r = lv.range(6,2);

    /* auto */ sc_subref< sc_bv_base >   bv_range   = bv.range(5,1);
    /* auto */ sc_subref< sc_lv_base >   lv_range   = lv.range(6,2);

    sc_assert( bv_range[0] );
    sc_assert( !bv_range[1] );
    sc_assert( ~bv_range[1] );

    sc_assert( bv[1] == bv_range_r[0] );

    bv_range[0] = false;
    sc_assert( !bv[1] );
    sc_assert( bv[1] == bv_range_r[0] );
    sc_assert( bv[1] == bv_range[0] );

    bv_range[0] = SC_LOGIC_1;
    sc_assert( bv[1] );
    sc_assert( bv[1] == bv_range_r[0] );
    sc_assert( bv[1] == bv_range[0] );

    // sc_assert( ~lv_range[0] ); // could not convert ... to ‘bool’
    // sc_assert( lv_range_r[7] );  // could not convert ... to ‘bool’

    sc_assert( !lv_range_r[0].to_bool() );
    sc_assert( lv_range_r[1].to_bool() );
    sc_nassert( (~lv_range[1]).to_bool() );

    // with warnings
    sc_assert( lv_range[3].to_bool() );
    sc_assert( lv_range_r[4].to_bool() );
  }

  // bitrefs to concrefs
  {
    /* auto */ sc_concref< sc_concref<sc_subref<sc_bv_base>, sc_subref<sc_bv_base> >
                         , sc_bitref<sc_bv_base> > 
                 bv_range = ( bv.range(7,6) , bv.range(5,1), bv[1] );
    /* auto */ sc_concref_r< sc_concref_r<sc_subref<sc_bv_base>, sc_subref<sc_lv_base> >
                           , sc_bv_base >
      lv_range_r = ( bv.range(7,6) , lv.range(6,2), true );

    sc_assert( bv_range[0] );
    sc_assert( !bv_range[2] );
    sc_assert( ~bv_range[2] );

    sc_assert( bv[1] == bv_range[0] );

    bv_range[0] = false;
    sc_assert( !bv[1] );
    sc_assert( bv[1] == bv_range[0] );

    bv_range[0] = SC_LOGIC_1;
    sc_assert( bv[1] );
    sc_assert( bv[1] == bv_range[0] );

    // sc_assert( ~lv_range_r[0] ); // could not convert ... to ‘bool’
    // sc_assert( lv_range_r[7] );  // could not convert ... to ‘bool’

    sc_assert( lv_range_r[0].to_bool() );
    sc_assert( !lv_range_r[1].to_bool() );
    sc_assert( (~lv_range_r[1]).to_bool() );

    // with warnings
    sc_assert( lv_range_r[4].to_bool() );
    sc_assert( lv_range_r[5].to_bool() );
  }

  return 0;
}
