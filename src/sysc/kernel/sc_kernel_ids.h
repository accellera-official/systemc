/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_kernel_ids.h -- Report ids for the kernel code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_KERNEL_IDS_H
#define SC_KERNEL_IDS_H


#include "sysc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (kernel)
//
//  Report ids in the range of 500-599.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
namespace sc_core {
    extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
}
#endif

SC_DEFINE_MESSAGE(SC_ID_NO_BOOL_RETURNED_            , 500, 
	"operator does not return boolean")
SC_DEFINE_MESSAGE(SC_ID_NO_INT_RETURNED_             , 501,
	"operator does not return int")
SC_DEFINE_MESSAGE(SC_ID_NO_SC_LOGIC_RETURNED_        , 502,
	"operator does not return sc_logic")
SC_DEFINE_MESSAGE(SC_ID_OPERAND_NOT_SC_LOGIC_        , 503,
	"operand is not sc_logic")
SC_DEFINE_MESSAGE(SC_ID_OPERAND_NOT_BOOL_            , 504,
	"operand is not bool")
SC_DEFINE_MESSAGE(SC_ID_OBJECT_EXISTS_               , 505,
	"object already exists")       
SC_DEFINE_MESSAGE(SC_ID_ILLEGAL_CHARACTERS_          , 506,
	"illegal characters" )
SC_DEFINE_MESSAGE(SC_ID_VC6_PROCESS_HELPER_          , 507,
	"internal error: sc_vc6_process_helper" )
SC_DEFINE_MESSAGE(SC_ID_VC6_MAX_PROCESSES_EXCEEDED_  , 508,
	"maximum number of processes per module exceeded (VC6)" )
SC_DEFINE_MESSAGE(SC_ID_END_MODULE_NOT_CALLED_       , 509,
	"module construction not properly completed: did "
	"you forget to add a sc_module_name parameter to "
	"your module constructor?" )
SC_DEFINE_MESSAGE(SC_ID_HIER_NAME_INCORRECT_         , 510,
	"hierarchical name as shown may be incorrect due to previous errors" )
SC_DEFINE_MESSAGE(SC_ID_SET_STACK_SIZE_              , 511,
	"set_stack_size() is only allowed for SC_THREADs and SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_SC_MODULE_NAME_USE_          , 512,
	"incorrect use of sc_module_name" )
SC_DEFINE_MESSAGE(SC_ID_SC_MODULE_NAME_REQUIRED_     , 513,
	"an sc_module_name parameter for your constructor is required" )
SC_DEFINE_MESSAGE(SC_ID_SET_TIME_RESOLUTION_         , 514,
	"set time resolution failed" )
SC_DEFINE_MESSAGE(SC_ID_SET_DEFAULT_TIME_UNIT_       , 515,
	"set default time unit failed" )
SC_DEFINE_MESSAGE(SC_ID_DEFAULT_TIME_UNIT_CHANGED_   , 516,
	"default time unit changed to time resolution" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_UNTIL_NOT_ALLOWED_      , 518,
    "wait_until() is only allowed in SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_NOT_ALLOWED_            , 519,
	"wait() is only allowed in SC_THREADs and SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_NEXT_TRIGGER_NOT_ALLOWED_    , 520,
	"next_trigger() is only allowed in SC_METHODs" )
SC_DEFINE_MESSAGE(SC_ID_IMMEDIATE_NOTIFICATION_      , 521,
	"immediate notification is not allowed during the update phase" )
SC_DEFINE_MESSAGE(SC_ID_HALT_NOT_ALLOWED_            , 522,
	"halt() is only allowed in SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WATCHING_NOT_ALLOWED_        , 523,
	"watching() is only allowed in SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_DONT_INITIALIZE_             , 524,
	"dont_initialize() has no effect for SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_N_INVALID_              , 525,
	"wait(n) is only valid for n > 0" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_              , 526,
	"make sensitive failed" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_POS_          , 527,
	"make sensitive pos failed" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_NEG_          , 528,
	"make sensitive neg failed" )
SC_DEFINE_MESSAGE(SC_ID_INSERT_MODULE_               , 529,
	"insert module failed" )
SC_DEFINE_MESSAGE(SC_ID_REMOVE_MODULE_               , 530,
	"remove module failed" )
SC_DEFINE_MESSAGE(SC_ID_NOTIFY_DELAYED_              , 531,
	"notify_delayed() cannot be called on events "
	"that have pending notifications" )
SC_DEFINE_MESSAGE(SC_ID_GEN_UNIQUE_NAME_             , 532,
	"cannot generate unique name from null string" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_NAME_STACK_EMPTY_     , 533,
	"module name stack is empty: did you forget to "
	"add a sc_module_name parameter to your module "
	"constructor?" )
SC_DEFINE_MESSAGE(SC_ID_CYCLE_MISSES_EVENTS_         , 537,
         "the simulation contains timed-events but they are "
         "ignored by sc_cycle() ==> the simulation will be "
	 "incorrect" )
SC_DEFINE_MESSAGE(SC_ID_SC_CYCLE_DEPRECATED_         , 540,
	"sc_cycle is deprecated: use sc_start(...) instead" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_METHOD_AFTER_START_   , 541,
	"call to SC_METHOD in sc_module while simulation running" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_THREAD_AFTER_START_   , 542,
	"call to SC_THREAD in sc_module while simulation running" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_CTHREAD_AFTER_START_   , 543,
	"call to SC_CTHREAD in sc_module while simulation running" )
SC_DEFINE_MESSAGE(SC_ID_SIMULATION_TIME_OVERFLOW_   , 544,
	"simulation time value overflow, simulation aborted" )
SC_DEFINE_MESSAGE(SC_ID_SIMULATION_STOP_CALLED_TWICE_ , 545,
	"sc_stop has already been called" ) 
SC_DEFINE_MESSAGE(SC_ID_SIMULATION_START_AFTER_STOP_  , 546,
	"sc_start called after sc_stop has been called" ) 
SC_DEFINE_MESSAGE(SC_ID_STOP_MODE_AFTER_START_        , 547,
	"attempt to set sc_stop mode  after start will be ignored" ) 
SC_DEFINE_MESSAGE(SC_ID_WAIT_ON_METHOD_HANDLE_        , 560,
	"Attempt to wait on process handle associated with method process" ) 
SC_DEFINE_MESSAGE(SC_ID_JOIN_ON_METHOD_HANDLE_        , 561,
	"Attempt to register method process with sc_join object" ) 
SC_DEFINE_MESSAGE(SC_ID_SC_INITIALIZE_DEPRECATED_     , 562,
	"sc_initialize is deprecated: use sc_start(0) instead" )
SC_DEFINE_MESSAGE(SC_ID_NULL_PROCESS_HANDLE           , 563,
	"Attempt to set sc_process_handle to null target" )

#endif

// Taf!
