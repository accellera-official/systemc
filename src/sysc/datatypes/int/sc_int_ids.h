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

  sc_int_ids.h -- Report ids for the datatypes/int code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_INT_IDS_H
#define SC_INT_IDS_H


#include "sysc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/int)
//
//  Report ids in the range of 400-499.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
#endif


SC_DEFINE_MESSAGE( SC_ID_INIT_FAILED_, 400, "initialization failed" )
SC_DEFINE_MESSAGE( SC_ID_ASSIGNMENT_FAILED_, 401, "assignment failed" )
SC_DEFINE_MESSAGE( SC_ID_OPERATION_FAILED_, 402, "operation failed" )
SC_DEFINE_MESSAGE( SC_ID_CONVERSION_FAILED_, 403, "conversion failed" )




#endif

// Taf!
