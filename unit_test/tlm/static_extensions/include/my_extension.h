/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef __MY_EXTENSION_H__
#define __MY_EXTENSION_H__

#include "tlm.h"

class my_extension :
   public tlm::tlm_extension<my_extension>
{
public:
    my_extension()
        : m_data(0)
    {}
    tlm_extension_base* clone() const
    {
        return new my_extension(*this);
    }
    
    int m_data;
};

// For DMI, we also want to give the get_direct_mem_ptr access to
// the extension, because the address handling could depend on the
// extension:

class my_dmi_mode : public tlm::tlm_dmi_mode
{
public:
    my_extension* m_ext;
};

struct my_extended_payload_types
{
  typedef tlm::tlm_generic_payload_types::tlm_payload_type tlm_payload_type;
  typedef tlm::tlm_generic_payload_types::tlm_phase_type tlm_phase_type;
  typedef my_dmi_mode tlm_dmi_mode_type;
};

#endif
