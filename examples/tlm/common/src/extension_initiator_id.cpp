/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
