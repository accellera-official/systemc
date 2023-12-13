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

  sc_simcontext.h -- Definition of the simulation context class.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_SIMCONTEXT_H
#define SC_SIMCONTEXT_H

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_status.h"
#include "sysc/kernel/sc_time.h"
#include "sysc/kernel/sc_stage_callback_if.h"
#include "sysc/utils/sc_hash.h"
#include "sysc/utils/sc_pq.h"

#include "sysc/communication/sc_host_mutex.h"

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(push)
#pragma warning(disable: 4251) // DLL import for std::vector
#endif

namespace sc_core {

// forward declarations

class sc_cor;
class sc_cor_pkg;
class sc_event;
class sc_event_timed;
class sc_export_registry;
class sc_hierarchy_scope;
class sc_initializer_function;
class sc_module;
class sc_module_name;
class sc_module_registry;
class sc_name_gen;
class sc_object;
class sc_object_host;
class sc_object_manager;
class sc_stage_callback_registry;
class sc_process_handle;
class sc_port_registry;
class sc_prim_channel_registry;
class sc_process_table;
class sc_signal_bool_deval;
class sc_trace_file;
class sc_runnable;
class sc_process_host;
class sc_method_process;
class sc_cthread_process;
class sc_thread_process;
class sc_reset_finder;
class sc_stub_registry;

extern sc_simcontext* sc_get_curr_simcontext();

template< typename > class sc_plist;
typedef sc_plist< sc_process_b* > sc_process_list;

struct SC_API sc_curr_proc_info
{
    sc_process_b*     process_handle;
    sc_curr_proc_kind kind;
    sc_curr_proc_info() : process_handle( 0 ), kind( SC_NO_PROC_ ) {}
};

typedef const sc_curr_proc_info* sc_curr_proc_handle;

enum sc_stop_mode {          // sc_stop modes:
    SC_STOP_FINISH_DELTA,
    SC_STOP_IMMEDIATE
};
extern SC_API void sc_set_stop_mode( sc_stop_mode mode );
extern SC_API sc_stop_mode sc_get_stop_mode();

enum sc_starvation_policy 
{
    SC_EXIT_ON_STARVATION,
    SC_RUN_TO_TIME
};
extern SC_API void sc_start();
extern SC_API void sc_start( const sc_time& duration, 
                      sc_starvation_policy p=SC_RUN_TO_TIME );
inline void sc_start( int duration, sc_time_unit unit, 
                      sc_starvation_policy p=SC_RUN_TO_TIME )
{
    sc_start( sc_time((double)duration,unit), p );
}

inline void sc_start( double duration, sc_time_unit unit, 
                      sc_starvation_policy p=SC_RUN_TO_TIME )
{
    sc_start( sc_time(duration,unit), p );
}

extern SC_API void sc_stop();

// friend function declarations

SC_API sc_dt::uint64 sc_delta_count();
SC_API sc_dt::uint64 sc_delta_count_at_current_time();
SC_API const std::vector<sc_event*>& sc_get_top_level_events(
				const   sc_simcontext* simc_p);
SC_API const std::vector<sc_object*>& sc_get_top_level_objects(
				const   sc_simcontext* simc_p);
SC_API bool    sc_is_running( const sc_simcontext* simc_p );
SC_API void    sc_pause();
SC_API bool    sc_end_of_simulation_invoked();
SC_API void    sc_start( const sc_time&, sc_starvation_policy );
SC_API bool    sc_start_of_simulation_invoked();
SC_API void    sc_set_time_resolution( double, sc_time_unit );
SC_API sc_time sc_get_time_resolution();
SC_API void    sc_set_default_time_unit( double, sc_time_unit );
SC_API sc_time sc_get_default_time_unit();
SC_API bool    sc_pending_activity_at_current_time( const sc_simcontext* );
SC_API bool    sc_pending_activity_at_future_time( const sc_simcontext* );
SC_API sc_time sc_time_to_pending_activity( const sc_simcontext* );

SC_API void sc_register_stage_callback(sc_stage_callback_if & cb,
                                       unsigned int mask);
SC_API void sc_unregister_stage_callback(sc_stage_callback_if & cb,
                                         unsigned int mask);

class sc_invoke_method;

SC_API void    sc_suspend_all();
SC_API void    sc_unsuspend_all();
SC_API void    sc_unsuspendable();
SC_API void    sc_suspendable();

// ----------------------------------------------------------------------------
//  CLASS : sc_simcontext
//
//  The simulation context.
// ----------------------------------------------------------------------------

class SC_API sc_simcontext
{
    friend class sc_event;
    friend class sc_export_registry;
    friend class sc_hierarchy_scope;
    friend class sc_initializer_function;
    friend class sc_invoke_method;
    friend class sc_module;
    friend class sc_object;
    friend class sc_object_host;
    friend class sc_time;
    friend class sc_time_tuple;
    friend class sc_clock;
    friend class sc_method_process;
    friend class sc_stage_callback_registry;
    friend class sc_port_registry;
    friend class sc_process_b;
    friend class sc_process_handle;
    friend class sc_prim_channel;
    friend class sc_cthread_process;
    friend class sc_thread_process;
    friend SC_API sc_dt::uint64 sc_delta_count();
    friend SC_API const std::vector<sc_event*>& sc_get_top_level_events(
        const sc_simcontext* simc_p);
    friend SC_API const std::vector<sc_object*>& sc_get_top_level_objects(
        const sc_simcontext* simc_p);
    friend SC_API bool sc_is_running( const sc_simcontext* simc_p );
    friend SC_API void sc_pause();
    friend SC_API bool sc_end_of_simulation_invoked();
    friend SC_API void sc_start( const sc_time&, sc_starvation_policy );
    friend SC_API bool sc_start_of_simulation_invoked();
    friend void sc_thread_cor_fn(void*);
    friend SC_API sc_time sc_time_to_pending_activity( const sc_simcontext* );
    friend SC_API bool sc_pending_activity_at_current_time( const sc_simcontext* );
    friend SC_API bool sc_pending_activity_at_future_time( const sc_simcontext* );
    friend SC_API void sc_suspend_all();
    friend SC_API void sc_unsuspend_all();
    friend SC_API void sc_unsuspendable();
    friend SC_API void sc_suspendable();

