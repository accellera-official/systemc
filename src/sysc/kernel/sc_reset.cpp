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

  sc_reset.cpp -- Support for reset.

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

// $Log: sc_reset.cpp,v $
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

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_reset.h"
#include "sysc/kernel/sc_process_handle.h"
#include "sysc/communication/sc_signal.h"
#include "sysc/communication/sc_signal_ports.h"


namespace sc_core {

class sc_reset_finder;
static sc_reset_finder* reset_finder_q=0;  // Q of reset finders to reconcile.

//==============================================================================
// sc_reset_finder -
//
//==============================================================================
class sc_reset_finder {
    friend class sc_reset;
  public:
    sc_reset_finder(
        const sc_in<bool>* port_p, bool level, sc_process_b* target_p);

  protected:
    bool                 m_level;     // Level for reset.
    sc_reset_finder*     m_next_p;    // Next reset finder in list.
    const sc_in<bool>*   m_port_p;    // Port for which reset is needed.
    sc_process_b*        m_target_p;  // Process to reset.

  private: // disabled
    sc_reset_finder( const sc_reset_finder& );
    const sc_reset_finder& operator = ( const sc_reset_finder& );
};

inline sc_reset_finder::sc_reset_finder(
    const sc_in<bool>* port_p, bool level, sc_process_b* target_p
) : 
    m_level(level), m_port_p(port_p), m_target_p(target_p)
{   
    m_next_p = reset_finder_q;
    reset_finder_q = this;
}


//------------------------------------------------------------------------------
//"sc_reset::notify_processes"
//
// Notify processes that there is a change in the reset signal value.
//------------------------------------------------------------------------------
void sc_reset::notify_processes()
{
    int process_i; // Index of process resetting.
    int process_n; // # of processes to reset.
    process_n = m_processes.size();
    for ( process_i = 0; process_i < process_n; process_i++ )
    {
        m_processes[process_i]->reset_changed();
    }
}


//------------------------------------------------------------------------------
//"sc_reset::read"
//
// This method returns the value of the signal associated with this object 
// instance.
//------------------------------------------------------------------------------
bool sc_reset::read()
{
    return m_iface_p->read(); 
}


//------------------------------------------------------------------------------
//"sc_reset::reconcile_resets"
//
// This static method processes the sc_reset_finders to establish the actual
// reset connections.
//
// Notes:
//   (1) For sc_cthread_process instances we do not record the process in
//       the m_processes list. This is because we want the reset to act as
//       a synchronous reset rather than an asynchronous one.
//------------------------------------------------------------------------------
void sc_reset::reconcile_resets()
{
    const sc_signal_in_if<bool>*  iface_p; // Interface to reset signal.
    sc_reset_finder*              next_p;  // Next finder to process.
    sc_reset_finder*              now_p;   // Finder currently processing.
    sc_reset*                     reset_p; // Reset object to use.

    for ( now_p = reset_finder_q; now_p; now_p = next_p )
    {
        next_p = now_p->m_next_p;
        if ( now_p->m_target_p->m_reset_p )
            SC_REPORT_ERROR(SC_ID_MULTIPLE_RESETS_,now_p->m_target_p->name());
        iface_p = DCAST<const sc_signal_in_if<bool>*>(
            now_p->m_port_p->get_interface());
        assert( iface_p != 0 );
        reset_p = iface_p->is_reset();
        now_p->m_target_p->m_reset_p = reset_p;
        now_p->m_target_p->m_reset_level = now_p->m_level;
		reset_p->m_processes.push_back(now_p->m_target_p);
        delete now_p;
    }
}


//------------------------------------------------------------------------------
//"sc_reset::remove_process"
//
//------------------------------------------------------------------------------
void sc_reset::remove_process( sc_process_b* process_p )
{
    int process_i; // Index of process resetting.
    int process_n; // # of processes to reset.

    process_n = m_processes.size();
    for ( process_i = 0; process_i < process_n; process_i++ )
    {
        if ( m_processes[process_i] == process_p )
        {
            m_processes[process_i] = m_processes[process_n-1];
            m_processes.resize(process_n-1);
            return;
        }
    }
}

//------------------------------------------------------------------------------
//"sc_reset_signal_is"
//
//------------------------------------------------------------------------------
void sc_reset::reset_signal_is( const sc_in<bool>& port, bool level )
{
    const sc_signal_in_if<bool>* iface_p;
    sc_process_b*                process_p;
    
    process_p = (sc_process_b*)sc_get_current_process_handle();
    assert( process_p );
    switch ( process_p->proc_kind() )
    {
      case SC_THREAD_PROC_:
      case SC_METHOD_PROC_:
        SC_REPORT_ERROR(SC_ID_RESET_SIGNAL_IS_NOT_ALLOWED_,"");
		break;
      case SC_CTHREAD_PROC_:
        process_p->m_reset_level = level;
        iface_p = DCAST<const sc_signal_in_if<bool>*>(port.get_interface());
        if ( iface_p )
            reset_signal_is( *iface_p, level );
        else
        {
            new sc_reset_finder( &port, level, process_p );
        }
        break;
      default:
        SC_REPORT_ERROR(SC_ID_UNKNOWN_PROCESS_TYPE_, process_p->name());
        break;
    }
}

//------------------------------------------------------------------------------
//"sc_reset::reset_signal_is"
//
// Notes:
//   (1) For sc_cthread_process instances we do not record the process in
//       the m_processes list. This is because we want the reset to act as
//       a synchronous reset rather than an asynchronous one.
//------------------------------------------------------------------------------
void sc_reset::reset_signal_is( const sc_signal_in_if<bool>& iface, bool level )
{
    sc_process_b* process_p; // Process adding reset for.
    sc_reset*        reset_p;   // Reset object.

    process_p = sc_process_b::last_created_process_base();
    assert( process_p );
    if ( process_p->m_reset_p )
        SC_REPORT_ERROR(SC_ID_MULTIPLE_RESETS_,process_p->name());
    switch ( process_p->proc_kind() )
    {
      case SC_CTHREAD_PROC_:
        process_p->m_reset_level = level;
        reset_p = iface.is_reset();
        process_p->m_reset_p = reset_p;
		reset_p->m_processes.push_back(process_p);
        break;
      case SC_THREAD_PROC_:
      case SC_METHOD_PROC_:
        SC_REPORT_ERROR(SC_ID_RESET_SIGNAL_IS_NOT_ALLOWED_,"");
        break;
      default:
        SC_REPORT_ERROR(SC_ID_UNKNOWN_PROCESS_TYPE_, process_p->name());
        break;
    }
}


} // namespace sc_core
