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

  sc_string.cpp -- Implementation of a simple string class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#include "sysc/utils/sc_string.h"
#include "sysc/utils/sc_report.h"  // sc_assert
#include "sysc/utils/sc_utils_ids.h"
#include "sysc/utils/sc_report.h"

using std::isspace;
using std::strcmp;
using std::strcpy;
using std::strlen;
using std::strncpy;

namespace sc_dt {

inline static int
sc_roundup( int n, int m )
{
    return ((n - 1) / m + 1) * m;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_string_rep
//
//  Reference counting string implementation class.
// ----------------------------------------------------------------------------

class SC_API sc_string_rep
{
    sc_string_rep( int size = 16 ) :
        ref_count(1), alloc( sc_roundup( size, 16 ) ), str( new char[alloc] )
    {
        *str = '\0';
    }

    sc_string_rep( const char* s ) : ref_count(1), alloc(0), str(0)
    {
        if (s) {
            alloc = 1 + strlen(s);
            str = strcpy( new char[alloc], s );
        }
        else {
            alloc = 16;
            str = strcpy( new char[alloc], "" );
        }
    }

    sc_string_rep( const char* s, int n); // get first n chars from the string

    ~sc_string_rep()
    {
        sc_assert( ref_count == 0 );
        delete[] str;
    }

    void resize( int new_size );
    void set_string( const char* s );

    int ref_count;
    int alloc;
    char* str;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

sc_string_rep::sc_string_rep( const char* s, int n) :
    ref_count(1), alloc(0), str(0)
{
    if (s && n>0) {
        alloc = 1 + n;
        str = strncpy( new char[alloc], s,n );
        str[n] = 00;
    }
    else {
        alloc = 16;
        str = strcpy( new char[alloc], "" );
    }
}

void
sc_string_rep::resize( int new_size )
{
    if (new_size <= alloc) return;
    alloc = sc_roundup( new_size, 16 );
    char* new_str = strcpy( new char[alloc], str );
    delete[] str;
    str = new_str;
}

void
sc_string_rep::set_string( const char* s )
{
    int len = strlen(s);
    resize( len + 1 );
    strcpy( str, s );
}


#if defined(_MSC_VER)
   // Windows provides safer implementation
#  define sc_vsnprintf _vsnprintf
#else
#  define sc_vsnprintf vsnprintf
#endif

} // namespace sc_dt

// $Log: sc_string.cpp,v $
// Revision 1.6  2011/08/29 18:04:32  acg
//  Philipp A. Hartmann: miscellaneous clean ups.
//
// Revision 1.5  2011/08/26 22:49:42  acg
//  Torsten Maehne: remove redudant assignment.
//
// Revision 1.4  2011/08/26 20:46:19  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.3  2011/08/24 22:05:56  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.2  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.

// taf