    friend SC_API void sc_register_stage_callback(sc_stage_callback_if & cb,
                                                  unsigned int mask);
    friend SC_API void sc_unregister_stage_callback(sc_stage_callback_if & cb,
                                                    unsigned int mask);

    enum sc_signal_write_check
    {
       SC_SIGNAL_WRITE_CHECK_DISABLE_  = 0x0, // no multiple writer checks
       SC_SIGNAL_WRITE_CHECK_DEFAULT_  = 0x1, // default IEEE-1666 writer checks
       SC_SIGNAL_WRITE_CHECK_CONFLICT_ = 0x2  // only check for conflicting writes
    };


    void init();
    void clean();

public:

    sc_simcontext();
    ~sc_simcontext();

    void initialize( bool = false );
    void cycle( const sc_time& );
    void simulate( const sc_time& duration );
    void stop();
    void end();
    void reset();

    int sim_status() const;
    bool elaboration_done() const;

    std::vector<sc_thread_handle>& get_active_invokers();

    sc_object_manager* get_object_manager();

    inline sc_status get_status() const;

    sc_status get_thread_safe_status();

    inline sc_stage get_stage() const;

    sc_object_host* active_object();

    sc_object* first_object();
    sc_object* next_object();
    sc_object* find_object( const char* name );

    sc_module_registry* get_module_registry();
    sc_port_registry* get_port_registry();
    sc_export_registry* get_export_registry();
    sc_prim_channel_registry* get_prim_channel_registry();
    sc_stage_callback_registry* get_stage_cb_registry();
    sc_stub_registry* get_stub_registry();

    std::string construct_hierarchical_name(const sc_object* parent,
                                            const std::string& name);
    bool register_hierarchical_name(const sc_object* parent,
                                    const std::string& name);
    bool unregister_hierarchical_name(const sc_object* parent,
                                      const std::string& name);
    bool hierarchical_name_exists(const sc_object* parent,
                                  const std::string& name);
    const char* get_hierarchical_name(const sc_object* parent,
                                      const std::string& name);

    // to generate unique names for objects in an MT-Safe way
    const char* gen_unique_name( const char* basename_, 
                                 bool preserve_first = false 
                               );

    // process creation
    sc_process_handle create_cthread_process( 
    const char* name_p, bool free_host, sc_entry_func method_p,
    sc_process_host* host_p, const sc_spawn_options* opt_p );

