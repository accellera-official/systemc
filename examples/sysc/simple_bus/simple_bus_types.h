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
 
  simple_bus_types.h : The common types.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_types_h
#define __simple_bus_types_h

#include <stdio.h>
#include <systemc.h>

enum simple_bus_status { SIMPLE_BUS_OK = 0
			 , SIMPLE_BUS_REQUEST
			 , SIMPLE_BUS_WAIT
			 , SIMPLE_BUS_ERROR };

// needed for more readable debug output
extern char simple_bus_status_str[4][20]; 

struct simple_bus_request;
typedef std::vector<simple_bus_request *> simple_bus_request_vec;

extern int sb_fprintf(FILE *, const char *, ...);

#endif
