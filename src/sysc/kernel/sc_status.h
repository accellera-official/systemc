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

  sc_status.h -- Definition of the simulation phases

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-15

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_STATUS_H_INCLUDED_
#define SC_STATUS_H_INCLUDED_

#include <iosfwd>

#include "sysc/kernel/sc_cmnhdr.h"

namespace sc_core {

// simulation status codes

const int SC_SIM_OK        = 0;
const int SC_SIM_ERROR     = 1;
const int SC_SIM_USER_STOP = 2;

enum sc_status
{   // sc_get_status values:
    SC_ELABORATION               = 0x001, // during module hierarchy construction
    SC_BEFORE_END_OF_ELABORATION = 0x002, // during before_end_of_elaboration()
    SC_END_OF_ELABORATION        = 0x004, // during end_of_elaboration()
    SC_START_OF_SIMULATION       = 0x008, // during start_of_simulation()

    SC_RUNNING                   = 0x010, // initialization, evaluation or update
    SC_PAUSED                    = 0x020, // when scheduler stopped by sc_pause()
    SC_SUSPENDED                 = 0x040, // when scheduler stopped by sc_suspend_all()
    SC_STOPPED                   = 0x080, // when scheduler stopped by sc_stop()
    SC_END_OF_SIMULATION         = 0x100, // during end_of_simulation()
};

// pretty-printing of sc_status values
SC_API std::ostream& operator << ( std::ostream&, sc_status );

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#endif /* SC_STATUS_H_INCLUDED_ */
// Taf!

