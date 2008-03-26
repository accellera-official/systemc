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

  sc_main_main.cpp - Wrapper around user's toplevel routine `sc_main'.

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_externs.h"
#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_report.h"

namespace sc_core {

extern void pln();

static int    argc_copy;	// Copy of argc value passed to sc_elab_and_sim.
static char** argv_copy;	// Copy of argv value passed to sc_elab_and_sim.

static
inline
void
message_function( const char* s )
{
    ::std::cout << "\n" << s << ::std::endl;
}

bool sc_in_action = false;

int sc_argc() 
{
    return argc_copy;
}

const char* const* sc_argv() 
{
    return argv_copy;
}


int
sc_elab_and_sim( int argc, char* argv[] )
{
    int status = 0;
    argc_copy = argc;
    argv_copy = new char*[argc];
    for ( int i = 0; i < argc; i++ ) 
    argv_copy[i] = argv[i];

    try
    {
        pln();

        // Perform initialization here
        sc_in_action = true;

        status = sc_main( argc, argv );

        // Perform cleanup here
        sc_in_action = false;
    }
    catch( const sc_report& x )
    {
        message_function( x.what() );
    }
    catch( const char* s )
    {
        message_function( s );
    }
    catch( ... )
    {
        message_function( "UNKNOWN EXCEPTION OCCURED" );
    }

    delete [] argv_copy;
    return status;
}

} // namespace sc_core
