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

  sc_process_host.h -- Declarations for sc_process_host class.

  Original Author: Andy Goodrich, Forte Design Systems, 11 May 2003

 *****************************************************************************/

/*****************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affiliation, date and
    changes you are making here.

        Name, Affiliation, Date:
    Description of Modification:

 *****************************************************************************/

#ifndef SC_PROCESS_HOST_H
#define SC_PROCESS_HOST_H

#include "sysc/kernel/sc_object.h"

namespace sc_core {

// -----------------------------------------------------------------------------
// CLASS sc_process_host
//
// This is the base class for objects which may have processes defined for
// their methods (e.g., sc_module)
// -----------------------------------------------------------------------------

class sc_process_host : public sc_object
{

  public:
	sc_process_host(const char* nm=0) : sc_object(nm)
	{}
	~sc_process_host() { }
	void defunct() {}
};


// -----------------------------------------------------------------------------
// CLASS sc_process_monitor
//
// This class provides a way of monitoring a process' status (e.g., waiting 
// for a thread to complete its execution.) This class is intended to be a base
// class for classes which need to monitor a process or processes (e.g.,
// sc_join.) Its methods should be overloaded where notifications are desired.
// -----------------------------------------------------------------------------

class sc_process_monitor {
  public:
	enum {
		spm_exit = 0
	};
    virtual void signal(sc_thread_handle thread_p, int type);  
};

// -----------------------------------------------------------------------------
// SUPPORT FOR PROCESS INVOCATION IF CASTS TO void (sc_process_host::*)() ARE
// NOT ALLOWED
// -----------------------------------------------------------------------------
#if !defined(SC_USE_MEMBER_FUNC_PTR)

	template<class T>
	sc_method_handle sc_create_dynamic_method( 
		const char* name, T* host_p, void (T::*method_p)(), 
		bool dont_initialize = false
	) {
		sc_simcontext* simc = sc_get_curr_simcontext();
		return simc->create_dynamic_method_process(
		  name, (SC_ENTRY_FUNC)(new sc_process_call<T>(method_p)),
		  host_p, dont_initialize);
         }


	template<class T>
	sc_thread_handle sc_create_dynamic_thread( 
		const char* name, T* host_p, void (T::*method_p)(), 
		int stk_size = 0, bool dont_initialize = false
	) {
		sc_simcontext* simc = sc_get_curr_simcontext();
		return simc->create_dynamic_thread_process(
		  name, (SC_ENTRY_FUNC)(new sc_process_call<T>(method_p)),
		  host_p, stk_size, dont_initialize);
         }


// -----------------------------------------------------------------------------
// SUPPORT FOR PROCESS INVOCATION IF CASTS TO void (sc_process_host::*)() ARE
// LEGAL
// -----------------------------------------------------------------------------
# else

	template<class T>
	sc_method_handle sc_create_dynamic_method( 
		const char* name, T* host_p, void (T::*method_p)(), 
		bool dont_initialize = false
	) {
		sc_simcontext* simc = sc_get_curr_simcontext();
		return simc->create_dynamic_method_process(
		  name, (SC_ENTRY_FUNC)method_p,
		  host_p, dont_initialize);
         }

	template<class T>
	sc_thread_handle sc_create_dynamic_thread( 
		const char* name, T* host_p, void (T::*method_p)(), 
		int stk_size = 0, bool dont_initialize = false
	) {
		sc_simcontext* simc = sc_get_curr_simcontext();
		return simc->create_dynamic_thread_process(
		  name, (SC_ENTRY_FUNC)method_p,
		  host_p, stk_size, dont_initialize);
         }


#endif // !defined(SC_USE_MEMBER_FUNC_PTR)

} // namespace sc_core

#endif // SC_PROCESS_HOST_H

