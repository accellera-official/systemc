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

  sc_process.h -- Declarations for process classes. Requires
                  cstdlib -- We need size_t.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affiliation, date and
    changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
  Description of Modification: Changes to support dynamic processes.


 *****************************************************************************/

#ifndef SC_PROCESS_H
#define SC_PROCESS_H

#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_constants.h"

namespace sc_core {

// -----------------------------------------------------------------------------
// PROCESS HANDLES AND BASE CLASS FORWARD REFERENCE:
// -----------------------------------------------------------------------------

typedef class sc_cthread_process* sc_cthread_handle;
typedef class sc_method_process*  sc_method_handle;
typedef class sc_thread_process*  sc_thread_handle;

class sc_process_b;     // Base class for all kinds of processes.
class sc_process_host;  // Base class for objects which have processes.


// -----------------------------------------------------------------------------
// PROCESS INVOCATION METHOD OR FUNCTION:
//
//  Define SC_USE_MEMBER_FUNC_PTR if we want to use member function pointers 
//  to implement process dispatch. Otherwise, we'll use a hack that involves 
//  creating a templated invocation object which will invoke the member
//  function. This should not be necessary, but some compilers (e.g., VC++)
//  do not allow the conversion from `void (callback_tag::*)()' to 
//  `void (sc_process_host::*)()'. This is supposed to be OK as long as the 
//  dynamic type is correct.  C++ Standard 5.4 "Explicit type conversion", 
//  clause 7: a pointer to member of derived class type may be explicitly 
//  converted to a pointer to member of an unambiguous non-virtual base class 
//  type. 
// -----------------------------------------------------------------------------

#if defined(_MSC_VER)
#if ( _MSC_VER > 1200 )
#   define SC_USE_MEMBER_FUNC_PTR
#endif
#else
#   define SC_USE_MEMBER_FUNC_PTR
#endif


// COMPILER DOES SUPPORT CAST TO void (sc_process_host::*)() from (T::*)():

#if defined(SC_USE_MEMBER_FUNC_PTR)

    typedef void (sc_process_host::*SC_ENTRY_FUNC)();
#   define SC_DEFUNCT_PROCESS_FUNCTION &::sc_core::sc_process_host::defunct
#   define SC_DECL_HELPER_STRUCT(callback_tag, func) /*EMPTY*/
#   define SC_MAKE_FUNC_PTR(callback_tag, func) \
        (void (::sc_core::sc_process_host::*)())(&callback_tag::func)
        // static_cast<SC_ENTRY_FUNC>(&callback_tag::func)


// COMPILER NOT DOES SUPPORT CAST TO void (sc_process_host::*)() from (T::*)():

#else // !defined(SC_USE_MEMBER_FUNC_PTR)
	class sc_process_call_base {
	  public:
		inline sc_process_call_base()
		{
		}

		virtual ~sc_process_call_base()
		{
		}

		virtual void invoke(sc_process_host* host_p)
		{
		}
	};
	extern sc_process_call_base sc_process_defunct;

	template<class T>
	class sc_process_call : public sc_process_call_base {
	  public:
		sc_process_call( void (T::*method_p)() ) :
			sc_process_call_base()
		{
			 m_method_p = method_p;
		}

		virtual ~sc_process_call()
		{
		}

		virtual void invoke(sc_process_host* host_p)
		{
			(((T*)host_p)->*m_method_p)();
		}

	  protected:
		void (T::*m_method_p)();  // Method implementing the process.
	};

    typedef sc_process_call_base* SC_ENTRY_FUNC;
#   define SC_DEFUNCT_PROCESS_FUNCTION &::sc_core::sc_process_defunct       
#   define SC_DECL_HELPER_STRUCT(callback_tag, func) /*EMPTY*/
#   define SC_MAKE_FUNC_PTR(callback_tag, func) \
        (::sc_core::SC_ENTRY_FUNC) (new \
        ::sc_core::sc_process_call<callback_tag>(&callback_tag::func))

#endif // !defined(SC_USE_MEMBER_FUNC_PTR)


extern void sc_set_stack_size( sc_thread_handle, size_t );

} // namespace sc_core

#endif // SC_PROCESS_H


