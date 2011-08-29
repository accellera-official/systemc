/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_name_gen.h -- Unique name generator.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_NAME_GEN
#define SC_NAME_GEN


#include <cstdio>

#include "sysc/utils/sc_hash.h"

namespace sc_core{

// ----------------------------------------------------------------------------
//  CLASS : sc_name_gen
//
//  Unique name generator class.
// ----------------------------------------------------------------------------

class sc_name_gen
{
public:

    sc_name_gen();
    ~sc_name_gen();

    const char* gen_unique_name( const char* basename_, 
                                 bool preserve_first = false 
                               );

private:

    sc_strhash<int*> m_unique_name_map;
    char             m_unique_name[BUFSIZ];

private:

    // disabled
    sc_name_gen( const sc_name_gen& );
    sc_name_gen& operator = ( const sc_name_gen& );
};

} // namespace sc_core

// $Log: sc_name_gen.h,v $
// Revision 1.5  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.4  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.3  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.2  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.

#endif

// Taf!
