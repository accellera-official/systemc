/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License")
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

// $Log: sc_int_ids.h,v $
// Revision 1.2  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:31  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

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
