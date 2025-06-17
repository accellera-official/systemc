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

  sc_cor_std_thread.h -- Coroutine implementation with std::threads.

  Original Author: Andy Goodrich 2025-06-10

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_COR_STD_THREAD_H
#define SC_COR_STD_THREAD_H


#if defined(SC_USE_STD_THREADS)

#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_cmnhdr.h"
#include <thread>

namespace sc_core {

class sc_cor_pkg_std_thread;
typedef sc_cor_pkg_std_thread sc_cor_pkg_t;

// ----------------------------------------------------------------------------
//  CLASS : sc_cor_std_thread
//
//  Coroutine class implemented with Posix Threads.
//
// ----------------------------------------------------------------------------

typedef void (my_fun)(void *);
class sc_cor_std_thread : public sc_cor
{
  public:

    // constructor
    sc_cor_std_thread( sc_cor_fn, void* arg_p );

    // destructor
    virtual ~sc_cor_std_thread();

    // module method invocator (starts thread execution)
    static void invoke_thread( void* context_p ); 

  public: 

    sc_cor_fn*                   m_cor_fn;       // Core function.
    void*                        m_cor_fn_arg;   // Core function argument.
    std::condition_variable      m_condition;    // Condition waiting for.
    std::mutex                   m_mutex;        // Mutex
    sc_cor_pkg_std_thread*       m_pkg_p;        // the creating coroutine package
    std::thread*                 m_thread_p;     // Our std::thread.

  private:

    // disabled
    sc_cor_std_thread( const sc_cor_std_thread& );
    sc_cor_std_thread& operator = ( const sc_cor_std_thread& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg_std_thread
//
//  Coroutine package class implemented with Standard Library Threads.
//
// Notes:
//   (1) The thread creation mutex and the creation condition are used to
//       suspend the thread creating another one until the created thread
//       reaches its wait in invoke_thread(). This allows us to get control
//       of thread scheduling away from the std_thread package.
// ----------------------------------------------------------------------------

class sc_cor_pkg_std_thread
  : public sc_cor_pkg
{
    // friend void sc_cor_std_thread::invoke_module_method( void* context_p );
    friend void sc_cor_std_thread::invoke_thread( void* context_p );
  public:

    // constructor
    sc_cor_pkg_std_thread( sc_simcontext* simc );

    // destructor
    virtual ~sc_cor_pkg_std_thread();

    // create a new coroutine
    virtual sc_cor* create( std::size_t stack_size, sc_cor_fn* fn, void* arg );

    // yield to the next coroutine
    virtual void yield( sc_cor* next_cor );

    // abort the current coroutine (and resume the next coroutine)
    virtual void abort( sc_cor* next_cor );

    // get the main coroutine
    virtual sc_cor* get_main();

private: 

    sc_cor_std_thread            m_main_cor;     // Main coroutine
    sc_cor_std_thread*           m_curr_cor;     // Active coroutine
    std::condition_variable      m_create_cond;  // Condition waiting for.
    std::mutex                   m_create_mutex; // Mutex for lock.

private:

    // disabled
    sc_cor_pkg_std_thread();
    sc_cor_pkg_std_thread( const sc_cor_pkg_std_thread& );
    sc_cor_pkg_std_thread& operator = ( const sc_cor_pkg_std_thread& );
};

} // namespace sc_core

#endif // SC_COR_STD_THREAD_H

#endif // defined(SC_USE_STD_THREADS)

// Taf!
