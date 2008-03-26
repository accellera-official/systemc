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

  sc_process_b.h -- Process base class.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
  Description of Modification: Changes for dynamic processes.

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: - provide children sc_objects (dynamic processes)
                               - provide integer child count field to 
                                 generate unique names for un-named dynamic
                                 processes in a scheduler-ordering-independent
                                 fashion

 *****************************************************************************/

#ifndef SC_PROCESS_B_H
#define SC_PROCESS_B_H

#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_process.h"

namespace sc_core {

class sc_name_gen;
class sc_report;

// ----------------------------------------------------------------------------
//  CLASS : sc_process_b
//
//  Process base class.
// ----------------------------------------------------------------------------

class sc_process_b
: public sc_object
{
	friend class sc_object;
    friend class sc_port_base;
	friend class sc_runnable;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;
    friend class sc_module;
    friend class sc_report_handler;
    friend const char* sc_gen_unique_name( const char*, bool preserve_first );

    friend bool timed_out( sc_simcontext* );

    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );

public:

    SC_ENTRY_FUNC entry_fn;
    sc_process_host*    host;
    const char*   file;
    int           lineno;
    int           proc_id;


    virtual const char* kind() const
        { return "sc_process_b"; }

    virtual bool is_cthread() const;

    static sc_process_b* get_last_created_process();

    const ::std::vector<sc_object*>& get_child_objects() const;

protected:

    sc_process_b( const char*   nm,
		  SC_ENTRY_FUNC fn,
		  sc_process_host*    host );
    virtual ~sc_process_b();

    bool do_initialize() const;
    void do_initialize( bool );

    void add_static_event( const sc_event& );
    void remove_static_events();

    const char* gen_unique_name( const char* basename_, bool preserve_first );

    bool trigger_static();
    
    inline sc_report* get_last_report()              { return m_last_report; }
    inline void set_last_report( sc_report* last_p ) 
        { 
	    if ( m_last_report ) delete m_last_report;
	     m_last_report = last_p;
	}

    bool is_runnable();

    void execute();

    bool timed_out() const;

protected:

    enum trigger_t
    {
	STATIC,
	EVENT,
	OR_LIST,
	AND_LIST,
	TIMEOUT,
	EVENT_TIMEOUT,
	OR_LIST_TIMEOUT,
	AND_LIST_TIMEOUT
    };

    sc_pvector<sc_object*>      m_child_objects;
    bool                        m_do_initialize;
    const sc_event*             m_event;
    int                         m_event_count;
    sc_event_list*              m_event_list;
    sc_process_b*		m_exist_p;        // existence link
    static sc_process_b*	m_last_created_p; // last created process
    sc_report*                  m_last_report;
    sc_name_gen*                m_name_gen_p;     // Subprocess name generator.
    sc_process_b*		m_runnable_p;     // sc_runnable link
    sc_pvector<const sc_event*> m_static_events;
    bool                        m_timed_out;
    sc_event                    m_timeout_event;
    trigger_t                   m_trigger_type;
};

} // namespace sc_core

#endif

// Taf!
