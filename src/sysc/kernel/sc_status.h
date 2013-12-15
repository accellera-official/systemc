/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_status.h -- Definition of the simulation phases

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-15

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_STATUS_H_INCLUDED_
#define SC_STATUS_H_INCLUDED_

#include <iosfwd>

namespace sc_core {

// simulation status codes

const int SC_SIM_OK        = 0;
const int SC_SIM_ERROR     = 1;
const int SC_SIM_USER_STOP = 2;

enum sc_status
{   // sc_get_status values:
    SC_UNITIALIZED=0x00,                 // initialize() not called yet

    SC_ELABORATION               = 0x01, // during module hierarchy construction
    SC_BEFORE_END_OF_ELABORATION = 0x02, // during before_end_of_elaboration()
    SC_END_OF_ELABORATION        = 0x04, // during end_of_elaboration()
    SC_START_OF_SIMULATION       = 0x08, // during start_of_simulation()

    SC_RUNNING                   = 0x10, // initialization, evaluation or update
    SC_PAUSED                    = 0x20, // when scheduler stopped by sc_pause()
    SC_STOPPED                   = 0x40, // when scheduler stopped by sc_stop()
    SC_END_OF_SIMULATION         = 0x80, // during end_of_simulation()

    // detailed simulation phases (for dynamic callbacks)
    SC_END_OF_INITIALIZATION     = 0x100, // after initialization
//    SC_END_OF_EVALUATION         = 0x200, // between eval and update
    SC_END_OF_UPDATE             = 0x400, // after update/notify phase
    SC_BEFORE_TIMESTEP           = 0x800, // before next time step

    SC_STATUS_LAST               = SC_BEFORE_TIMESTEP,
    SC_STATUS_ANY                = 0xdff
};

// pretty-printing of sc_status values
std::ostream& operator << ( std::ostream&, sc_status );

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#endif /* SC_STATUS_H_INCLUDED_ */
// Taf!

