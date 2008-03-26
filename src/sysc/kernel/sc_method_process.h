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

  sc_method_process.h -- Method process declarations

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005


 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_method_process.h,v $
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
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_method_process_h_INCLUDED)
#define sc_method_process_h_INCLUDED

#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_event.h"

namespace sc_core {

// friend function declarations
    void sc_method_cor_fn( void* );
    void sc_cmethod_cor_fn( void* );
    void sc_set_stack_size( sc_method_handle, std::size_t );
    class sc_event;
    class sc_module;
    class sc_process_table;
    class sc_process_handle;
    class sc_simcontext;
    class sc_runnable;

    void next_trigger( sc_simcontext* );
    void next_trigger( const sc_event&,
                  sc_simcontext* );
    void next_trigger( sc_event_or_list&,
                  sc_simcontext* );
    void next_trigger( sc_event_and_list&,
                  sc_simcontext* );
    void next_trigger( const sc_time&,
                  sc_simcontext* );
    void next_trigger( const sc_time&, const sc_event&,
                  sc_simcontext* );
    void next_trigger( const sc_time&, sc_event_or_list&,
                  sc_simcontext* );
    void next_trigger( const sc_time&, sc_event_and_list&,
                  sc_simcontext* );


//==============================================================================
// sc_method_process -
//
//==============================================================================
class sc_method_process : public sc_process_b {
    friend void sc_method_cor_fn( void* );
    friend void sc_cmethod_cor_fn( void* );
    friend void sc_set_stack_size( sc_method_handle, std::size_t );
    friend class sc_event;
    friend class sc_module;
    friend class sc_process_table;
    friend class sc_process_handle;
    friend class sc_simcontext;
    friend class sc_runnable;

    friend void next_trigger( sc_simcontext* );
    friend void next_trigger( const sc_event&,
                  sc_simcontext* );
    friend void next_trigger( sc_event_or_list&,
                  sc_simcontext* );
    friend void next_trigger( sc_event_and_list&,
                  sc_simcontext* );
    friend void next_trigger( const sc_time&,
                  sc_simcontext* );
    friend void next_trigger( const sc_time&, const sc_event&,
                  sc_simcontext* );
    friend void next_trigger( const sc_time&, sc_event_or_list&,
                  sc_simcontext* );
    friend void next_trigger( const sc_time&, sc_event_and_list&,
                  sc_simcontext* );

  public:
    sc_method_process( const char* name_p, bool free_host,
        SC_ENTRY_FUNC method_p, sc_process_host* host_p,
        const sc_spawn_options* opt_p );

    virtual ~sc_method_process();

    virtual const char* kind() const
        { return "sc_method_process"; }

  protected:
    virtual void kill_process();
    sc_method_handle next_exist();
    sc_method_handle next_runnable();
    void clear_trigger();
    void next_trigger( const sc_event& );
    void next_trigger( sc_event_or_list& );
    void next_trigger( sc_event_and_list& );
    void next_trigger( const sc_time& );
    void next_trigger( const sc_time&, const sc_event& );
    void next_trigger( const sc_time&, sc_event_or_list& );
    void next_trigger( const sc_time&, sc_event_and_list& );
    void set_next_exist( sc_method_handle next_p );
    void set_next_runnable( sc_method_handle next_p );
    void set_stack_size( std::size_t size );
    bool trigger_dynamic( sc_event* );

  protected:
    sc_cor*                          m_cor;        // Thread's coroutine.
    std::size_t                      m_stack_size; // Thread stack size.
    std::vector<sc_process_monitor*> m_monitor_q;  // Thread monitors.

  private: // disabled
    sc_method_process( const sc_method_process& );
    const sc_method_process& operator = ( const sc_method_process& );

};


inline
void
sc_method_process::next_trigger( const sc_event& e )
{
    clear_trigger();
    e.add_dynamic( this );
    m_event_p = &e;
    m_trigger_type = EVENT;
}

inline
void
sc_method_process::next_trigger( sc_event_or_list& el )
{
    clear_trigger();
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST;
}

inline
void
sc_method_process::next_trigger( sc_event_and_list& el )
{
    clear_trigger();
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST;
}

inline
void
sc_method_process::next_trigger( const sc_time& t )
{
    clear_trigger();
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    m_trigger_type = TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, const sc_event& e )
{
    clear_trigger();
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    e.add_dynamic( this );
    m_event_p = &e;
    m_trigger_type = EVENT_TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, sc_event_or_list& el )
{
    clear_trigger();
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, sc_event_and_list& el )
{
    clear_trigger();
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST_TIMEOUT;
}

inline
void sc_method_process::set_next_exist(sc_method_handle next_p)
{
    m_exist_p = next_p;
}

inline
sc_method_handle sc_method_process::next_exist()
{
    return (sc_method_handle)m_exist_p;
}


inline
void sc_method_process::set_next_runnable(sc_method_handle next_p)
{
    m_runnable_p = next_p;
}

inline
sc_method_handle sc_method_process::next_runnable()
{
    return (sc_method_handle)m_runnable_p;
}

} // namespace sc_core

#endif // !defined(sc_method_process_h_INCLUDED)
