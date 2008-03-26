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

  sc_lambda.cpp - Implementation of dynamically created lambdas.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/******************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

******************************************************************************/

#include <stdio.h>

#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_lambda.h"
#include "sysc/communication/sc_port.h"

namespace sc_core {

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

sc_lambda
sc_lambda::dummy_lambda;


// create a lambda out of a bool signal

sc_lambda::sc_lambda( const sc_signal_in_if<bool>& b )
: rator_ty( SC_LAMBDA_BOOL ),
  op1( new sc_lambda_rand( b ) ),
  op2( 0 )
{
    ref_count = 0;
}


// evaluates the expression tree

bool
sc_lambda::eval() const
{
    // The compiler should optimize the switch statement to a computed
    // branch. Otherwise, we should create a table of functions to
    // speed things up.

    switch( rator_ty ) {

    /* relational operators for sc_dt::sc_logic type */
    case SC_LAMBDA_SUL_EQ:
        return ( op1->sc_logic_read() == op2->sc_logic_read() );
    case SC_LAMBDA_SUL_NE:
        return ( op1->sc_logic_read() != op2->sc_logic_read() );

    /* relational operators for integer type */
    case SC_LAMBDA_INT_EQ:
        return ( op1->int_read() == op2->int_read() );
    case SC_LAMBDA_INT_NE:
        return ( op1->int_read() != op2->int_read() );
    case SC_LAMBDA_INT_LE:
        return ( op1->int_read() <= op2->int_read() );
    case SC_LAMBDA_INT_GE:
        return ( op1->int_read() >= op2->int_read() );
    case SC_LAMBDA_INT_LT:
        return ( op1->int_read() < op2->int_read() );
    case SC_LAMBDA_INT_GT:
        return ( op1->int_read() > op2->int_read() );

    /* boolean operators */
    case SC_LAMBDA_AND:
        return ( op1->bool_read() && op2->bool_read() );
    case SC_LAMBDA_OR:
        return ( op1->bool_read() || op2->bool_read() );
    case SC_LAMBDA_NOT:
        return ( ! op1->bool_read() );
    case SC_LAMBDA_BOOL:
        return op1->bool_read();
    case SC_LAMBDA_BOOL_EQ:
        return ( op1->bool_read() == op2->bool_read() );
    case SC_LAMBDA_BOOL_NE:
        return ( op1->bool_read() != op2->bool_read() );

    case SC_LAMBDA_FALSE:
        return false;

    case SC_LAMBDA_TRUE:
        return true;

    default:
	char message[128];
	sprintf(message, "sc_lambda::eval(): %x", int(rator_ty));
        SC_REPORT_WARNING( SC_ID_NO_BOOL_RETURNED_, message);
        return false;
    }
}


// destructor

sc_lambda::~sc_lambda()
{
    assert( ( ref_count == 0 ) || ( this == &dummy_lambda ) );
    delete op1;
    delete op2;
}


// constructor - takes an operator code and two operands

sc_lambda::sc_lambda( sc_lambda_rator_e op,
                      sc_lambda_rand*   o1,
                      sc_lambda_rand*   o2 )
: rator_ty( op ),
  op1( o1 ),
  op2( o2 )
{
    ref_count = 0;
}


// evaluates the expression as an int

int
sc_lambda::int_eval() const
{
    switch( rator_ty ) {

    case SC_LAMBDA_INT_ADD:
        return ( op1->int_read() + op2->int_read() );
    case SC_LAMBDA_INT_SUB:
        return ( op1->int_read() - op2->int_read() );
    case SC_LAMBDA_INT_MUL:
        return ( op1->int_read() * op2->int_read() );
    case SC_LAMBDA_INT_DIV:
        return ( op1->int_read() / op2->int_read() );
    case SC_LAMBDA_INT_REM:
        return ( op1->int_read() % op2->int_read() );
    case SC_LAMBDA_INT_BITAND:
        return ( op1->int_read() & op2->int_read() );
    case SC_LAMBDA_INT_BITOR:
        return ( op1->int_read() | op2->int_read() );
    case SC_LAMBDA_INT_BITNOT:
        return ( ~ op1->int_read() );
    case SC_LAMBDA_INT_BITXOR:
        return ( op1->int_read() ^ op2->int_read() );
       
    default:
	char message[128];
	sprintf(message, "sc_lambda::int_eval(): %x", int(rator_ty));
        SC_REPORT_WARNING( SC_ID_NO_INT_RETURNED_, message);
        return 0;
    }
}


// evalutes the expression as a sc_dt::sc_logic

sc_dt::sc_logic
sc_lambda::sc_logic_eval() const
{
    switch( rator_ty ) {

    case SC_LAMBDA_SUL_BITAND:
        return ( op1->sc_logic_read() & op2->sc_logic_read() );
    case SC_LAMBDA_SUL_BITOR:
        return ( op1->sc_logic_read() | op2->sc_logic_read() );
    case SC_LAMBDA_SUL_BITNOT:
        return ( ~ op1->sc_logic_read() );
    case SC_LAMBDA_SUL_BITXOR:
        return ( op1->sc_logic_read() ^ op2->sc_logic_read() );

    default:
	char message[128];
	sprintf(message, "sc_lambda::sc_logic_eval(): %x", int(rator_ty));
        SC_REPORT_WARNING( SC_ID_NO_SC_LOGIC_RETURNED_, message);
        return sc_dt::sc_logic();
    }
}


void
sc_lambda::replace_ports( void (*replace_fn)( sc_port_registry*,
					      sc_lambda_rand* ),
                          sc_port_registry* port_registry )
{
    if( op1 ) {
	op1->replace_ports( replace_fn, port_registry );
    }
    if( op2 ) {
	op2->replace_ports( replace_fn, port_registry );
    }
}


// This version should be used for creating the dummy_lambda only

sc_lambda::sc_lambda()
: rator_ty( SC_LAMBDA_FALSE ),
  op1( 0 ),
  op2( 0 )
{
    static int one = 0;

    // make sure this constructor doesn't get called more than once
    assert( one == 0 );
    one ++;
    // make sure it doesn't get deleted
    ref_count = 1;
}


/*****************************************************************************/

static const char* sc_lambda_rand_names[] = {
    "SC_LAMBDA_RAND_LAMBDA",
    "SC_LAMBDA_RAND_SIGNAL_INT",
    "SC_LAMBDA_RAND_SIGNAL_SUL",
    "SC_LAMBDA_RAND_SIGNAL_BOOL",
    "SC_LAMBDA_RAND_INT",
    "SC_LAMBDA_RAND_SUL",
    "SC_LAMBDA_RAND_BOOL"
};

sc_lambda_rand::~sc_lambda_rand()
{
#if 0
  	if (SC_LAMBDA_RAND_LAMBDA == rand_ty) {
	        ((sc_lambda_ptr*) lamb_space)->~sc_lambda_ptr();
	} else if (SC_LAMBDA_RAND_SUL == rand_ty) {
	        ((sc_dt::sc_logic*) ch_space)->~sc_logic();
	}
#endif
    switch( rand_ty ) {

    case SC_LAMBDA_RAND_LAMBDA:{
        ((sc_lambda_ptr*) lamb_space)->~sc_lambda_ptr();
        break;
    }
    case SC_LAMBDA_RAND_SUL:{
	// We use an alias location here because aCC does not like the
	// cast in the destructor invocation (i.e., the style used in
	// the case above). ACG
	sc_dt::sc_logic* alias_p = (sc_dt::sc_logic*)ch_space;
        alias_p->~sc_logic();
	break;
    }
    default:
      break;
    }
    return;
}

int
sc_lambda_rand::int_read() const
{
    switch( rand_ty ) {

    case SC_LAMBDA_RAND_INT:
        return val;

    case SC_LAMBDA_RAND_SIGNAL_INT:
        return int_sig->read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->int_eval();

    default:
        ::std::cout << "sc_lambda_rand::int_read(): operand "
             << sc_lambda_rand_names[rand_ty]
             << "is not integer." << ::std::endl;
        return 0;
    }
}

sc_dt::sc_logic
sc_lambda_rand::sc_logic_read() const
{
    switch (rand_ty) {

    case SC_LAMBDA_RAND_SUL:
        return *((sc_dt::sc_logic*) ch_space);

    case SC_LAMBDA_RAND_SIGNAL_SUL:
        return sul_sig->read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->sc_logic_eval();

    default:
	char message[128];
	sprintf(message, "sc_lambda::bool_read(): %s", 
			sc_lambda_rand_names[rand_ty]);
        SC_REPORT_WARNING( SC_ID_OPERAND_NOT_SC_LOGIC_, message );
        return sc_dt::sc_logic();
    }
}

bool
sc_lambda_rand::bool_read() const
{
    switch (rand_ty) {

    case SC_LAMBDA_RAND_BOOL:
    case SC_LAMBDA_RAND_INT:
        return (val != 0);

    case SC_LAMBDA_RAND_SIGNAL_BOOL:
        return edgy_sig->read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->eval();

    default:
	char message[128];
	sprintf(message, "sc_lambda::bool_read(): %s", 
			sc_lambda_rand_names[rand_ty]);
        SC_REPORT_WARNING( SC_ID_OPERAND_NOT_BOOL_, message);
        return false;
    }
}


void
sc_lambda_rand::replace_ports(  void (*replace_fn)( sc_port_registry*,
						    sc_lambda_rand* ),
                                sc_port_registry* port_registry )
{
    switch( rand_ty ) {
    case SC_LAMBDA_RAND_LAMBDA:
        (*((sc_lambda_ptr*) lamb_space))->replace_ports( replace_fn,
							 port_registry );
        break;
    case SC_LAMBDA_RAND_SIGNAL_BOOL:
        (*replace_fn)( port_registry, this );
        break;
    default:
        break;
    }
}

} // namespace sc_core
