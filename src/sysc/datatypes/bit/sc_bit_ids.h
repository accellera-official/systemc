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

  sc_bit_ids.h -- Report ids for the datatypes/bit code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_BIT_IDS_H
#define SC_BIT_IDS_H


#include "sysc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/bit)
//
//  Report ids in the range of 200-299.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
namespace sc_core {
    extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
}
#endif


SC_DEFINE_MESSAGE( SC_ID_LENGTH_MISMATCH_, 200,
		"length mismatch in bit/logic vector assignment" )
SC_DEFINE_MESSAGE( SC_ID_INCOMPATIBLE_TYPES_, 201,
		"incompatible types" )
SC_DEFINE_MESSAGE( SC_ID_CANNOT_CONVERT_, 202,
		"cannot perform conversion" )
SC_DEFINE_MESSAGE( SC_ID_INCOMPATIBLE_VECTORS_, 203,
		"incompatible vectors" )
SC_DEFINE_MESSAGE( SC_ID_VALUE_NOT_VALID_, 204,
		"value is not valid" )
SC_DEFINE_MESSAGE( SC_ID_ZERO_LENGTH_,     205,
		"zero length" )
SC_DEFINE_MESSAGE( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, 206,
		"vector contains 4-value logic" )  
SC_DEFINE_MESSAGE( SC_ID_SC_BV_CANNOT_CONTAIN_X_AND_Z_, 207,
		"sc_bv cannot contain values X and Z" )
SC_DEFINE_MESSAGE( SC_ID_VECTOR_TOO_LONG_,  208,
		"vector is too long: truncated" )  
SC_DEFINE_MESSAGE( SC_ID_VECTOR_TOO_SHORT_, 209,
		"vector is too short: 0-padded" )  
SC_DEFINE_MESSAGE( SC_ID_WRONG_VALUE_, 210,
		"wrong value" )
SC_DEFINE_MESSAGE( SC_ID_LOGIC_Z_TO_BOOL_, 211,
		"sc_logic value 'Z' cannot be converted to bool" )
SC_DEFINE_MESSAGE( SC_ID_LOGIC_X_TO_BOOL_, 212,
		"sc_logic value 'X' cannot be converted to bool" )




#endif

// Taf!
