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

  sc_main_main.cpp - Wrapper around user's toplevel routine `sc_main'.

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_main_main.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/01/25 00:31:19  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_externs.h"
#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_report.h"
#include "sysc/utils/sc_report_handler.h"

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

    // IF DEPRECATION WARNINGS WERE ISSUED TELL THE USER HOW TO TURN THEM OFF 

    if ( sc_report_handler::get_count("/IEEE_Std_1666/deprecated") > 0 )
    {
        SC_REPORT_INFO("/IEEE_Std_1666/deprecated",
          "You can turn off warnings about\n" \
		  "             IEEE 1666 deprecated features by placing this method " \
		  "call as the\n" \
		  "             first statement in your sc_main() function:\n" \
          "\n  sc_report_handler::set_actions(\"/IEEE_Std_1666/deprecated\", " \
          "SC_DO_NOTHING);\n\n" );
    }

    delete [] argv_copy;
    return status;
}

} // namespace sc_core
