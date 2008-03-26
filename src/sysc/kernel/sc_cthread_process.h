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

  sc_cthread_process.h -- Clocked thread declarations

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005


 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_cthread_process.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/04/11 23:13:20  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_cthread_process_h_INCLUDED)
#define sc_cthread_process_h_INCLUDED

#include "sysc/kernel/sc_thread_process.h"

namespace sc_core {


// friend function declarations
    void wait( sc_simcontext* );
    void wait( const sc_event&,
              sc_simcontext* );
    void wait( sc_event_or_list&,
              sc_simcontext* );
    void wait( sc_event_and_list&,
              sc_simcontext* );
    void wait( const sc_time&,
              sc_simcontext* );
    void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );

    void sc_cthread_cor_fn( void* );

    void halt( sc_simcontext* );
    void wait( int,
              sc_simcontext* );

//==============================================================================
// sc_cthread_process -
//
//==============================================================================
class sc_cthread_process : public sc_thread_process {

    friend class sc_module;
    friend class sc_process_handle;
    friend class sc_process_table;
    friend class sc_thread_process;
    friend class sc_simcontext;

    friend void wait( sc_simcontext* );
    friend void wait( const sc_event&,
              sc_simcontext* );
    friend void wait( sc_event_or_list&,
              sc_simcontext* );
    friend void wait( sc_event_and_list&,
              sc_simcontext* );
    friend void wait( const sc_time&,
              sc_simcontext* );
    friend void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );

    friend void sc_cthread_cor_fn( void* );

    friend void halt( sc_simcontext* );
    friend void wait( int,
              sc_simcontext* );

  public:
    sc_cthread_process( const char* name_p, bool free_host,
        SC_ENTRY_FUNC method_p, sc_process_host* host_p,
        const sc_spawn_options* opt_p );
    virtual ~sc_cthread_process();

    virtual void dont_initialize( bool dont );
    virtual const char* kind() const
        { return "sc_cthread_process"; }

  protected:
    sc_cthread_handle next_exist();
    void set_next_exist( sc_cthread_handle next_p );

private:

    sc_cthread_process( const char*   nm,
            SC_ENTRY_FUNC fn,
            sc_process_host*    host );

    virtual void prepare_for_simulation();


    bool eval_watchlist();
    bool eval_watchlist_curr_level();

    void wait_halt();

};

//------------------------------------------------------------------------------
//"sc_cthread_process existence chain manipulations"
//
//------------------------------------------------------------------------------
inline sc_cthread_handle sc_cthread_process::next_exist()
{
    return (sc_cthread_handle)m_exist_p;
}

inline void sc_cthread_process::set_next_exist(sc_cthread_handle next_p)
{
    m_exist_p = next_p;
}

//------------------------------------------------------------------------------
//"sc_cthread_process::wait_halt"
//
//------------------------------------------------------------------------------
inline void sc_cthread_process::wait_halt()
{
    m_wait_cycle_n = 0;
    suspend_me();
    throw sc_halt();
}

} // namespace sc_core

#endif // !defined(sc_cthread_process_h_INCLUDED)
