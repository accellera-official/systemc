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
  @file my_initiator_id_extension.cpp
  @brief This is the generic payload extension

  @Details
  This module implements the gp extension for the example. In this example, we
  define an optional extension to be the string containing initiator ID.. 
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include "extension_initiator_id.h"               ///< our header

extension_initiator_id::extension_initiator_id    ///< constructor
( void
)
{
}
 
extension_initiator_id::~extension_initiator_id   ///< destructor
( void
)
{
}

void
extension_initiator_id::copy_from               ///< copy_from
( const tlm_extension_base &extension
)
{
  m_initiator_id = static_cast<extension_initiator_id const &>(extension).m_initiator_id;
}

tlm::tlm_extension_base*
extension_initiator_id::clone                     ///< clone operation
( void
) const
{
  return new extension_initiator_id ( *this );
}
