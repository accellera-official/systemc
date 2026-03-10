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

  sc_cor_std_thread.cpp -- Coroutine implementation with std_threads.

  Original Author: Andy Goodrich 2025-06-10

 CHANGE LOG APPEARS AT THE END OF THE FILE
 *****************************************************************************/

#if !defined(_WIN32) && !defined(WIN32) && defined(SC_USE_STD_THREADS)

#include <cstring>
#include <sstream>

// ORDER OF THE INCLUDES AND namespace sc_core IS IMPORTANT!!!

#include "sysc/kernel/sc_cor_std_thread.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_report.h"

using namespace std;

namespace sc_core {

#define DEBUGF \
    if (false) std::cout << "sc_cor_std_thread.cpp(" << __LINE__ << ") "

// +------------------------------------------------------------------------------------------------
// |"sc_cor_std_thread::sc_cor_std_thread"
// | 
// | This is the object instance constructor for this object type. It really doesn't do anything 
// | other than initialize some fields. The real work occurs in sc_cor_pkg_std_thread::create().
// +------------------------------------------------------------------------------------------------
sc_cor_std_thread::sc_cor_std_thread( sc_cor_fn fn, void* arg_p )
    : m_cor_fn_arg( 0 ), m_pkg_p( 0 )
{
    DEBUGF << this << ": sc_cor_std_thread::sc_cor_std_thread()" << std::endl;
}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_std_thread::~sc_cor_std_thread"
// | 
// | This is the object instance destructor for this object type. It detaches and frees the 
// | std::thread associated with this object instance.
// +------------------------------------------------------------------------------------------------
sc_cor_std_thread::~sc_cor_std_thread()
{
    DEBUGF << this << ": sc_cor_std_thread::~sc_cor_std_thread()" << std::endl;
    m_thread_p->detach(); 
    delete m_thread_p; 
}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_std_thread::invoke_thread"
// | 
// | This static method is a helper callback that serves as the function for std::thread instances
// | created for sc_cor_std_thread instances. It picks up the actual sc_cor_std_thread instance,
// | and yields to the main thread. Upon return from that yield it invokes the function that is 
// | the thread's implementation.
// |
// | Arguments:
// |     context_p -> sc_cor_std_thread instance to invoke the thread function of.
// +------------------------------------------------------------------------------------------------
void sc_cor_std_thread::invoke_thread(void* context_p) 
{
    sc_cor_std_thread* p = static_cast<sc_cor_std_thread*>(context_p);
    DEBUGF << p << ": sc_cor_std_thread::invoke_thread()" << std::endl;

    // SUSPEND THE THREAD SO WE CAN GAIN CONTROL FROM THE STD_THREAD PACKAGE:
    //
    // Since std_cor_pkg_std_thread::create schedules each thread behind our back for its
    // initial execution we immediately suspend a newly created thread
    // here so we can control when its execution will occur. We also wake
    // up the main thread which is waiting for this thread to execute to this
    // wait point.

    std::unique_lock<std::mutex> create_lock(p->m_pkg_p->m_create_mutex, std::defer_lock);  
    std::unique_lock<std::mutex> child_lock(p->m_mutex, std::defer_lock);  

    DEBUGF << p << ": child signalling main thread " << endl;
    create_lock.lock();
    p->m_pkg_p->m_create_cond.notify_one();
    DEBUGF << p << ": child waiting to be invoked " << endl;
    child_lock.lock();
    create_lock.unlock();
    p->m_condition.wait(child_lock);
    child_lock.unlock();

    // CALL THE SYSTEMC CODE THAT WILL ACTUALLY START THE THREAD OFF:

    p->m_pkg_p->m_curr_cor = p;
    DEBUGF << p << ": about to invoke real method" << std::endl;
    (p->m_cor_fn)(p->m_cor_fn_arg);

}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_pkg_std_thread::sc_cor_pkg_std_thread"
// | 
// | This is the object instance construcctor for this class. It makes note of the sc_simcontext
// | instance for this simulation and initializes the thread creation locking mechanism.
// |
// | Arguments:
// |     simc -> sc_simcontext instance for this simulation.
// +------------------------------------------------------------------------------------------------

sc_cor_pkg_std_thread::sc_cor_pkg_std_thread( sc_simcontext* simc )
  : sc_cor_pkg( simc )
  , m_main_cor( NULL, NULL ) 
  , m_curr_cor( &m_main_cor )
{
    m_main_cor.m_pkg_p = this;

    DEBUGF << &m_main_cor << ": is main co-routine" << std::endl;
}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_pkg_std_thread::sc_cor_pkg_std_thread"
// | 
// | This is the object desctructo for this class. 
// |
// | Notes:
// |   (1) The thread creation locking mechanism will automatically be destroyed when the fields
// |       associated with it are deleted.
// +------------------------------------------------------------------------------------------------
sc_cor_pkg_std_thread::~sc_cor_pkg_std_thread()
{
}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_pkg_std_thread::create"
// | 
// | This method creates a co-routine (thread) using the supplied information. It creates the
// | std::thread that implements the co-routine and intializes the information associated with
// | the execution of the thread.
// |
// | Notes:
// |   (1) The stack information is currently ignored as the std::thread package supplies an
// |       8 MB stack by default. 
// |
// | Arguments:
// |   stack_size = size of the stack to create for the co-routine. (Ignored see note 1 above)
// |   fn         = function implementing the co-routine.
// +------------------------------------------------------------------------------------------------
sc_cor*
sc_cor_pkg_std_thread::create( std::size_t stack_size, sc_cor_fn* fn, void* arg )
{
    sc_cor_std_thread* cor_p = new sc_cor_std_thread( fn, arg ); 
    DEBUGF << &m_main_cor << ": sc_cor_pkg_std_thread::create("
           << cor_p << ")" << std::endl;


    // INITIALIZE OBJECT'S FIELDS FROM ARGUMENT LIST:

    cor_p->m_pkg_p = this;
    cor_p->m_cor_fn = fn;
    cor_p->m_cor_fn_arg = arg;


    // ALLOCATE THE STD LIBRARY THREAD TO USE AND FORCE SEQUENTIAL EXECUTION:
    //
    // Because std_thread_create causes the created thread to be executed,
    // we need to let it run until we can block in the invoke_thread.
    // So we:
    //   (1) Create the thread instance.
    //   (2) Sleep on the creation condition, which will be signalled by
    //       the newly created thread just before it goes to sleep in
    //       invoke_thread.
    // This scheme results in the newly created thread being dormant before
    // the main thread continues execution.

    std::unique_lock<std::mutex> create_lock(m_create_mutex, std::defer_lock);  
    create_lock.lock();
    DEBUGF << &m_main_cor << ": about to create actual thread "
           << cor_p << std::endl;
    cor_p->m_thread_p = new std::thread(sc_cor_std_thread::invoke_thread, cor_p);

    DEBUGF << &m_main_cor << ": main thread waiting for signal from "
           << cor_p << std::endl;
    m_create_cond.wait(create_lock);
    create_lock.unlock();
    
    DEBUGF << &m_main_cor << ": exiting sc_cor_pkg_std_thread::create("
           << cor_p << ")" << std::endl;

    return cor_p;
}


// +------------------------------------------------------------------------------------------------
// |"sc_cor_pkg_std_thread::yield"
// | 
// | This method yields the execcution of the thread associated with this object instance to 
// | the thread associated with the supplied object instance. On return the from the wait this
// | object instances thread will continue execution.
// |
// | Globals:
// |     m_curr_cor =  current objecct instance that is executing.
// | Arguments:
// |     next_cor_p -> next object instance to execute the thread of.
// +------------------------------------------------------------------------------------------------
void
sc_cor_pkg_std_thread::yield( sc_cor* next_cor_p )
{
    sc_cor_std_thread* from_p = m_curr_cor;
    sc_cor_std_thread* to_p = static_cast<sc_cor_std_thread*>(next_cor_p);

    DEBUGF << from_p << ": switch to " << to_p << std::endl;
    if ( to_p != from_p )
    {
	std::unique_lock<std::mutex> to_lock(to_p->m_mutex,std::defer_lock);
	std::unique_lock<std::mutex> from_lock(from_p->m_mutex,std::defer_lock);

	to_lock.lock();
	to_p->m_condition.notify_one();
	from_lock.lock();
	to_lock.unlock();
	from_p->m_condition.wait(from_lock);
	from_lock.unlock();
    }

    m_curr_cor = from_p; // When we come out of wait make ourselves active.
    DEBUGF << from_p << " restarting after yield to " << to_p << std::endl;
}


// abort the current coroutine (and resume the next coroutine)

void
sc_cor_pkg_std_thread::abort( sc_cor* next_cor_p )
{
    sc_cor_std_thread* n_p = static_cast<sc_cor_std_thread*>(next_cor_p);

    DEBUGF << m_curr_cor << ": aborting, switching to " << n_p << std::endl;
    {
	std::unique_lock<std::mutex> to_lock(n_p->m_mutex);
	n_p->m_condition.notify_one();
    }
}


// +------------------------------------------------------------------------------------------------
// |"get_main"
// | 
// | This method returns the sc_cor_std_thread instance associated with the main thread.
// |
// | Globals:
// |     m_main_cor -> main thread's sc_cor_std_thread instance.
// | Returns:
// |    main thread's sc_cor_std_thread instance.
// +------------------------------------------------------------------------------------------------
sc_cor*
sc_cor_pkg_std_thread::get_main()
{
    return &m_main_cor;
}

} // namespace sc_core

#endif // !defined(_WIN32) && !defined(WIN32) && defined(SC_USE_STD_THREADS)


// Taf!
