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

/*****************************************************************************

  sc_stage_callback_if.h -- Definition of the simulation phase callbacks interface

  Original Author: Jerome Cornet, STMicroelectronics, 2021-06-02

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_STAGE_CALLBACK_IF_H_INCLUDED_
#define SC_STAGE_CALLBACK_IF_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"

namespace sc_core {

enum sc_stage {
    SC_POST_BEFORE_END_OF_ELABORATION = 0x001,
    SC_POST_END_OF_ELABORATION        = 0x002,
    SC_POST_START_OF_SIMULATION       = 0x004,
    SC_POST_UPDATE                    = 0x008,
    SC_PRE_TIMESTEP                   = 0x010,
    SC_PRE_PAUSE                      = 0x020,
    SC_PRE_SUSPEND                    = 0x040,
    SC_POST_SUSPEND                   = 0x080,
    SC_PRE_STOP                       = 0x100,
    SC_POST_END_OF_SIMULATION         = 0x200,
};

// ----------------------------------------------------------------------------
//  CLASS : sc_stage_callback_if
//
//  Interface defining the method to be called for stage callbacks.
// ----------------------------------------------------------------------------

class SC_API sc_stage_callback_if
{
public:

  typedef unsigned int stage_cb_mask;

  virtual ~sc_stage_callback_if() {}

  virtual void stage_callback(const sc_stage & stage) = 0;

};

// utility helper to print a simulation stage
SC_API std::ostream& operator << ( std::ostream& os, sc_stage s );

// ------------------------------------------------------------------

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#endif /* SC_STAGE_CALLBACK_IF_H_INCLUDED_ */
// Taf!

