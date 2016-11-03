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
 
  simple_bus_request.h : The bus interface request form.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_request_h
#define __simple_bus_request_h

enum simple_bus_lock_status { SIMPLE_BUS_LOCK_NO = 0
			      , SIMPLE_BUS_LOCK_SET
			      , SIMPLE_BUS_LOCK_GRANTED 
};

struct simple_bus_request
{
  // parameters
  unsigned int priority;

  // request parameters
  bool do_write;
  unsigned int address;
  unsigned int end_address;
  int *data;
  simple_bus_lock_status lock;

  // request status
  sc_event transfer_done;
  simple_bus_status status;

  // default constructor
  simple_bus_request();
};

inline simple_bus_request::simple_bus_request()
  : priority(0)
  , do_write(false)
  , address(0)
  , end_address(0)
  , data((int *)0)
  , lock(SIMPLE_BUS_LOCK_NO)
  , status(SIMPLE_BUS_OK)
{}

#endif
