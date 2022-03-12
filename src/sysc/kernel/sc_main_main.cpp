/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_main_main.cpp - Wrapper around user's toplevel routine `sc_main'.

  Original Author: Andy Goodrich, Forte Design Systems

 CHANGE LOG APPEARS AT THE END OF THE FILE
 *****************************************************************************/

#define SC_DISABLE_API_VERSION_CHECK // for in-library sc_ver.h inclusion

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_externs.h"
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_ver.h"
#include "sysc/utils/sc_report.h"
#include "sysc/utils/sc_report_handler.h"
#include "sysc/utils/sc_utils_ids.h"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>

namespace sc_core {

extern void pln();

static int    argc_orig;  // Original argc value passed to sc_elab_and_sim.
static char** argv_orig;  // Original argv value passed to sc_elab_and_sim.

bool sc_in_action = false;

int sc_argc()
{
    return argc_orig;
}

const char* const* sc_argv()
{
    return argv_orig;
}


int
sc_elab_and_sim( int argc, char* argv[] )
{
    int status = 1;
    argc_orig = argc;
    argv_orig = argv;

    // Copy argv into a new structure to prevent sc_main from modifying the
    // result returned from sc_argv.
    std::vector<char*> argv_copy(argc + 1, static_cast<char*>(NULL));
    for ( int i = 0; i < argc; ++i ) {
        std::size_t size = std::strlen(argv[i]) + 1;
        argv_copy[i] = new char[size];
        std::copy(argv[i], argv[i] + size, argv_copy[i]);
    }

    try
    {
        pln();

        // Perform initialization here
        sc_in_action = true;

        // copy array of pointers to keep allocated pointers for later release
        std::vector<char*> argv_call = argv_copy;
        status = sc_main( argc, &argv_call[0] );

        // Perform cleanup here
        sc_in_action = false;
    }
    catch( const sc_report& x )
    {
        sc_report_handler::get_handler()
            ( x, sc_report_handler::get_catch_actions() );
    }
    catch( ... )
    {
        // translate other escaping exceptions
        sc_report*  err_p = sc_handle_exception();
        if( err_p )
            sc_report_handler::get_handler()
                ( *err_p, sc_report_handler::get_catch_actions() );
        delete err_p;
    }

    for ( int i = 0; i < argc; ++i ) {
        delete[] argv_copy[i];
    }

    // IF DEPRECATION WARNINGS WERE ISSUED TELL THE USER HOW TO TURN THEM OFF

    if ( sc_report_handler::get_count( SC_ID_IEEE_1666_DEPRECATION_ ) > 0 )
    {
        std::stringstream ss;

        const char MSGNL[] = "\n             ";
        const char CODENL[] = "\n  ";

        ss << "You can turn off warnings about" << MSGNL
           << "IEEE 1666 deprecated features by placing this method call" << MSGNL
           << "as the first statement in your sc_main() function:\n" << CODENL
           << "sc_core::sc_report_handler::set_actions( "
           << "\"" << SC_ID_IEEE_1666_DEPRECATION_ << "\"," << CODENL
           << "                                         " /* indent param */
           << "sc_core::SC_DO_NOTHING );"
           << std::endl;

        SC_REPORT_INFO( SC_ID_IEEE_1666_DEPRECATION_, ss.str().c_str() );
    }

#if 0 // this needs work - there are issues with threads being released after sc_main_main
      // has returned. For example, attempts to call sc_simcontext::remove_process() after
      // the sc_simcontext object has been removed.

#   ifdef PURIFY
        sc_get_curr_simcontext()->clean();
#   else
        delete sc_get_curr_simcontext();
#   endif
#endif
    return status;
}

} // namespace sc_core

// $Log: sc_main_main.cpp,v $
// Revision 1.9  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.8  2011/05/09 04:07:48  acg
//  Philipp A. Hartmann:
//    (1) Restore hierarchy in all phase callbacks.
//    (2) Ensure calls to before_end_of_elaboration.
//
// Revision 1.7  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.6  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.5  2010/03/15 18:29:25  acg
//  Andy Goodrich: Changed the default stack size to 128K from 64K.
//
// Revision 1.4  2009/10/14 19:06:48  acg
//  Andy Goodrich: changed the way the "copy" of argv is handled. It is
//  now passed to sc_main, and the original is referenced via argv_copy.
//
// Revision 1.3  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.2  2008/04/11 20:41:28  acg
//  Andy Goodrich: changed the return value in sc_elab_and_sim() to be 1
//  when an exception occurs in sc_main() rather than 0.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.4  2006/01/25 00:31:19  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//
