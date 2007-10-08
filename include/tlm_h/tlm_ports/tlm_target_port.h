/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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

#ifndef _TLM_TARGET_PORT_H_
#define _TLM_TARGET_PORT_H_

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include <vector>

#include "systemc.h"

#include "tlm_ports/tlm_target_port_base.h"

//----------------------------------------------------------------------------
/// Class tlm_target_port: port to be instantiated on the target side
/**
 * This class could be used as base class for user defined target ports (support of user convenience layer).
 * \n It provides new binding methods definition: sc_export to sc_export binding must be identified in order
 * create tlm_target_port_base::target_port_list used target port ID propagation through hierarchical 
 * binding; the only port ID that makes sense is the port ID of the port directly bound to the interface.
 * An error detection is also provided: multiple interface bind declarations are not detected by 
 * sc_export (the interface is just replaced), the new binding definition issues an error message in this
 * case.
 **/
//----------------------------------------------------------------------------
template<typename IF>
class tlm_target_port : 
  public sc_export<IF>,
  public tlm_target_port_base {

  typedef IF interface_type;
  typedef tlm_target_port<IF> target_port_type;
  typedef tlm_target_port_base target_port_base_type;
    
public:
    
  //--------------
  // Constructor
  //--------------
  tlm_target_port(const char * name);
        
  //--------------------------------------------------------------------
  /// @name  sc_export overridden methods
  /// Need to specialize binding operation to support port propagation in transaction recorders. 
  /// Specialization is done for sc_export/sc_export.
  /// Other redefinitions of bindings are just calls to regular bind function
  /// \n Note: This is due to non virtual definition of SystemC bind 
  //--------------------------------------------------------------------
  /// @{
  ///
  //-------------------------------------------------------------------------------
  // Before end of elaboration
  /**
   * Called just before end of elaboration to propagate the significant target port id:
   * the only valid port id is the one set by the module which implements the tlm core interface.
   *
   * Remark: sc_port_base::complete_binding should be the best place to do 
   *         that but this method is private !!
   **/
  //-------------------------------------------------------------------------------
  void before_end_of_elaboration();

  //-------------------------------------------------------------------------------
  // End of elaboration
  /**
   * Called at the end of elaboration. Used to print debug message: content opf the target port list
   **/
  //-------------------------------------------------------------------------------
  void end_of_elaboration();

  //---------------------------------------------------------
  // Special binding between two target ports 
  //---------------------------------------------------------
    
  /// bind(sc_export) with target ports propagation
  void bind(target_port_type& target_port_);

  /// Case of the other port uses another interface as template: works only if OTHER_IF derives from IF
  template<typename OTHER_IF>
  void bind(tlm_target_port<OTHER_IF>& target_port_);

  /// operator() (sc_export) overridden: just calls bind(sc_export)
  void operator() (target_port_type& target_port_) {
    bind(target_port_);
  }

  /// Case of the other port uses another interface as template: works only if OTHER_IF derives from IF
  template<typename OTHER_IF>
  void operator() (tlm_target_port<OTHER_IF>& target_port_) {
    bind<OTHER_IF>(target_port_);
  }
    
    
  //---------------------------------------------------------
  // Normal binding between port and interface.
  // To be redefined here (bind(port_) is overridden above
  //---------------------------------------------------------
    
  /// bind(interface_type) overridden
  void bind(interface_type& interface_);
    
  /// operator() (interface_type) overridden
  void operator() (interface_type& interface_);
    
  /// @}   

protected:

/**  Returns true and issues an error message if the port is already bound to an interface during the 
     binding of other_if (called by bind(sc_export))
**/
  bool is_already_bound(sc_interface * other_if);

};


//--------------
// Constructor
//--------------

template<typename IF>
tlm_target_port<IF>::tlm_target_port(const char * name) :
  sc_export<interface_type>(name)
{}



/* Return true and issue an error message if the port is already bound to an interface during the 
   binding of other_if (called by bind(sc_export))
*/
template<typename IF>
bool tlm_target_port<IF>::is_already_bound(sc_interface * other_if) {
  if (dynamic_cast<sc_interface * >(this->get_interface()) == NULL) return(false);
  else {
    std::string if_name1,if_name2 ;
    sc_object * tmp = dynamic_cast<sc_object * >(other_if);
    if (tmp) if_name1 = tmp->name();
    else if_name1 = "unnamed interface (non sc_object)";
    tmp = dynamic_cast<sc_object * >(this->get_interface());
    if (tmp) if_name2 = tmp->name();
    else if_name2 = "unnamed interface (non sc_object)";

    std::string msg(sc_object::name());
    msg += (std::string)(": tlm_target_port warning, Can't bind to \"") + if_name1;
    msg += (std::string)("\" interface,the target port is already bound to this interface: \"");
    msg += if_name2 + (std::string)("\"\n");
    SC_REPORT_WARNING("binding warning",msg.c_str());

    return(true);
  }
}

