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

  sc_lambda.h -- Dynamically created lambdas.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_LAMBDA_H
#define SC_LAMBDA_H


#include <cassert>

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_macros.h"
#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/utils/sc_mempool.h"

namespace sc_core {

class sc_port_registry;


// forward declarations
class sc_lambda;
class sc_lambda_ptr;
class sc_lambda_rand;


/*
 *  ABBREVIATIONS:
 *    rator = operator
 *    rand  = operand
 */


// ----------------------------------------------------------------------------
//  ENUM : sc_lambda_rator_e
//
//  Enumeration of lambda operators.
// ----------------------------------------------------------------------------

enum sc_lambda_rator_e
{
    SC_LAMBDA_FALSE = 0x00000000,
    SC_LAMBDA_TRUE,

    // logical operators
    SC_LAMBDA_AND,
    SC_LAMBDA_OR,
    SC_LAMBDA_NOT,

    SC_LAMBDA_BOOL,
    SC_LAMBDA_BOOL_EQ,
    SC_LAMBDA_BOOL_NE,

    // relational operators for sc_dt::sc_logic
    SC_LAMBDA_SUL_EQ,
    SC_LAMBDA_SUL_NE,

    // relational operators for int
    SC_LAMBDA_INT_EQ,
    SC_LAMBDA_INT_NE,
    SC_LAMBDA_INT_LE,
    SC_LAMBDA_INT_GE,
    SC_LAMBDA_INT_LT,
    SC_LAMBDA_INT_GT,

    // bitwise operators for sc_dt::sc_logic
    SC_LAMBDA_SUL_BITAND,
    SC_LAMBDA_SUL_BITOR,
    SC_LAMBDA_SUL_BITNOT,
    SC_LAMBDA_SUL_BITXOR,

    // arithmetic operators for int
    SC_LAMBDA_INT_ADD,
    SC_LAMBDA_INT_SUB,
    SC_LAMBDA_INT_MUL,
    SC_LAMBDA_INT_DIV,
    SC_LAMBDA_INT_REM,

    // bitwise operators for int
    SC_LAMBDA_INT_BITAND,
    SC_LAMBDA_INT_BITOR,
    SC_LAMBDA_INT_BITNOT,
    SC_LAMBDA_INT_BITXOR
};


// ----------------------------------------------------------------------------
//  CLASS : sc_lambda
//
//  <summary> Dynamically created lambda objects </summary>
//
//  sc_lambda implements an expression-tree representation of a delay-evaluted
//  expression, i.e. lambda. The eval() method is used to force the evaluation
//  of the expression tree.
//
//  The user should not create a lambda object explicitly; rather, he writes
//  an expression involving a signal of type sc_dt::sc_logic or int, and a smart
//  pointer sc_lambda_ptr object will be automatically created.
//
// ----------------------------------------------------------------------------

class sc_lambda
{
    friend class sc_lambda_ptr;
    friend class sc_lambda_rand;
    friend class sc_port_registry;

#include "sysc/kernel/sc_lambda_friends.h"

public:

    static void* operator new( size_t sz )
        { return sc_mempool::allocate( sz ); }

    static void operator delete( void* p, size_t sz )
        { sc_mempool::release( p, sz ); }

    // create a lambda out of a bool signal
    explicit sc_lambda( const sc_signal_in_if<bool>& );

    // evaluates the expression tree
    bool eval() const;

    // destructor
    ~sc_lambda();

private:

    // constructor - takes an operator code and two operands
    sc_lambda( sc_lambda_rator_e op,
               sc_lambda_rand* o1 = 0, sc_lambda_rand* o2 = 0 );

    // evaluates the expression as an int
    int int_eval() const;

    // evalutes the expression as a sc_dt::sc_logic
    sc_dt::sc_logic sc_logic_eval() const;

    // returns true if the type of the expression tree is bool
    bool is_bool() const
    {
        return ( ( SC_LAMBDA_AND <= rator_ty ) &&
		 ( rator_ty <= SC_LAMBDA_INT_GT ) );
    }

    // returns true if the type of the expression tree is int
    bool is_int() const
    {
        return ( ( SC_LAMBDA_INT_ADD <= rator_ty ) &&
		 ( rator_ty <= SC_LAMBDA_INT_BITXOR ) );
    }

    // returns true if the type of the expression tree is sc_dt::sc_logic
    bool is_sc_logic() const
    {
        return ( ( SC_LAMBDA_SUL_BITAND <= rator_ty ) &&
		 ( rator_ty <= SC_LAMBDA_SUL_BITXOR ) );
    }

    void replace_ports( void (*fn)( sc_port_registry*, sc_lambda_rand* ),
                        sc_port_registry* );

private:

    sc_lambda_rator_e rator_ty;
    sc_lambda_rand*   op1;
    sc_lambda_rand*   op2;
    int               ref_count;  // reference count

private:

    sc_lambda();

    static sc_lambda dummy_lambda;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_lambda_ptr
//
//  <summary> Smart pointer for sc_lambda </summary>
//
//  sc_lambda_ptr is a smart pointer type that is created by the various
//  overloaded operators declared in sc_lambda_friends.h_
//
//  The user should not define an object of type sc_lambda_ptr directly.
//  Instead, the overloaded operators that return sc_lambda_ptr should be
//  used.
//
// ----------------------------------------------------------------------------

class sc_lambda_ptr
{
    friend class sc_cthread_process;
    friend class sc_lambda;
    friend class sc_lambda_rand;
    friend class sc_port_registry;

#include "sysc/kernel/sc_lambda_friends.h"

public:

    static void* operator new( size_t sz )
        { return sc_mempool::allocate( sz ); }

    static void* operator new( size_t, void* p )
        { return p; }

