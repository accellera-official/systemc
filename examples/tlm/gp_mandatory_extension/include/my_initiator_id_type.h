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
/*==============================================================================
  @file my_initiator_id_type.h
  @brief This is the user-defined protocol class based on generic payload 
         extension.

  @Details
  This module defines the new protocol class to be used in mandatory extension 
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __MY_INITIATOR_ID_TYPE_H__
#define __MY_INITIATOR_ID_TYPE_H__

#include "tlm.h"                                  ///< top tlm header

struct my_initiator_id_type                       ///< new protocol class
{
  typedef tlm::tlm_generic_payload tlm_payload_type;
  typedef tlm::tlm_phase           tlm_phase_type;
}; 
 
#endif /* __MY_INITIATOR_ID_TYPE_H__ */
