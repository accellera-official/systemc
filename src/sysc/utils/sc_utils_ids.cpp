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

  sc_utils_ids.cpp -- Report ids for the utils code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  Alex Riesen, Synopsys, Inc., 2003-02-02
  ported to SystemC 2.1 exception reporting.
    
 *****************************************************************************/


#include "sysc/utils/sc_report.h"


namespace sc_core {
#define SC_DEFINE_MESSAGE(id,unused,text) extern const char id[] = text;
#include "sysc/utils/sc_utils_ids.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/communication/sc_communication_ids.h"
#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/datatypes/fx/sc_fx_ids.h"
#include "sysc/datatypes/int/sc_int_ids.h"
#undef SC_DEFINE_MESSAGE


static sc_msg_def texts[] = {
#define SC_DEFINE_MESSAGE(id,n,unused) \
    { (id), 0u, {0u}, 0u, {0u}, 0u, 0u, {0u}, 0, n },

#undef SC_UTILS_IDS_H
#include "sysc/utils/sc_utils_ids.h"

#undef SC_KERNEL_IDS_H
#include "sysc/kernel/sc_kernel_ids.h"

#undef SC_COMMUNICATION_IDS_H
#include "sysc/communication/sc_communication_ids.h"

#undef SC_BIT_IDS_H
#include "sysc/datatypes/bit/sc_bit_ids.h"

#undef SC_FX_IDS_H
#include "sysc/datatypes/fx/sc_fx_ids.h"

#undef SC_INT_IDS_H
#include "sysc/datatypes/int/sc_int_ids.h"

#undef SC_DEFINE_MESSAGE
};
static sc_report_handler::msg_def_items items = {
    texts, sizeof(texts)/sizeof(*texts), false, 0
};

static
int initialize()
{
    sc_report_handler::add_static_msg_types(&items);
    return 42;
}

static int forty_two = initialize();

} // namespace sc_core

// Taf!
