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

  sc_process.cpp -- Base process implementation.

  Original Authors: Andy Goodrich, Forte Design Systems, 17 June 2003
                    Stuart Swan, Cadence
                    Bishnupriya Bhattacharya, Cadence Design Systems,
                    25 August, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems, 12 Aug 05
  Description of Modification: This is the rewrite of process support. It 
                               contains some code from the now-defunct 
                               sc_process_b.cpp, as well as the former 
                               version of sc_process_b.cpp.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_process.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.5  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_name_gen.h"
#include "sysc/kernel/sc_cthread_process.h"
#include "sysc/kernel/sc_method_process.h"
#include "sysc/kernel/sc_thread_process.h"
#include "sysc/kernel/sc_sensitive.h"
#include "sysc/kernel/sc_process_handle.h"

namespace sc_core {

// sc_process_handle entities that are returned for null pointer instances:

std::vector<sc_object*> sc_process_handle::empty_vector;
sc_event                sc_process_handle::non_event;

// Process to delete next:
sc_process_b* sc_process_b::m_delete_next_p = 0;

// Last process that was created:
sc_process_b* sc_process_b::m_last_created_process_p = 0;

//------------------------------------------------------------------------------
//"sc_process_b::add_static_event"
//
// This method adds an event to the list of static events, and sets the
// event up to call back this process when it fires.
//------------------------------------------------------------------------------
void sc_process_b::add_static_event( const sc_event& e )
{
    sc_method_handle method_h; // This process as a method.
    sc_thread_handle thread_h; // This process as a thread.
    

    // CHECK TO SEE IF WE ARE ALREADY REGISTERED WITH THE EVENT:

    for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
        if( &e == m_static_events[i] ) {
            return;
        }
    }

    // REMEMBER THE EVENT AND THEN REGISTER OUR OBJECT INSTANCE WITH IT:

    m_static_events.push_back( &e );

    switch ( m_process_kind )
    {
      case SC_THREAD_PROC_:
      case SC_CTHREAD_PROC_:
        thread_h = DCAST<sc_thread_handle>( this );
        assert( thread_h != 0 );
        e.add_static( thread_h );
        break;
      case SC_METHOD_PROC_:
        method_h = DCAST<sc_method_handle>( this );
        assert( method_h != 0 );
        e.add_static( method_h );
        break;
      default:
        assert( false );
        break;
    }
}


//------------------------------------------------------------------------------
//"sc_process_b::dont_initialize"
//
// This virtual method sets the initialization switch for this object instance.
//------------------------------------------------------------------------------
void sc_process_b::dont_initialize( bool dont )
{
    m_dont_init = dont;
}


//------------------------------------------------------------------------------
//"sc_process_b::gen_unique_name"
//
// This method generates a unique name within this object instance's namespace.
//------------------------------------------------------------------------------
const char* sc_process_b::gen_unique_name( const char* basename_,
    bool preserve_first )
{
    if ( ! m_name_gen_p ) m_name_gen_p = new sc_name_gen;
    return m_name_gen_p->gen_unique_name( basename_, preserve_first );
}


//------------------------------------------------------------------------------
//"sc_process_b::kill_process"
//
// This method removes this object instance from use. It will be called by
// the kill_process() methods of classes derived from it. This object 
// will be removed from any data structures it resides, other than existence.
//------------------------------------------------------------------------------
void sc_process_b::kill_process()
{ 
    sc_method_handle method_h; // This process as a method.
    sc_thread_handle thread_h; // This process as a thread.

    if ( m_zombie ) return;    // Nothing to be done.


    // REMOVE ANY DYNAMIC WAITS

    switch ( m_process_kind )
    {
      case SC_THREAD_PROC_:
      case SC_CTHREAD_PROC_:
        thread_h = DCAST<sc_thread_handle>(this);
        assert( thread_h );
        if ( m_event_p ) m_event_p->remove_dynamic( thread_h );
        if ( m_event_list_p )
        {
            m_event_list_p->remove_dynamic( thread_h, 0 );
            m_event_list_p->auto_delete();
        }
        break;
      case SC_METHOD_PROC_:
        method_h = DCAST<sc_method_handle>(this);
        assert( method_h );
        if ( m_event_p ) m_event_p->remove_dynamic( method_h );
        if ( m_event_list_p )
        {
            m_event_list_p->remove_dynamic( method_h, 0 );
            m_event_list_p->auto_delete();
        }
        break;
      default: // Some other type, it needs to clean up itself.
        // std::cout << "Check " << __FILE__ << ":" << __LINE__ << std::endl;
        break;
    }


    // REMOVE STATIC WAITS, AND REMOVE THE PROCESS FROM ITS SC_RESET:

    remove_static_events();
    if ( m_reset_p ) m_reset_p->remove_process( this );


    // FIRE THE TERMINATION EVENT, MARK AS TERMINATED, AND DECREMENT THE COUNT:
    //
    // (1) We wait to set the process kind until after doing the removals
    //     above.
    // (2) Decrementing the reference count will result in actual object 
    //     deletion if we hit zero.

    m_zombie = true;
    if ( m_term_event_p ) m_term_event_p->notify();
    reference_decrement();
}