    sc_process_handle create_method_process( 
    const char* name_p, bool free_host, sc_entry_func method_p,
    sc_process_host* host_p, const sc_spawn_options* opt_p );

    sc_process_handle create_thread_process( 
    const char* name_p, bool free_host, sc_entry_func method_p,
    sc_process_host* host_p, const sc_spawn_options* opt_p );

    sc_curr_proc_handle get_curr_proc_info();
    sc_process_b* get_current_writer() const;
    bool write_check() const;
    bool write_check_conflicts_only() const;
    void set_curr_proc( sc_process_b* );
    void reset_curr_proc();

    int next_proc_id();

    friend SC_API void    sc_set_time_resolution( double, sc_time_unit );
    friend SC_API sc_time sc_get_time_resolution();
    friend SC_API void    sc_set_default_time_unit( double, sc_time_unit );
    friend SC_API sc_time sc_get_default_time_unit();

    const sc_time& max_time() const;
    const sc_time& time_stamp() const;

    sc_dt::uint64 change_stamp() const;
    sc_dt::uint64 delta_count() const;
    sc_dt::uint64 delta_count_at_current_time() const;
    bool event_occurred( sc_dt::uint64 last_change_count ) const;
    bool evaluation_phase() const;
    bool is_running() const;
    bool update_phase() const;
    bool notify_phase() const;
    bool get_error();
    void set_error( sc_report* );

    sc_cor_pkg* cor_pkg()
        { return m_cor_pkg; }
    sc_cor* next_cor();

    void add_reset_finder( sc_reset_finder* );

    const ::std::vector<sc_object*>& get_child_objects() const;

    sc_event& null_event();

    void elaborate();
    void prepare_to_simulate();
    inline void initial_crunch( bool no_crunch );
    bool next_time( sc_time& t ) const; 
    bool pending_activity_at_current_time() const;

    void pre_suspend() const;
    void post_suspend() const;

private:
    void hierarchy_push(sc_object_host*);
    sc_object_host* hierarchy_pop();
    sc_object_host* hierarchy_curr() const;

    void add_child_event( sc_event* );
    void add_child_object( sc_object* );
    void remove_child_event( sc_event* );
    void remove_child_object( sc_object* );

    void crunch( bool once=false );

    int add_delta_event( sc_event* );
    void remove_delta_event( sc_event* );
    void add_timed_event( sc_event_timed* );

    void trace_cycle( bool delta_cycle );

    void execute_method_next( sc_method_handle );
    void execute_thread_next( sc_thread_handle );

    sc_method_handle pop_runnable_method();
    sc_thread_handle pop_runnable_thread();

    void preempt_with( sc_method_handle );
    inline void preempt_with( sc_thread_handle );

    void push_runnable_method( sc_method_handle );
    void push_runnable_thread( sc_thread_handle );

    void push_runnable_method_front( sc_method_handle );
    void push_runnable_thread_front( sc_thread_handle );

    void remove_runnable_method( sc_method_handle );
    void remove_runnable_thread( sc_thread_handle );

    void requeue_current_process();
    void suspend_current_process();

    void do_sc_stop_action();
    void do_timestep( const sc_time& );
    void mark_to_collect_process( sc_process_b* zombie_p );
    void do_collect_processes();

    sc_method_handle remove_process( sc_method_handle );
    sc_thread_handle remove_process( sc_thread_handle );

    inline void set_simulation_status(sc_status status);

private:

    enum execution_phases {
        phase_initialize = 0,
        phase_evaluate,
        phase_update,
        phase_notify
    };
    sc_object_manager*          m_object_manager;

    sc_module_registry*         m_module_registry;
    sc_port_registry*           m_port_registry;
    sc_export_registry*         m_export_registry;
    sc_prim_channel_registry*   m_prim_channel_registry;
    sc_stage_callback_registry* m_stage_cb_registry;
    sc_stub_registry*           m_stub_registry;

    sc_name_gen*                m_name_gen;

    sc_process_table*           m_process_table;
    sc_curr_proc_info           m_curr_proc_info;
    sc_process_b*               m_current_writer;
    sc_signal_write_check       m_write_check;
    int                         m_next_proc_id;

    std::vector<sc_thread_handle> m_active_invokers;

    std::vector<sc_event*>      m_child_events;
    std::vector<sc_object*>     m_child_objects;

