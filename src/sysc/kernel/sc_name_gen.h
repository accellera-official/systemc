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

  sc_name_gen.h -- Unique name generator.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_name_gen.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

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

#endif

// Taf!
