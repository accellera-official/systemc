/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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
 
  simple_bus_arbiter.cpp : The arbitration unit.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#include "simple_bus_arbiter.h"

simple_bus_request *
simple_bus_arbiter::arbitrate(const simple_bus_request_vec &requests)
{
  int i;
  // at least one request is here
  simple_bus_request *best_request = requests[0]; 

  if (m_verbose) 
    { // shows the list of pending requests
      sb_fprintf(stdout, "%g %s :", sc_time_stamp().to_double(), name());
      for (i = 0; i < requests.size(); ++i)
	{
	  simple_bus_request *request = requests[i];
          // simple_bus_lock_status encoding
          const char lock_chars[] = { '-', '=', '+' };
          // simple_bus_status encoding
           sb_fprintf(stdout, "\n    R[%d](%c%s@%x)",
                     request->priority,
                     lock_chars[request->lock],
                     simple_bus_status_str[request->status],
                     request->address);
	}
    }

  // highest priority: status==SIMPLE_BUS_WAIT and lock is set: 
  // locked burst-action
  for (i = 0; i < requests.size(); ++i)
    {
      simple_bus_request *request = requests[i];
      if ((request->status == SIMPLE_BUS_WAIT) &&
	  (request->lock == SIMPLE_BUS_LOCK_SET))
	{
	  // cannot break-in a locked burst
	  if (m_verbose)
            sb_fprintf(stdout, " -> R[%d] (rule 1)\n", request->priority);
	  return request;
	}
    }

  // second priority: lock is set at previous call, 
  // i.e. SIMPLE_BUS_LOCK_GRANTED
  for (i = 0; i < requests.size(); ++i)
    if (requests[i]->lock == SIMPLE_BUS_LOCK_GRANTED)
      {
	if (m_verbose)
	  sb_fprintf(stdout, " -> R[%d] (rule 2)\n", requests[i]->priority);
	return requests[i];
      }

  // third priority: priority
  for (i = 1; i < requests.size(); ++i)
    {
      sc_assert(requests[i]->priority != best_request->priority);
      if (requests[i]->priority < best_request->priority)
	best_request = requests[i];
    }

  if (best_request->lock != SIMPLE_BUS_LOCK_NO)
    best_request->lock = SIMPLE_BUS_LOCK_GRANTED;
	
  if (m_verbose) 
    sb_fprintf(stdout, " -> R[%d] (rule 3)\n", best_request->priority);

  return best_request;
}