//---------------------------
// sc_export overridden methods
//---------------------------
  
//-------------------------------------------------------------------------------
// before_end_of_elaboration()
// Called just before end of elaboration to propagate the significant target port id 
// The only significant target port id is the one set by the module which implements the 
// tlm core interface
//-------------------------------------------------------------------------------
template<typename IF>
void 
tlm_target_port<IF>::before_end_of_elaboration()
{
  // if this target port is not directly connected to the core interface
  if (get_target_port_list().size() > 1) {

    for(typename std::vector<target_port_base_type *>::iterator target_port = get_target_port_list().begin();
	target_port != get_target_port_list().end();
	target_port++) {		 	  
      set_tlm_export_id((*target_port)->get_tlm_export_id());	// Propagate the target port id
    }
  }
}

//-------------------------------------------------------------------------------
// end_of_elaboration(). Debug message concerning target port list centent
//-------------------------------------------------------------------------------
template<typename IF>
void 
tlm_target_port<IF>::end_of_elaboration() {

#ifdef TLM_PORT_DEBUG
  printf("DEBUG\t\t%s: Registered target port list :\n",sc_object::name());
  for(typename std::vector<target_port_base_type *>::iterator port = get_target_port_list().begin();
      port != get_target_port_list().end();
      port++) {
    printf("DEBUG\t\t%s: \t- %s\n",sc_object::name(),(static_cast<target_port_type * >(*port))->name());      
  }
  printf("DEBUG\t\t%s: -------------------------------------------------------\n",sc_object::name());
#endif
    
}
//---------------------------------------------------------
// Special binding between two target ports 
//---------------------------------------------------------

// bind(sc_export)
template<typename IF>
void 
tlm_target_port<IF>::bind(target_port_type& target_port_)
{
  // If the target port is still not bound
  if (!is_already_bound(target_port_.get_interface())) {
    // Copy the list of registered target ports to the current bound port (port propagation)
    for(typename std::vector<target_port_base_type *>::iterator target_port = target_port_.get_target_port_list().begin();
	target_port != target_port_.get_target_port_list().end();
	target_port++) {
      this->register_target_port(*target_port);
    }
      
    // Calls sc_export standard bind method
    sc_export<interface_type>::bind(target_port_);
  }
}

// Specific case: if the bound export is based on a different interface. Requirement, the interface of 
// the bound export MUST derive from the interface the export is based on.
template<typename IF>
template<typename OTHER_IF>
void 
tlm_target_port<IF>::bind(tlm_target_port<OTHER_IF>& target_port_) {
  // Test if OTHER_IF derives from IF
  IF * other_if = dynamic_cast<IF*>(target_port_.get_interface());
  if (other_if) {
    // If the target port is still not bound
    if (!is_already_bound(target_port_.get_interface())) {
      // Copy the list of registered target ports to the current bound port (port propagation)
      for(typename std::vector<target_port_base_type *>::iterator target_port = target_port_.get_target_port_list().begin();
	  target_port != target_port_.get_target_port_list().end();
	  target_port++) { 
	this->register_target_port(*target_port);
      }
	
      // Calls sc_export standard bind method 
      sc_export<interface_type>::bind(*other_if);
    }
  }
  else {
    std::string msg(sc_object::name());
    msg += (std::string)(": tlm_target_port error, incompatible interface detected during the binding tlm_target_port  \"") + (std::string)(this->name());
    msg += (std::string)" to tlm_target_port \"" + (std::string)(target_port_.name()) +(std::string)("\"\n") ;
    SC_REPORT_ERROR("bind export to export failed",msg.c_str());
  }

}

  
//---------------------------------------------------------
// Normal binding between port and interface.
// To be redefined here (bind(port_) is overridden above
//---------------------------------------------------------
  
// bind(interface_type) 
template<typename IF>
void 
tlm_target_port<IF>::bind(interface_type& interface_) {
  // Calls sc_export standard bind method
  sc_export<interface_type>::bind(interface_);
}
  
// operator() (interface_type): just calls bind(interface_type)
template<typename IF>
void 
tlm_target_port<IF>::operator() (interface_type& interface_) {
  sc_export<interface_type>::bind(interface_); 
}

#endif /* _TLM_TARGET_PORT_H_ */
