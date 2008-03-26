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

  sc_value_base.cpp -- Base class for all SystemC data values.

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_value_base.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:54:01  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <ctype.h>

#include "sysc/datatypes/int/sc_int_ids.h"
#include "sysc/datatypes/misc/sc_value_base.h"

namespace sc_dt
{

void sc_value_base::concat_clear_data( bool to_ones )
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_clear_data method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
}

bool sc_value_base::concat_get_ctrl( sc_digit* dst_p, int low_i ) const
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_get_ctrl method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
    return false;
}

bool sc_value_base::concat_get_data( sc_digit* dst_p, int low_i ) const
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_get_data method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
    return false;
}

sc_dt::uint64 sc_value_base::concat_get_uint64() const
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_get_uint64 method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
    return 0;
}

int sc_value_base::concat_length(bool* xz_present_p) const
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_length method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
    return 0;
}

void sc_value_base::concat_set( int64 src, int i_low )
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_set(int64) method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
}

void sc_value_base::concat_set( const sc_signed& src, int low_i )
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_set(sc_signed) method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
}

void sc_value_base::concat_set( const sc_unsigned& src, int low_i )
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_set(sc_unsigned) method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
}

void sc_value_base::concat_set( uint64 src, int i_low )
{
    char error_message[128];
    std::sprintf(error_message, 
    "concat_set(uint64) method not supported by this type");
    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, error_message );
}

} // namespace sc_dt
