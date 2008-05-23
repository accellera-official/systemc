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
  @file my_initiator_id_extension.h
  @brief This is the generic payload extension

  @Details
  This module defines the gp extension for the example. In this example, we
  define an optional extension to be the string containing initiator ID.. 
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __MY_INITIATOR_ID_EXTENSION_H__
#define __MY_INITIATOR_ID_EXTENSION_H__

#include "tlm.h"                                      ///< top tlm header
#include <string>

namespace gp_extension {
class my_initiator_id_extension                                 ///< extension class 
  : public tlm::tlm_extension<my_initiator_id_extension>        ///< tlm extension
{
public:
//==============================================================================
// Constructors & Destructor 
//==============================================================================
    my_initiator_id_extension(void)        ///< constructor
    {};
     
    /// Destructor
    ~my_initiator_id_extension(void)
    {};                                    ///< destructor

    tlm::tlm_extension_base* clone() const
    {
      return new my_initiator_id_extension(*this);
    }

//==============================================================================
    std::string      m_initiator_ID;             

//==============================================================================
// Private member variables and methods
//==============================================================================
}; 
}
 #endif /* __MY_INITIATOR_ID_EXTENSION_H__ */