    std::vector<sc_event*>      m_delta_events;
    sc_ppq<sc_event_timed*>*    m_timed_events;

    sc_event*                   m_null_event_p;

    std::vector<sc_trace_file*> m_trace_files;
    bool                        m_something_to_trace;

    sc_runnable*                m_runnable;
    sc_process_list*            m_collectable;

    sc_time_params*             m_time_params;
    sc_time                     m_curr_time;
    mutable sc_time             m_max_time;
 
    sc_invoke_method*           m_method_invoker_p;
    sc_dt::uint64               m_change_stamp; // "time" change occurred.
    sc_dt::uint64               m_delta_count;
    sc_dt::uint64               m_initial_delta_count_at_current_time;
    bool                        m_forced_stop;
    bool                        m_paused;
    bool                        m_ready_to_simulate;
    bool                        m_elaboration_done;
    execution_phases            m_execution_phase;
    sc_report*                  m_error;
    bool                        m_in_simulator_control;   
    bool                        m_end_of_simulation_called;
    sc_status                   m_simulation_status;
    sc_host_mutex               m_simulation_status_mutex;
    sc_stage                    m_stage;
    bool                        m_start_of_simulation_called;

    sc_cor_pkg*                 m_cor_pkg; // the simcontext's coroutine package
    sc_cor*                     m_cor;     // the simcontext's coroutine

    sc_reset_finder*            m_reset_finder_q; // Q of reset finders to reconcile.

    int                         m_suspend;
    int                         m_unsuspendable;

private:

    // disabled
    sc_simcontext( const sc_simcontext& );
    sc_simcontext& operator = ( const sc_simcontext& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// Not MT safe.

#if 1
extern SC_API sc_simcontext* sc_curr_simcontext;
extern SC_API sc_simcontext* sc_default_global_context;

inline sc_simcontext*
sc_get_curr_simcontext()
{
    if( sc_curr_simcontext == 0 ) {
        sc_default_global_context = new sc_simcontext;
        sc_curr_simcontext = sc_default_global_context;
    }
    return sc_curr_simcontext;
}
#else
    extern SC_API sc_simcontext* sc_get_curr_simcontext();
#endif // 0


// +------------------------------------------------------------------------------------------------
// |"sc_get_status"
// | 
// | This method returns the current simulator status, and uses a mutex mechanism to guarantee
// | thread safety. It may be called from pthreads other than the simulator's.
// |
// | Result:
// |     Current simulator status (see the sc_status enum).
// +------------------------------------------------------------------------------------------------
inline sc_status sc_get_status()
{
    return sc_get_curr_simcontext()->get_thread_safe_status();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
bool
sc_simcontext::elaboration_done() const
{
    return m_elaboration_done;
}


// +------------------------------------------------------------------------------------------------
// |"sc_simcontext::get_status"
// | 
// | This method returns the current simulator status, but does not use a mutex mechanism. It is
// | intended for use within the simulator's pthread.
// |
// | Notes:
// |     (1) To get the status from outside the simulator's thread the get_thread_safe_status()
// |         method should be used.
// | Result:
// |     Current simulator status (see the sc_status enum).
// +------------------------------------------------------------------------------------------------
inline sc_status sc_simcontext::get_status() const
{
    return m_simulation_status != SC_RUNNING ? 
                  m_simulation_status :
		  (m_in_simulator_control ? (m_suspend ? SC_SUSPENDED: SC_RUNNING) : SC_PAUSED);
}

inline sc_stage sc_simcontext::get_stage() const
{
    return m_stage;
}

inline
int
sc_simcontext::sim_status() const
{
    if( m_error ) {
        return SC_SIM_ERROR;
    }
    if( m_forced_stop ) {
        return SC_SIM_USER_STOP;
    }
    return SC_SIM_OK;
}


inline
sc_object_manager*
sc_simcontext::get_object_manager()
{
    return m_object_manager;
}

inline
sc_module_registry*
sc_simcontext::get_module_registry()
{
    return m_module_registry;
}

inline
sc_port_registry*
sc_simcontext::get_port_registry()
{
    return m_port_registry;
}

inline
sc_export_registry*
sc_simcontext::get_export_registry()
{
    return m_export_registry;
}

inline
sc_prim_channel_registry*
sc_simcontext::get_prim_channel_registry()
{
    return m_prim_channel_registry;
}

inline
sc_stage_callback_registry*
sc_simcontext::get_stage_cb_registry()
{
    return m_stage_cb_registry;
}

inline
sc_stub_registry*
sc_simcontext::get_stub_registry()
{
    return m_stub_registry;
}

inline
sc_curr_proc_handle
sc_simcontext::get_curr_proc_info()
{
    return &m_curr_proc_info;
}


inline
int
sc_simcontext::next_proc_id()
{
    return ( ++ m_next_proc_id );
}


inline
const sc_time&
sc_simcontext::max_time() const
{
    if ( m_max_time == SC_ZERO_TIME )
    {
        m_max_time = sc_time::from_value( ~sc_dt::UINT64_ZERO );
    }
    return m_max_time;
}

inline
sc_dt::uint64
sc_simcontext::change_stamp() const
{
    return m_change_stamp;
}

inline sc_dt::uint64
sc_simcontext::delta_count_at_current_time() const
{
    return m_delta_count - m_initial_delta_count_at_current_time;
}

inline
const sc_time&
sc_simcontext::time_stamp() const
{
    return m_curr_time;
}


inline 
bool
sc_simcontext::event_occurred(sc_dt::uint64 last_change_stamp) const
{
    return m_change_stamp == last_change_stamp;
}

inline
bool
sc_simcontext::evaluation_phase() const
{
    return (m_execution_phase == phase_evaluate) &&
           m_ready_to_simulate;
}

inline
bool
sc_simcontext::update_phase() const
{
    return m_execution_phase == phase_update;
}

inline
bool
sc_simcontext::notify_phase() const
{
    return m_execution_phase == phase_notify;
}

inline
void
sc_simcontext::set_error( sc_report* err )
{
    delete m_error;
    m_error = err;
}


inline
bool
sc_simcontext::get_error()
{
    return m_error != NULL;
}

inline
int
sc_simcontext::add_delta_event( sc_event* e )
{
    m_delta_events.push_back( e );
    return static_cast<int>( m_delta_events.size() - 1 );
}

inline
void
sc_simcontext::add_timed_event( sc_event_timed* et )
{
    m_timed_events->insert( et );
}

// ----------------------------------------------------------------------------

inline sc_process_b*
sc_simcontext::get_current_writer() const
{
    return m_current_writer;
}

inline void sc_simcontext::set_simulation_status(sc_status status)
{
    sc_scoped_lock lock( m_simulation_status_mutex );
    m_simulation_status = status;
}


inline bool
sc_simcontext::write_check() const
{
    return m_write_check != SC_SIGNAL_WRITE_CHECK_DISABLE_;
}

// ----------------------------------------------------------------------------

class sc_process_handle;
SC_API sc_process_handle sc_get_current_process_handle();

// Get the current object hierarchy context
//
// Returns a pointer the the sc_object (module or process) that
// would become the parent object of a newly created element
// of the SystemC object hierarchy, or NULL.
//
inline sc_object*
sc_get_current_object()
{
  return sc_get_curr_simcontext()->active_object();
}

inline
sc_process_b*
sc_get_current_process_b()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
}

// THE FOLLOWING FUNCTION IS DEPRECATED IN 2.1
extern SC_API sc_process_b* sc_get_curr_process_handle();

inline
sc_curr_proc_kind
sc_get_curr_process_kind()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->kind;
}


inline int sc_get_simulator_status()
{
    return sc_get_curr_simcontext()->sim_status();
}


// Generates unique names within each module.
extern SC_API
const char*
sc_gen_unique_name( const char* basename_, bool preserve_first = false );


// Set the random seed for controlled randomization -- not yet implemented
extern SC_API
void
sc_set_random_seed( unsigned int seed_ );


extern SC_API void sc_initialize();

extern SC_API const sc_time& sc_max_time();    // Get maximum time value.
extern SC_API const sc_time& sc_time_stamp();  // Current simulation time.
extern SC_API double sc_simulation_time();     // Current time in default time units.

inline
const std::vector<sc_event*>& sc_get_top_level_events(
    const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
    return simc_p->m_child_events;
}

inline
const std::vector<sc_object*>& sc_get_top_level_objects(
    const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
    return simc_p->m_child_objects;
}

extern SC_API sc_event* sc_find_event( const char* name );

extern SC_API sc_object* sc_find_object( const char* name );

inline
sc_dt::uint64 sc_delta_count()
{
    return sc_get_curr_simcontext()->m_delta_count;
}

inline
sc_dt::uint64 sc_delta_count_at_current_time()
{
    return sc_get_curr_simcontext()->delta_count_at_current_time();
}

inline 
bool sc_is_running( const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
    static bool stop_assert_issued = false;
    bool lrm_assert_condition_section_4_6_7_sc_is_running = ( ( sc_get_status() & (SC_RUNNING | SC_PAUSED | SC_SUSPENDED) ) != 0 );
    if ( ( !stop_assert_issued ) &&
         ( simc_p->m_ready_to_simulate != lrm_assert_condition_section_4_6_7_sc_is_running )
       ) {
        stop_assert_issued = true;  // This must be before the sc_assert!!!
        sc_assert( simc_p->m_ready_to_simulate == lrm_assert_condition_section_4_6_7_sc_is_running );
    }
    return simc_p->m_ready_to_simulate;
}

SC_API bool sc_is_unwinding();

inline void sc_pause()
{
    sc_get_curr_simcontext()->m_paused = true;
}

// Return indication if there are more processes to execute in this delta phase

inline bool sc_pending_activity_at_current_time
  ( const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
  return simc_p->pending_activity_at_current_time();
}

// Return indication if there are timed notifications in the future

inline bool sc_pending_activity_at_future_time
  ( const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
  sc_time ignored;
  return simc_p->next_time( ignored );
}

// Return indication if there are processes to run,
// or notifications in the future

inline bool sc_pending_activity
  ( const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
  return sc_pending_activity_at_current_time( simc_p )
      || sc_pending_activity_at_future_time( simc_p );
}

SC_API sc_time
sc_time_to_pending_activity
  ( const sc_simcontext* simc_p = sc_get_curr_simcontext() );


inline
bool
sc_end_of_simulation_invoked()
{
    return sc_get_curr_simcontext()->m_end_of_simulation_called;
}

inline
const char*
sc_get_current_process_name( const char * if_empty = NULL )
{
    sc_process_b* active_p; // active process to get name of.
    const char*   result;   // name of active process.

    active_p = sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
    if ( active_p )
        result = active_p->name();
    else
        result = if_empty;
    return result;
}

inline
bool
sc_hierarchical_name_exists( const char* name )
{
    return sc_get_curr_simcontext()->hierarchical_name_exists(NULL, name);
}

inline
bool
sc_hierarchical_name_exists( const sc_object* parent,
                             const char* name )
{
    return sc_get_curr_simcontext()->hierarchical_name_exists(parent, name);
}

inline
const char*
sc_get_hierarchical_name(const char* name)
{
    return sc_get_curr_simcontext()->get_hierarchical_name(NULL, name);
}

inline
const char*
sc_get_hierarchical_name(const sc_object* parent, const char* name)
{
    return sc_get_curr_simcontext()->get_hierarchical_name(parent, name);
}

inline
bool
sc_register_hierarchical_name(const char* name)
{
    return sc_get_curr_simcontext()->register_hierarchical_name(NULL, name);
}

inline
bool
sc_register_hierarchical_name(const sc_object* parent, const char* name)
{
    return sc_get_curr_simcontext()->register_hierarchical_name(parent, name);
}

inline
bool
sc_unregister_hierarchical_name(const char* name)
{
    return sc_get_curr_simcontext()->unregister_hierarchical_name(NULL, name);
}

inline
bool
sc_unregister_hierarchical_name(const sc_object* parent, const char* name)
{
    return sc_get_curr_simcontext()->unregister_hierarchical_name(parent, name);
}

inline
bool
sc_start_of_simulation_invoked()
{
    return sc_get_curr_simcontext()->m_start_of_simulation_called;
}

// The following variable controls whether process control corners should
// be considered errors or not. See sc_simcontext.cpp for details on what
// happens if this value is set to true.

extern SC_API bool sc_allow_process_control_corners;

} // namespace sc_core

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(pop)
#endif

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
  Description of Modification: - phase callbacks
                               - sc_stop mode

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: - support for dynamic process
                               - support for sc export registry
                               - new member methods elaborate(), 
                 prepare_to_simulate(), and initial_crunch()
                 that are invoked by initialize() in that order
                               - add sc_get_last_created_process_handle() for 
                 use before simulation starts
                               
      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               3 March, 2004
  Description of Modification: add sc_get_curr_process_kind()

