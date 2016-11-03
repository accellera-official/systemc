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
/*==============================================================================
  @file my_initiator_id_extension.h
  
  @brief This is a generic payload extension
  
  @details
  This module defines the gp extension for the example. In this example, we
  define an optional extension to be the string containing initiator ID.
  
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Charles Wilson, ESLX
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __EXTENSION_INITIATOR_ID_H__
#define __EXTENSION_INITIATOR_ID_H__

#include "tlm.h"                                    ///< TLM header
#include <string>                                   ///< standard string

class extension_initiator_id                        ///< extension class 
: public tlm::tlm_extension<extension_initiator_id> ///< tlm extension
{
  public:
  
  typedef tlm::tlm_base_protocol_types::tlm_payload_type  tlm_payload_type;
  typedef tlm::tlm_base_protocol_types::tlm_phase_type    tlm_phase_type;

  extension_initiator_id                            ///< constructor
  ( void
  );
   
  ~extension_initiator_id                           ///< destructor
  ( void
  );

  void
  copy_from
  ( const tlm_extension_base &extension
  );

  tlm::tlm_extension_base*
  clone                                             ///< clone operation
  ( void
  ) const;

  // member variables

  std::string m_initiator_id;                     ///< initiator ID string
};
/*
struct extension_initiator_id_payload_types
{
};
*/
#endif /* __EXTENSION_INITIATOR_ID_H__ */
