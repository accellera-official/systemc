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
  unsigned int i;
  // at least one request is here
  simple_bus_request *best_request = requests[0]; 

  if (m_verbose) 
    { // shows the list of pending requests
      sb_fprintf(stdout, "%s %s :", sc_time_stamp().to_string().c_str(), name());
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