      Name, Affiliation, Date: 
  Description of Modification: 
                               
 *****************************************************************************/
// $Log: sc_simcontext.h,v $
// Revision 1.26  2011/09/05 21:20:22  acg
//  Andy Goodrich: result of automake invocation.
//
// Revision 1.25  2011/09/01 15:28:10  acg
//  Andy Goodrich: the aftermath of automake.
//
// Revision 1.24  2011/08/29 18:04:32  acg
//  Philipp A. Hartmann: miscellaneous clean ups.
//
// Revision 1.23  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.22  2011/08/24 22:05:51  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.21  2011/05/09 04:07:49  acg
//  Philipp A. Hartmann:
//    (1) Restore hierarchy in all phase callbacks.
//    (2) Ensure calls to before_end_of_elaboration.
//
// Revision 1.20  2011/04/08 18:26:07  acg
//  Andy Goodrich: added execute_method_next() to handle method dispatch
//   for asynchronous notifications that occur outside the evaluation phase.
//
// Revision 1.19  2011/04/05 20:50:57  acg
//  Andy Goodrich:
//    (1) changes to make sure that event(), posedge() and negedge() only
//        return true if the clock has not moved.
//    (2) fixes for method self-resumes.
//    (3) added SC_PRERELEASE_VERSION
//    (4) removed kernel events from the object hierarchy, added
//        sc_hierarchical_name_exists().
//
// Revision 1.18  2011/03/20 13:43:23  acg
//  Andy Goodrich: added async_signal_is() plus suspend() as a corner case.
//
// Revision 1.17  2011/03/07 18:25:19  acg
//  Andy Goodrich: tightening of check for resume on a disabled process to
//  only produce an error if it is ready to run.
//
// Revision 1.16  2011/03/06 15:58:50  acg
//  Andy Goodrich: added escape to turn off process control corner case
//  checks.
//
// Revision 1.15  2011/03/05 04:45:16  acg
//  Andy Goodrich: moved active process calculation to the sc_simcontext class.
//
// Revision 1.14  2011/03/05 01:39:21  acg
//  Andy Goodrich: changes for named events.
//
// Revision 1.13  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.12  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.11  2011/02/13 21:34:35  acg
//  Andy Goodrich: added SC_UNINITIALIZED enum value to process status so
//  its possible to detect throws before initialization.
//
// Revision 1.10  2011/02/11 13:25:24  acg
//  Andy Goodrich: Philipp A. Hartmann's changes:
//    (1) Removal of SC_CTHREAD method overloads.
//    (2) New exception processing code.
//
// Revision 1.9  2011/02/01 21:18:56  acg
//  Andy Goodrich: addition of new preempt_with() method used to immediately
//  throw exceptions from threads.
//
// Revision 1.8  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.7  2011/01/19 23:21:50  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.6  2011/01/18 20:10:45  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.5  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
//
// Revision 1.4  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.3  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.2  2007/09/20 20:32:35  acg
//  Andy Goodrich: changes to the semantics of throw_it() to match the
//  specification. A call to throw_it() will immediately suspend the calling
//  thread until all the throwees have executed. At that point the calling
//  thread will be restarted before the execution of any other threads.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.13  2006/05/08 18:00:06  acg
// Andy Goodrich: added David Long's forward declarations for friend
//   functions, methods, and operators to keep the Microsoft compiler happy.
//
// Revision 1.11  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.10  2006/03/21 00:00:34  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.9  2006/01/26 21:04:54  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.8  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.7  2006/01/19 00:29:52  acg
// Andy Goodrich: Yet another implementation for signal write checking. This
// one uses an environment variable SC_SIGNAL_WRITE_CHECK, that when set to
// DISABLE will disable write checking on signals.
//
// Revision 1.6  2006/01/18 21:42:37  acg
// Andy Goodrich: Changes for check writer support.
//
// Revision 1.5  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//
// Revision 1.4  2006/01/03 23:18:44  acg
// Changed copyright to include 2006.
//
// Revision 1.3  2005/12/20 22:11:10  acg
// Fixed $Log lines.
//
// Revision 1.2  2005/12/20 22:02:30  acg
// Changed where delta cycles are incremented to match IEEE 1666. Added the
// event_occurred() method to hide how delta cycle comparisions are done within
// sc_simcontext. Changed the boolean update_phase to an enum that shows all
// the phases.

#endif
