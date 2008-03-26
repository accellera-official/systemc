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

  sc_lambda_exps.h_ -- Macro expansions for available lambda expressions.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*****************************************************************************

  This header file is included by sc_lambda_defs.h_ and sc_lambda_friends.h_
  The former defines the code for the inline functions, whereas the latter
  declare friendship for the classes sc_lambda, sc_lambda_ptr, and
  sc_lambda_rator. By using macros, consistency among these files will be
  more easily maintained, as well as amount of typing is reduced.

 *****************************************************************************/


#ifdef L_DECL1


// ----------------------------------------------------------------------------

L_DECL2SPEC( ==, const sc_lambda_ptr&, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        assert( s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_SUL_EQ,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else if( s1->is_bool() ) {
        assert( s2->is_bool() );
        return new sc_lambda( SC_LAMBDA_BOOL_EQ,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else {
        assert( s2->is_int() );
        return new sc_lambda( SC_LAMBDA_INT_EQ,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
)


L_DECL2( ==, SC_LAMBDA_BOOL_EQ, const sc_signal_bool_deval&,
	                        const sc_signal_bool_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_BOOL_EQ, const sc_signal_bool_deval&,
                                bool,                        s1, s2 )
L_DECL2( ==, SC_LAMBDA_BOOL_EQ, bool,
                                const sc_signal_bool_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_BOOL_EQ, const sc_lambda_ptr&,
                                const sc_signal_bool_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_BOOL_EQ, const sc_signal_bool_deval&,
                                const sc_lambda_ptr&,        s1, s2 )


L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_signal_logic_deval&,
                               const sc_lambda_ptr&,         s1, s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_signal_logic_deval&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_signal_logic_deval&,
                               const sc_dt::sc_logic&,       s1, s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_dt::sc_logic&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_signal_logic_deval&,
                               char,                         s1,
                                                        sc_dt::sc_logic( s2 ) )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, char,
                              const sc_signal_logic_deval&, sc_dt::sc_logic(s1),
                                                             s2 )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&,
                               char,                         s1,
                                              sc_dt::sc_logic( s2 ) )
L_DECL2( ==, SC_LAMBDA_SUL_EQ, char,
                               const sc_lambda_ptr&,      sc_dt::sc_logic( s1 ),
                                                             s2 )


// ----------------------------------------------------------------------------

// Overloading operator!=() clashes with STL. Should look for a way around
// this, since STL has become a standard library for ANSI C++

L_DECL2SPEC( !=, const sc_lambda_ptr&, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        assert( s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_SUL_NE,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else if( s1->is_bool() ) {
        assert( s2->is_bool() );
        return new sc_lambda( SC_LAMBDA_BOOL_NE,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else {
        assert( s2->is_int() );
        return new sc_lambda( SC_LAMBDA_INT_NE,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
)


L_DECL2( !=, SC_LAMBDA_BOOL_NE, const sc_signal_bool_deval&,
                                const sc_signal_bool_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_BOOL_NE, const sc_signal_bool_deval&,
                                bool,                        s1, s2 )
L_DECL2( !=, SC_LAMBDA_BOOL_NE, bool,
                                const sc_signal_bool_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_BOOL_NE, const sc_lambda_ptr&,
                                const sc_signal_bool_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_BOOL_NE, const sc_signal_bool_deval&,
                                const sc_lambda_ptr&,        s1, s2 )


L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_signal_logic_deval&,
                               const sc_lambda_ptr&,         s1, s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_lambda_ptr&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_signal_logic_deval&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_signal_logic_deval&,
                               const sc_dt::sc_logic&,       s1, s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_dt::sc_logic&,
                               const sc_signal_logic_deval&, s1, s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_signal_logic_deval&,
                               char,                         s1,
                                                         sc_dt::sc_logic( s2 ) )
L_DECL2( !=, SC_LAMBDA_SUL_NE, char,
                            const sc_signal_logic_deval&, sc_dt::sc_logic( s1 ),
                                                             s2 )
L_DECL2( !=, SC_LAMBDA_SUL_NE, const sc_lambda_ptr&,
                               char,                         s1,
                                                sc_dt::sc_logic( s2 ) )
L_DECL2( !=, SC_LAMBDA_SUL_NE, char,
                               const sc_lambda_ptr&,  sc_dt::sc_logic( s1 ),
                                                             s2 )


// ----------------------------------------------------------------------------

L_DECL2SPEC( &, const sc_lambda_ptr&, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        assert( s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_SUL_BITAND,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else {
        assert( ! s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_INT_BITAND,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
)


L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_signal_logic_deval&,
                                  const sc_lambda_ptr&,         s1, s2 )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_signal_logic_deval&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_signal_logic_deval&,
                                  const sc_dt::sc_logic&,       s1, s2 )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_signal_logic_deval&,
                                  char,                         s1,
                                                         sc_dt::sc_logic(s2) )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, const sc_dt::sc_logic&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( &, SC_LAMBDA_SUL_BITAND, char,
                             const sc_signal_logic_deval&, sc_dt::sc_logic(s1),
                                                                s2 )


// ----------------------------------------------------------------------------

L_DECL2SPEC( |, const sc_lambda_ptr&, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        assert( s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_SUL_BITOR,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else {
        assert( ! s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_INT_BITOR,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
)


L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_signal_logic_deval&,
                                 const sc_lambda_ptr&,         s1, s2 )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&,
                                 const sc_signal_logic_deval&, s1, s2 )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_signal_logic_deval&,
                                 const sc_signal_logic_deval&, s1, s2 )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_signal_logic_deval&,
                                 const sc_dt::sc_logic&,       s1, s2 )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_signal_logic_deval&,
                                 char,                         s1,
                                                         sc_dt::sc_logic(s2) )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, const sc_dt::sc_logic&,
                                 const sc_signal_logic_deval&, s1, s2 )
L_DECL2( |, SC_LAMBDA_SUL_BITOR, char,
                             const sc_signal_logic_deval&, sc_dt::sc_logic(s1),
                                                               s2 )


// ----------------------------------------------------------------------------

L_DECL1SPEC( ~, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        return new sc_lambda( SC_LAMBDA_SUL_BITNOT,
                              new sc_lambda_rand( s1 ) );
    }
    else {
        return new sc_lambda( SC_LAMBDA_INT_BITNOT,
                              new sc_lambda_rand( s1 ) );
    }
)


L_DECL1( ~, SC_LAMBDA_SUL_BITNOT, const sc_signal_logic_deval&, s1 )


// ----------------------------------------------------------------------------

L_DECL2SPEC( ^, const sc_lambda_ptr&, const sc_lambda_ptr&,
    if( s1->is_sc_logic() ) {
        assert( s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_SUL_BITXOR,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
    else {
        assert( ! s2->is_sc_logic() );
        return new sc_lambda( SC_LAMBDA_INT_BITXOR,
                              new sc_lambda_rand( s1 ),
                              new sc_lambda_rand( s2 ) );
    }
)


L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_signal_logic_deval&,
                                  const sc_lambda_ptr&,         s1, s2 )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_signal_logic_deval&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_signal_logic_deval&,
                                  const sc_dt::sc_logic&,       s1, s2 )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_signal_logic_deval&,
                                  char,                         s1,
                                                          sc_dt::sc_logic(s2) )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, const sc_dt::sc_logic&,
                                  const sc_signal_logic_deval&, s1, s2 )
