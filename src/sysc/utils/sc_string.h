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

  sc_string.h -- Implementation of a simple string class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

// $Log: sc_string.h,v $
// Revision 1.3  2011/08/26 20:46:19  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
#ifndef SC_STRING_H
#define SC_STRING_H


#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_report.h"
#include <iostream>

// For legacy code using sc_string:
#ifdef SC_USE_STD_STRING
typedef ::std::string sc_string;
#endif // SC_USE_STD_STRING

namespace sc_dt {

// ----------------------------------------------------------------------------
//  ENUM : sc_numrep
//
//  Enumeration of number representations for character string conversion.
// ----------------------------------------------------------------------------

enum sc_numrep
{
    SC_NOBASE = 0,
    SC_BIN    = 2,
    SC_OCT    = 8,
    SC_DEC    = 10,
    SC_HEX    = 16,
    SC_BIN_US,
    SC_BIN_SM,
    SC_OCT_US,
    SC_OCT_SM,
    SC_HEX_US,
    SC_HEX_SM,
    SC_CSD
};

// We use typedefs for istream and ostream here to get around some finickiness
// from aCC:

typedef ::std::istream systemc_istream;
typedef ::std::ostream systemc_ostream;

const std::string to_string( sc_numrep );

//------------------------------------------------------------------------------
//"sc_io_base"
//
// This inline function returns the type of an i/o stream's base as a SystemC
// base designator.
//     stream_object = reference to the i/o stream whose base is to be returned.
//
//"sc_io_show_base"
//
// This inline function returns true if the base should be shown when a SystemC
// value is displayed via the supplied stream operator.
//     stream_object = reference to the i/o stream to return showbase value for.
//------------------------------------------------------------------------------
#if defined(__GNUC__) || defined(_MSC_VER)
    inline sc_numrep sc_io_base( systemc_ostream& stream_object,
        sc_numrep def_base )
    {
	::std::ios::fmtflags flags =
	    stream_object.flags() & ::std::ios::basefield;
	if ( flags & ::std::ios::dec ) return  SC_DEC;
	if ( flags & ::std::ios::hex ) return  SC_HEX;
	if ( flags & ::std::ios::oct ) return  SC_OCT;
	return def_base;
    }
    inline bool sc_io_show_base( systemc_ostream& stream_object )
    {
	return (stream_object.flags() & ::std::ios::showbase) != 0 ;
    }
#else   // Other
    inline sc_numrep sc_io_base( systemc_ostream& stream_object,
        sc_numrep def_base )
    {
        return SC_DEC;
    }
    inline bool sc_io_show_base( systemc_ostream& stream_object )
    {
        return false;
    }
#endif


} // namespace sc_dt

// Revision 1.2  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.4  2006/05/08 17:50:51  acg
//   Andy Goodrich: added David Long's forward declarations for friend
//   functions, methods, and operators to keep the Microsoft compiler happy.
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

#endif