    static void operator delete( void* p, size_t sz )
        { sc_mempool::release( p, sz ); }

    // constructors
    // <group>
    sc_lambda_ptr()
    : ptr( &sc_lambda::dummy_lambda )
    {
        ptr->ref_count ++;
    }

    sc_lambda_ptr( const sc_lambda_ptr& p )
    : ptr( p.ptr )
    {
        ptr->ref_count ++;
    }

    explicit sc_lambda_ptr( const sc_signal_bool_deval& b )
    : ptr( new sc_lambda( RCAST<const sc_signal_in_if<bool>&>( b ) ) )
    {
        ptr->ref_count ++;
    }
    // </group>

    ~sc_lambda_ptr()
    {
        if( -- ( ptr->ref_count ) == 0 )
            delete ptr;
    }

    // assignment operator - simply copy the pointer member
    sc_lambda_ptr& operator = ( const sc_lambda_ptr& p )
    {
        if( &p == this )
            return *this;
        // update reference count on original lambda
        if (--(ptr->ref_count) == 0)
            delete ptr;
        ptr = p.ptr;
        // Update reference count
        ptr->ref_count++;
        return *this;
    }

    operator bool() const
    {
        // Provide this implicit type conversion so that the code
        // would still work even if the user makes a mistake, e.g.
        // if (foo == '1') as opposed to if (foo.read() == '1').
        // Should a warning be issued here?
        // cout << "SystemC: warning: implicit conversion of lambda to bool\n";
        return ptr->eval();
    }

private:

    sc_lambda* ptr;
    sc_lambda_ptr( sc_lambda* p )
        : ptr(p)
    {
        // Update reference count
        ptr->ref_count++;
    }
    sc_lambda_ptr& operator=( sc_lambda* p )
    {
        // Update reference count on original lambda
        if (--(ptr->ref_count) == 0)
            delete ptr;
        ptr = p;
        ptr->ref_count++;
        return *this;
    }
    sc_lambda* operator->() const { return ptr; }
};


// ----------------------------------------------------------------------------
//  CLASS : sc_lambda_rand
//
//
//  <summary> Operand for the lambda expression tree </summary>
//
//  sc_lambda_rand is the representation of nodes in the lambda
//  expression trees.  Nodes could be of type lambda (a subtree),
//  int, signal of int, sc_dt::sc_logic, or signal of sc_dt::sc_logic.
//  The type is tagged in a member variable.  There's nothing of
//  interest to the user in this class.
//
// ----------------------------------------------------------------------------

enum sc_lambda_rand_e
{
    SC_LAMBDA_RAND_LAMBDA,
    SC_LAMBDA_RAND_SIGNAL_INT,
    SC_LAMBDA_RAND_SIGNAL_SUL,
    SC_LAMBDA_RAND_SIGNAL_BOOL,
    SC_LAMBDA_RAND_INT,
    SC_LAMBDA_RAND_SUL,
    SC_LAMBDA_RAND_BOOL
};


class sc_lambda_rand
{
    friend class sc_lambda;
    friend class sc_port_registry;

public:

    static void* operator new( size_t sz )
        { return sc_mempool::allocate( sz ); }

    static void operator delete( void* p, size_t sz )
        { sc_mempool::release( p, sz ); }

#include "sysc/kernel/sc_lambda_friends.h"

private:

    sc_lambda_rand_e rand_ty;
    union {
        char lamb_space[sizeof(sc_lambda_ptr)];
        char ch_space[sizeof(sc_dt::sc_logic)];
	const sc_signal_in_if<sc_dt::sc_logic>* sul_sig;
	const sc_signal_in_if<int>* int_sig;
	const sc_signal_in_if<bool>* edgy_sig;
        int val;
    };

    sc_lambda_rand( int i )
        : rand_ty( SC_LAMBDA_RAND_INT )
    {
        val = i;
    }
    sc_lambda_rand( bool b )
        : rand_ty( SC_LAMBDA_RAND_BOOL )
    {
        val = int(b);
    }
    sc_lambda_rand( const sc_dt::sc_logic& c )
        : rand_ty( SC_LAMBDA_RAND_SUL )
    {
        (void) new(ch_space) sc_dt::sc_logic(c);
    }
    sc_lambda_rand( const sc_signal_logic_deval& s )
        : rand_ty( SC_LAMBDA_RAND_SIGNAL_SUL )
    {
	sul_sig = RCAST<const sc_signal_in_if<sc_dt::sc_logic>*>( &s );
    }
    sc_lambda_rand( const sc_signal_in_if<int>& s )
        : rand_ty( SC_LAMBDA_RAND_SIGNAL_INT )
    {
        int_sig = &s;
    }
    sc_lambda_rand( const sc_signal_in_if<bool>& s )
        : rand_ty( SC_LAMBDA_RAND_SIGNAL_BOOL )
    {
        edgy_sig = &s;
    }
    sc_lambda_rand( const sc_signal_bool_deval& s )
        : rand_ty( SC_LAMBDA_RAND_SIGNAL_BOOL )
    {
        edgy_sig = RCAST<const sc_signal_in_if<bool>*>( &s );
    }

    sc_lambda_rand( const sc_lambda_ptr& l )
        : rand_ty( SC_LAMBDA_RAND_LAMBDA )
    {
        (void) new(lamb_space) sc_lambda_ptr(l);
    }

    ~sc_lambda_rand();

    int int_read() const;
    sc_dt::sc_logic sc_logic_read() const;
    bool bool_read() const;

    void replace_ports( void (*fn)(sc_port_registry*, sc_lambda_rand* ),
                        sc_port_registry* );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

#include "sysc/kernel/sc_lambda_defs.h"

} // namespace sc_core

#endif
