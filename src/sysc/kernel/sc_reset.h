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

  sc_reset.h -- Process reset support.

  Original Author: Andy Goodrich, Forte Design Systems, 17 June 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_reset.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_reset_h_INCLUDED)
#define sc_reset_h_INCLUDED

namespace sc_core {

template<typename DATA> class sc_signal_in_if;
template<typename IF> class sc_signal;
template<typename DATA> class sc_in;
class sc_process_b;

class sc_reset {
    friend class sc_cthread_process;
    friend class sc_method_process; 
    friend class sc_module; 
	friend class sc_process_b;
	friend class sc_signal<bool>;
    friend class sc_simcontext;
    friend class sc_thread_process; 

  protected:
	static void reconcile_resets();
    static void reset_signal_is(const sc_signal_in_if<bool>& iface, bool level);
    static void reset_signal_is(const sc_in<bool>& iface, bool level);

  protected:
    sc_reset( const sc_signal_in_if<bool>* iface_p ) :
        m_iface_p(iface_p) {}
	void notify_processes();
    bool read();
    void remove_process( sc_process_b* );

  protected:
    const sc_signal_in_if<bool>*  m_iface_p;   // Interface to read.
	std::vector<sc_process_b*> m_processes;    // List of processes to reset.

  private: // disabled
    sc_reset( const sc_reset& );
    const sc_reset& operator = ( const sc_reset& );
};



} // namespace sc_core

#endif // !defined(sc_reset_h_INCLUDED)
