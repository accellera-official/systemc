/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
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

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

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

// +============================================================================
// | CLASS sc_spawn_reset_base - Class to do a generic access to an 
// |                             sc_spawn_rest object instance
// +===========================================================================
class sc_spawn_reset_base
{
  public:
    sc_spawn_reset_base( bool async, bool level ) : 
	m_async( async ), m_level(level)
    {}
    virtual ~sc_spawn_reset_base() {}
    virtual void specify_reset() = 0;

  protected:
    bool m_async;   // = true if async reset.
    bool m_level;   // level indicating reset.
};

// +============================================================================
// | CLASS sc_spawn_reset<SOURCE> - Reset specification for sc_spawn_options.
// +===========================================================================
template<typename SOURCE>
class sc_spawn_reset : public sc_spawn_reset_base
{
  public:
    sc_spawn_reset( bool async, const SOURCE& source, bool level ) :
	sc_spawn_reset_base(async, level), m_source(source)
    {}
    virtual ~sc_spawn_reset() {}
    virtual void specify_reset()
    {
	sc_reset::reset_signal_is( m_async, m_source, m_level );
    }

  protected:
    const SOURCE& m_source; // source of reset signal.
};

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
        m_dont_initialize(false), m_resets(), m_sensitive_events(),
        m_sensitive_event_finders(), m_sensitive_interfaces(),
        m_sensitive_port_bases(), m_spawn_method(false), m_stack_size(0)
        { }

    ~sc_spawn_options()
    {
        size_t resets_n = m_resets.size();
	for ( size_t reset_i = 0; reset_i < resets_n; reset_i++ )
	    delete m_resets[reset_i];
    }

    void async_reset_signal_is( const sc_in<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_in<bool> >(true, port, level) );
    }

    void async_reset_signal_is( const sc_inout<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_inout<bool> >(true, port, level) );
    }

    void async_reset_signal_is( const sc_out<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_out<bool> >(true, port, level) );
    }

    void async_reset_signal_is( const sc_signal_in_if<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_signal_in_if<bool> >(true, port, level) );
    }

    void dont_initialize()   { m_dont_initialize = true; }

    bool is_method() const   { return m_spawn_method; }

    void reset_signal_is( const sc_in<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_in<bool> >(false, port, level) );
    }

    void reset_signal_is( const sc_inout<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_inout<bool> >(false, port, level) );
    }

    void reset_signal_is( const sc_out<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_out<bool> >(false, port, level) );
    }

    void reset_signal_is( const sc_signal_in_if<bool>& port, bool level )
    {
        m_resets.push_back(
	    new sc_spawn_reset<sc_signal_in_if<bool> >(false, port, level) );
    }

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

    void spawn_method()                 { m_spawn_method = true; }

  protected:
    void specify_resets() const
    {
        size_t resets_n; // number of reset specifications to process.
	resets_n = m_resets.size();
	for ( size_t reset_i = 0; reset_i < resets_n; reset_i++ )
	{
	    m_resets[reset_i]->specify_reset();
	}
    }

  private:
    sc_spawn_options( const sc_spawn_options& );
    const sc_spawn_options& operator = ( const sc_spawn_options& );

  protected:
    bool                               m_dont_initialize;         
    std::vector<sc_spawn_reset_base*>  m_resets;
    std::vector<const sc_event*>       m_sensitive_events;
    std::vector<sc_event_finder*>      m_sensitive_event_finders; 
    std::vector<sc_interface*>         m_sensitive_interfaces;
    std::vector<sc_port_base*>         m_sensitive_port_bases;
    bool                               m_spawn_method; // Method not thread.
    int                                m_stack_size;   // Thread stack size.
};

} // namespace sc_core

// $Log: sc_spawn_options.h,v $
// Revision 1.11  2011/08/26 20:46:11  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.10  2011/08/24 22:05:51  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.9  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.8  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.7  2011/02/07 19:17:20  acg
//  Andy Goodrich: changes for IEEE 1666 compatibility.
//
// Revision 1.6  2010/12/07 20:09:15  acg
// Andy Goodrich: replaced sc_signal signatures with sc_signal_in_if signatures for reset methods.
//
// Revision 1.5  2010/11/20 17:10:57  acg
//  Andy Goodrich: reset processing changes for new IEEE 1666 standard.
//
// Revision 1.4  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.3  2009/02/28 00:26:58  acg
//  Andy Goodrich: changed boost name space to sc_boost to allow use with
//  full boost library applications.
//
// Revision 1.2  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.4  2006/04/20 17:08:17  acg
//  Andy Goodrich: 3.0 style process changes.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.

#endif // !defined(sc_spawn_options_h_INCLUDED)