//------------------------------------------------------------------------------
//"sc_process_b::remove_static_events"
//
// This method removes this object instance from the events in its static
// event list.
//------------------------------------------------------------------------------
void
sc_process_b::remove_static_events()
{
    sc_method_handle method_h; // This process as a method.
    sc_thread_handle thread_h; // This process as a thread.

    switch ( m_process_kind )
    {
      case SC_THREAD_PROC_:
      case SC_CTHREAD_PROC_:
        thread_h = DCAST<sc_thread_handle>( this );
        assert( thread_h != 0 );
        for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
            m_static_events[i]->remove_static( thread_h );
        }
        m_static_events.resize(0);
        break;
      case SC_METHOD_PROC_:
        method_h = DCAST<sc_method_handle>( this );
        assert( method_h != 0 );
        for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
            m_static_events[i]->remove_static( method_h );
        }
        m_static_events.resize(0);
        break;
      default: // Some other type, it needs to clean up itself.
        // std::cout << "Check " << __FILE__ << ":" << __LINE__ << std::endl;
        break;
    }
}


//------------------------------------------------------------------------------
//"sc_process_b::sc_process_b"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_process_b::sc_process_b( const char* name_p, bool free_host,
     SC_ENTRY_FUNC method_p, sc_process_host* host_p, 
     const sc_spawn_options* opt_p 
) :
    sc_object( name_p ),
    proc_id( simcontext()->next_proc_id()),
    m_dont_init( false ),
    m_dynamic_proc( simcontext()->elaboration_done() ),
    m_event_p(0),
    m_event_list_p(0),
    m_exist_p(0),
    m_free_host( free_host ),
    m_last_report_p(0),
    m_name_gen_p(0),
    m_process_kind(SC_NO_PROC_),
    m_references_n(1), 
    m_reset_p(0),
    m_runnable_p(0),
    m_semantics_host_p( host_p ),
    m_semantics_method_p ( method_p ),
    m_term_event_p(0),
    m_throw_type( THROW_NONE ),
    m_timed_out(false),
    m_timeout_event_p(new sc_event),
    m_trigger_type(STATIC),
    m_zombie(false)
{

    // THIS OBJECT INSTANCE IS NOW THE LAST CREATED PROCESS:

    m_last_created_process_p = this;

}

//------------------------------------------------------------------------------
//"sc_process_b::~sc_process_b"
//
// This is the object instance destructor for this class.
//------------------------------------------------------------------------------
sc_process_b::~sc_process_b()
{
   
    // REDIRECT ANY CHILDREN AS CHILDREN OF THE SIMULATION CONTEXT:

    int size = m_child_objects.size();
    for(int i = 0; i < size; i++) 
    {
        sc_object* obj_p =  m_child_objects[i];
        obj_p->m_parent = NULL;
        simcontext()->add_child_object(obj_p);
    }


    // DELETE SEMANTICS OBJECTS IF NEED BE:

    if ( m_free_host ) delete m_semantics_host_p;
#   if !defined(SC_USE_MEMBER_FUNC_PTR) // Remove invocation object.
        delete m_semantics_method_p;
#   endif


    // REMOVE ANY STRUCTURES THAT MAY HAVE BEEN BUILT:

    if ( m_name_gen_p ) delete m_name_gen_p;
    if ( m_last_report_p ) delete m_last_report_p;
    if ( m_term_event_p ) delete m_term_event_p;
    if ( m_timeout_event_p ) delete m_timeout_event_p;

}


//------------------------------------------------------------------------------
//"sc_process_b::terminated_event"
//
// This inline method returns a reference to the terminated event for this
// object instance. If no event exists one is allocated.
//------------------------------------------------------------------------------
sc_event& sc_process_b::terminated_event()
{
    if ( m_process_kind == SC_METHOD_PROC_ )
    SC_REPORT_WARNING(SC_ID_METHOD_TERMINATION_EVENT_,"");
    if ( !m_term_event_p ) m_term_event_p = new sc_event;
    return *m_term_event_p;
}

//------------------------------------------------------------------------------
//"sc_process_handle::operator sc_cthread_handle"
//
//------------------------------------------------------------------------------
sc_process_handle::operator sc_cthread_handle()  
{
    return DCAST<sc_cthread_handle>(m_target_p); 
}

//------------------------------------------------------------------------------
//"sc_process_handle::sc_method_handle"
//
//------------------------------------------------------------------------------
sc_process_handle::operator sc_method_handle()  
{
    return DCAST<sc_method_handle>(m_target_p); 
}

//------------------------------------------------------------------------------
//"sc_process_handle::sc_thread_handle"
//
//------------------------------------------------------------------------------
sc_process_handle::operator sc_thread_handle()  
{
    return DCAST<sc_thread_handle>(m_target_p); 
}

} // namespace sc_core 
