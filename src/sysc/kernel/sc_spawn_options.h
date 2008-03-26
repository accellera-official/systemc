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

  sc_spawn_options.h -- Process spawning options specification.

  Original Authors: Andy Goodrich, Forte Design Systems, 17 June 2003
                    Stuart Swan, Cadence,
                    Bishnupriya Bhattacharya, Cadence Design Systems,
                    25 August, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_spawn_options.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_spawn_options_h_INCLUDED)
#define sc_spawn_options_h_INCLUDED

#include <vector>
#include "sysc/communication/sc_export.h"
#include "sysc/communication/sc_signal_ports.h"

namespace sc_core {

class sc_event;
class sc_port_base;
class sc_interface;
class sc_event_finder;
class sc_process_b;

//=============================================================================
// CLASS sc_spawn_options
//
//=============================================================================
class sc_spawn_options {
    friend class sc_cthread_process;
    friend class sc_method_process;
    friend class sc_process_b;
    friend class sc_thread_process;
  public:
    sc_spawn_options() :                  
        m_dont_initialize(false), m_spawn_method(false), m_stack_size(0)
        { }

    void spawn_method()                 { m_spawn_method = true; }
    void dont_initialize()              { m_dont_initialize = true; }
    bool is_method() const              { return m_spawn_method; }
    void set_stack_size(int stack_size) { m_stack_size = stack_size; }
    void set_sensitivity(const sc_event* event) 
        { m_sensitive_events.push_back(event); }
    void set_sensitivity(sc_port_base* port_base)
        { m_sensitive_port_bases.push_back(port_base); }
    void set_sensitivity(sc_interface* interface_p) 
        { m_sensitive_interfaces.push_back(interface_p); }
    void set_sensitivity(sc_export_base* export_base) 
        { m_sensitive_interfaces.push_back(export_base->get_interface()); }
    void set_sensitivity(sc_event_finder* event_finder) 
        { m_sensitive_event_finders.push_back(event_finder); }

  private:
    sc_spawn_options( const sc_spawn_options& );
    const sc_spawn_options& operator = ( const sc_spawn_options& );

  protected:
    bool                          m_dont_initialize;         
    std::vector<const sc_event*>  m_sensitive_events;
    std::vector<sc_event_finder*> m_sensitive_event_finders; 
    std::vector<sc_interface*>    m_sensitive_interfaces;
    std::vector<sc_port_base*>    m_sensitive_port_bases;
    bool                          m_spawn_method;          // Method not thread.
    int                           m_stack_size;            // Thread stack size.
};

} // namespace sc_core

#endif // !defined(sc_spawn_options_h_INCLUDED)
