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

#ifndef _TLM_INITIATOR_PORT_H_
#define _TLM_INITIATOR_PORT_H_

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include <vector>

#include "systemc.h"

#include "tlm_ports/tlm_initiator_port_base.h"
#include "tlm_ports/tlm_target_port.h"


//----------------------------------------------------------------------------
/// Class tlm_initiator_port: port to be instantiated on the initiator side
/**
 * This class could be used as base class for user defined initiator ports (support of user convenience layer).
 * \n It provides new binding methods definition: 
 *  - sc_port to sc_export binding must be identified in order to create target_port_list
 *    used for port ID propagation through hierarchical binding. 
 *  - sc_port to sc_port binding creates two initiator ports lists: one regular list (deep port "knows" other
 *     initiator ports through a hierarchy) and a reversed list (the port directly bound to the interface
 *     "knows" deeper ports through a hierarchy) used to convey target_port list to deeper ports for target port
 *     ID propagation.
 * An error detection is also provided: When the same interface is bound two times to a sc_port, SystemC causes 
 * the simulation end. This case could append in test platform (dedicated debug and regular port of a target 
 * bound to the same router). The specific binding issues a warning message but let the simulation run.

**/
//----------------------------------------------------------------------------
template<typename IF,int N = 1>
class tlm_initiator_port : 
  public sc_port<IF,N>,
  public tlm_initiator_port_base {

  typedef IF interface_type;
  typedef tlm_target_port<IF> target_port_type;
  typedef tlm_initiator_port<IF,N> initiator_port_type;

  typedef tlm_target_port_base target_port_base_type;
  typedef tlm_initiator_port_base initiator_port_base_type;

public:

  //--------------
  // Constructor
  //--------------
  tlm_initiator_port(const char * name);
    

  //--------------------------------------------------------------------
  /// @name  sc_port overridden methods
  /// Need to specialize binding operation to support port propagation in transaction recorders. 
  /// Specialization is done for sc_port/sc_port and sc_port/sc_export 
  /// Other redefinitions of bindings are just calls to regular bind function
  /// \n Note: This is due to non virtual definition of SystemC bind method
  //--------------------------------------------------------------------
  /// @{
  ///
  //-------------------------------------------------------------------------------
  // Before end of elaboration
  /**
   * Called just before end of elaboration to propagate registered target ports to all the 
   * bound initiator ports (using the reversed initiator port list).
   *
   * Remark: sc_port_base::complete_binding should be the best place to do 
   *         that but this method is private !!
   **/
  //-------------------------------------------------------------------------------
  void before_end_of_elaboration();

  //-------------------------------------------------------------------------------
  // End of elaboration
  /**
   * Called at the end of elaboration. Checks target port list: empty target port
   * list is a fatal error as initiator port should be bound at least to one target_port
   **/
  //-------------------------------------------------------------------------------
  void end_of_elaboration();
    
    
  //--------------------------------------------------------------------
  // Special binding between initiator port and target port.
  // (port registration added to the standard sc_port_base::bind())
  //--------------------------------------------------------------------
    
  /// bind(sc_export) overridden with port registration 
  void bind(target_port_type& target_port_);

  /// Case of the other port uses another interface as template: works only if OTHER_IF derives from IF
  template<typename OTHER_IF>
  void bind(tlm_target_port<OTHER_IF>& target_port_);


  /// operator() (sc_export) overridden with port registration 
  void operator() (target_port_type& target_port_);

  /// Case of the other port uses another interface as template: works only if OTHER_IF derives from IF
  template<typename OTHER_IF>
  void operator() (tlm_target_port<OTHER_IF>& target_port_) {
    bind<OTHER_IF>(target_port_);
  }


  /// bind(sc_port) overridden with port registration 
  void bind(sc_port_b<interface_type>& port_);
  
  /// operator() (sc_port) overridden with port registration 
  void operator() (sc_port_b<interface_type>& port_);
    
    
  //---------------------------------------------------------
  // Normal binding between port and interface.
  // To be redefined here (bind(port_) is overridden above
  //---------------------------------------------------------
    
  /// bind(interface_type) overridden with port registration 
  void bind(interface_type& interface_);

  /// operator () (interface_type) overridden with port registration
  void operator () (interface_type& interface_);
    
  /// @}

protected:

/** Returns true and issues an error message and return true if the same interface is bound 
    two times throught 2 differents target port. 
**/
  bool is_interface_bound_twice(sc_export_base& target_port_);

};



