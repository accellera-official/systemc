/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License")
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_fx_ids.h -- Report ids for the datatypes/fx code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_FX_IDS_H
#define SC_FX_IDS_H


#include "sysc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/fx)
//
//  Report ids in the range of 300-399.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
namespace sc_core {
    extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
}
#endif


SC_DEFINE_MESSAGE( SC_ID_INVALID_WL_, 300,
	"total wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_N_BITS_, 301,
	"number of bits < 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_DIV_WL_, 302,
	"division wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_CTE_WL_, 303,
	"constant wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_MAX_WL_, 304,
	"maximum wordlength <= 0 and != -1 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_FX_VALUE_, 305,
    "invalid fixed-point value" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_O_MODE_, 306,
	"invalid overflow mode" )
SC_DEFINE_MESSAGE( SC_ID_OUT_OF_RANGE_, 307,
	"index out of range" )
SC_DEFINE_MESSAGE( SC_ID_CONTEXT_BEGIN_FAILED_, 308,
	"context begin failed" )  
SC_DEFINE_MESSAGE( SC_ID_CONTEXT_END_FAILED_, 309, 
	"context end failed" )
SC_DEFINE_MESSAGE( SC_ID_WRAP_SM_NOT_DEFINED_, 310, 
	"SC_WRAP_SM not defined for unsigned numbers" )



#endif

// Taf!