L_DECL2( ^, SC_LAMBDA_SUL_BITXOR, char,
                              const sc_signal_logic_deval&, sc_dt::sc_logic(s1),
                                                                s2 )


// ----------------------------------------------------------------------------

L_DECL2( &&, SC_LAMBDA_AND, const sc_lambda_ptr&,
                            const sc_lambda_ptr&,        s1, s2 )
L_DECL2( &&, SC_LAMBDA_AND, const sc_signal_bool_deval&,
                            const sc_signal_bool_deval&, s1, s2 )
L_DECL2( &&, SC_LAMBDA_AND, const sc_signal_bool_deval&,
                            bool,                        s1, s2 )
L_DECL2( &&, SC_LAMBDA_AND, bool,
                            const sc_signal_bool_deval&, s1, s2 )
L_DECL2( &&, SC_LAMBDA_AND, const sc_signal_bool_deval&,
                            const sc_lambda_ptr&,        s1, s2 )
L_DECL2( &&, SC_LAMBDA_AND, const sc_lambda_ptr&,
                            const sc_signal_bool_deval&, s1, s2 )


// ----------------------------------------------------------------------------

L_DECL2( ||, SC_LAMBDA_OR, const sc_lambda_ptr&,
                           const sc_lambda_ptr&,        s1, s2 )
L_DECL2( ||, SC_LAMBDA_OR, const sc_signal_bool_deval&,
                           const sc_signal_bool_deval&, s1, s2 )
L_DECL2( ||, SC_LAMBDA_OR, const sc_signal_bool_deval&,
                           bool,                        s1, s2 )
L_DECL2( ||, SC_LAMBDA_OR, bool,
                           const sc_signal_bool_deval&, s1, s2 )
L_DECL2( ||, SC_LAMBDA_OR, const sc_signal_bool_deval&,
                           const sc_lambda_ptr&,        s1, s2 )
L_DECL2( ||, SC_LAMBDA_OR, const sc_lambda_ptr&,
                           const sc_signal_bool_deval&, s1, s2 )


// ----------------------------------------------------------------------------

L_DECL1( !, SC_LAMBDA_NOT, const sc_lambda_ptr&,        s1 )
L_DECL1( !, SC_LAMBDA_NOT, const sc_signal_bool_deval&, s1 )


#endif