//--------------
// Constructor
//--------------
template<typename IF,int N>
tlm_initiator_port<IF,N>::tlm_initiator_port(const char * name) :
  sc_port<interface_type,N>(name)
{}

  
/** Returns true and issues an error message if the same interface is bound 
    two times throught 2 differents target port. 
**/
template<typename IF,int N>
bool tlm_initiator_port<IF,N>::is_interface_bound_twice(sc_export_base& target_port_) {
  for(int i=0;i<this->size();i++) {
    if ((*this)[i] == target_port_.get_interface()) {
      sc_object * tmp = dynamic_cast<sc_object * >(target_port_.get_interface());
      std::string if_name;
      if (tmp) if_name = tmp->name();
      else if_name = "unnamed interface/non sc_object";

      std::string msg(sc_object::name());
      msg += (std::string)(": tlm_initiator_port warning, while binding the interface \"");
      msg += if_name + (std::string)("\" exported by target port ");
      msg += (std::string)(target_port_.name()) + (std::string)(": the initiator port is already bound to this interface\n");
      SC_REPORT_WARNING("binding warning",msg.c_str());
	
      return(true);
    }
  }
  return(false);
}

//---------------------------
// sc_port overridden methods
//---------------------------
  
//-------------------------------------------------------------------------------
// before_end_of_elaboration()
// Called just before end of elaboration to propagate registered target ports to all the 
// bound initiator ports
//-------------------------------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::before_end_of_elaboration()
{

  // Registered target port list propagation (complete sharing of target_port_list between all initiator ports)
  // If current port's target port list exist, share the list with all other port of the chain using the 
  // reversed initiator port list.
  if (m_target_port_list.size()) {
    for(typename std::vector<initiator_port_base_type *>::iterator port = get_reversed_initiator_port_list().begin();
	port != get_reversed_initiator_port_list().end();
	port++) {		 

      // Copy the target port(s) to the target port list of the registered initiator (port info. propagation)
      if ((*port) != this) {
	for(typename std::vector<target_port_base_type *>::iterator target_port = m_target_port_list.begin();
	    target_port != m_target_port_list.end();
	    target_port++) {
	  (*port)->register_target_port((*target_port));
	}
      }
    }
  }

  // Recreate the complete list of registered initiator_port, from deeper port to higher port (hierarchically speaking)
  // Objective: deeper port is able to "see" all others but intermediate port cannot "see" deeper ports.
  std::vector<initiator_port_base_type *> tmp_initiator_port_list; 
  tmp_initiator_port_list.push_back(this);
  this->create_port_list(tmp_initiator_port_list);
  // replace the regular initiator port list: this list is now complete
  this->set_initiator_port_list(tmp_initiator_port_list);
}

//-------------------------------------------------------------------------------
// end_of_elaboration()
// Check the target port list: if empty, stops the simulation as initiator port should be bound at 
// least to one target_port
//-------------------------------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::end_of_elaboration() {

#ifdef TLM_PORT_DEBUG
  printf("DEBUG\t\t%s: Registered initiator port list :\n",sc_object::name());
  for(typename std::vector<initiator_port_base_type *>::iterator port = get_initiator_port_list().begin();
      port != get_initiator_port_list().end();
      port++) {
    printf("DEBUG\t\t%s: \t- %s\n",sc_object::name(),(static_cast<initiator_port_type * >(*port))->name());
      
  }
  printf("DEBUG\t\t%s: Registered initiator reversed port list :\n",sc_object::name());
  for(typename std::vector<initiator_port_base_type *>::iterator port = get_reversed_initiator_port_list().begin();
      port != get_reversed_initiator_port_list().end();
      port++) {
    printf("DEBUG\t\t%s: \t- %s\n",sc_object::name(),(static_cast<initiator_port_type * >(*port))->name());
      
  }

  printf("DEBUG\t\t%s: Registered target port list:\n",sc_object::name());
  for(typename std::vector<target_port_base_type *>::iterator port = get_target_port_list().begin();
      port != get_target_port_list().end();
      port++) {
    printf("DEBUG\t\t%s: \t- %s\n",sc_object::name(),(static_cast<target_port_type * >(*port))->name());
      
  }

  printf("DEBUG\t\t%s: -------------------------------------------------------\n",sc_object::name());

#endif
  // Checks the target port list
  if ((!get_target_port_list().size()) && this->size()) {
    std::string msg(sc_object::name());
    msg += (std::string)(": tlm_initiator_port error, target port list is empty. Initiator port should be bound at least to one target port\n");
    SC_REPORT_ERROR("port end of elaboration",msg.c_str());
  }
}

//--------------------------------------------------------------------
// Special binding between initiator port and target port.
// (port registration added to the standard sc_port_base::bind())
//--------------------------------------------------------------------

//---------------------------------------------------------
// bind(sc_export)
//---------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::bind(target_port_type& target_port_) {
  if (!is_interface_bound_twice(target_port_))
    // Registers the target port
    register_target_port(&target_port_);
    
  // Calls sc_port standard bind() method 
  sc_port_b<interface_type>::bind(target_port_);
}


template<typename IF,int N>
template<typename OTHER_IF>
void 
tlm_initiator_port<IF,N>::bind(tlm_target_port<OTHER_IF>& target_port_) {
  // Test if OTHER_IF derives from IF
  IF * other_if = dynamic_cast<IF*>(target_port_.get_interface());
  if (other_if) {

    if (!is_interface_bound_twice(target_port_))      
      // Registers the target port
      this->register_target_port(&target_port_);

    // Calls sc_port standard bind() method 
    sc_port_b<interface_type>::bind(*other_if);
  }
  else {	
    std::string msg(sc_object::name());
    msg += (std::string)(": tlm_initiator_port error, incompatible interface detected during the binding tlm_initiator_port  \"") + (std::string)(this->name());
    msg += (std::string)" to tlm_target_port \"" + (std::string)(target_port_.name()) +(std::string)("\"\n") ;
    SC_REPORT_ERROR("bind export to export failed",msg.c_str());
  }
}

  

//---------------------------------------------------------
// operator() (sc_export): just calls bind(sc_export)
//---------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::operator() (target_port_type& target_port_) {
  bind(target_port_);
}

//---------------------------------------------------------
// bind(sc_port) 
//---------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::bind(sc_port_b<interface_type>& port_) {
  // Register the bound port in the regular list
  this->register_initiator_port(static_cast<initiator_port_type *>(&port_));

  // Bound port registers the current port. Used to share the target port list between all port before the end of elaboration
  for(typename std::vector<initiator_port_base_type *>::iterator initiator_port = get_reversed_initiator_port_list().begin();
      initiator_port != get_reversed_initiator_port_list().end();
      initiator_port++) {
    static_cast<initiator_port_type *>(&port_)->reversed_register_initiator_port(*initiator_port);
  }

  // Calls sc_port standard bind() method 
  sc_port_b<interface_type>::bind(port_);
}
  
//---------------------------------------------------------
// operator() (sc_port): just calls bind(sc_port)
//---------------------------------------------------------
template<typename IF,int N>
void  
tlm_initiator_port<IF,N>::operator() (sc_port_b<interface_type>& port_) {
  bind(port_);
}

//--------------------------------------------------------------------
// Normal binding between port and interface
//  To be redefined here (bind(port_) is overridden above
//--------------------------------------------------------------------

//---------------------------------------------------------
// bind(interface_type) 
//---------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::bind(interface_type& interface_) {
  // Calls sc_port standard bind method
  sc_port_b<interface_type>::bind(interface_);    
}

//---------------------------------------------------------
// operator() (interface_type) 
//---------------------------------------------------------
template<typename IF,int N>
void 
tlm_initiator_port<IF,N>::operator() (interface_type& interface_) {
  sc_port_b<interface_type>::bind(interface_); 
}

#endif /* _TLM_INITIATOR_PORT_H_ */
