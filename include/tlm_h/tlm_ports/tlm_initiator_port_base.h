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


#ifndef _TLM_INITIATOR_PORT_BASE_H_
#define _TLM_INITIATOR_PORT_BASE_H_


/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include <vector>

#include "systemc.h"

#include "tlm_ports/tlm_target_port_base.h"


  //----------------------------------------------------------------------------
  /// Class tlm_initiator_port_base: This class is a base class for tlm_initiator port.
  /**
   * \n It provides a container to store registered target ports as pointer list (required for target port ID propagation and
   *    transaction recording).
   * \n It provides also two containers to store registered initiator ports as pointer list:
   *  - The first container registers initiator port from "deeper" initiator port to the the initiator
   *    port directly bound to the interface. 
   *  - The second list, named reversed list, allows the port directly bound to the interface to propagate its target
   *    port list to "deeper" initiator ports. 
   *
   * \n Accessors are provided for all the port lists.
   **/
  //----------------------------------------------------------------------------
  class tlm_initiator_port_base {

    typedef tlm_initiator_port_base this_type;
    typedef tlm_target_port_base target_port_type;
    
  public:
    
    //--------------
    // Constructor
    //--------------
    tlm_initiator_port_base() {
      // Adds the port itself to the reversed initiator port list
      m_reversed_initiator_port_list.push_back(this);
    }


    //------------------------------------------
    /// @name  Data members access methods
    //------------------------------------------
    /// @{
    
    /// Register the target port
    inline void register_target_port(target_port_type * target_port) {
      m_target_port_list.push_back(target_port);
    }

    /// Returns the vector of registered target port(s) 
    inline std::vector<target_port_type *>& get_target_port_list() {
      return(m_target_port_list);
    }
    
    /// Sets the vector of registered target port(s) (port propagation)
    inline void set_target_port_list(std::vector<target_port_type *>& target_port_list) {
      m_target_port_list = target_port_list;
    }

    //---------------------------------------------------------------------------

    /// Register the initiator port in the regular list
    inline void register_initiator_port(this_type * initiator_port) {
      m_initiator_port_list.push_back(initiator_port);
    }

    /// Returns the vector of registered initiator port(s) 
    inline std::vector<this_type *>& get_initiator_port_list() {
      return(m_initiator_port_list);
    }

    /// Sets the vector of registered initiator port(s) (port propagation)
    inline void set_initiator_port_list(std::vector<this_type *>& initiator_port_list) {
      m_initiator_port_list = initiator_port_list;
    }

    //---------------------------------------------------------------------------

    /// Register the initiator port in the reversed list
    inline void reversed_register_initiator_port(this_type * initiator_port) {
      m_reversed_initiator_port_list.push_back(initiator_port);
    }

    /// Returns the vector of registered initiator port(s), reversed list 
    inline std::vector<this_type *>& get_reversed_initiator_port_list() {
      return(m_reversed_initiator_port_list);
    }

    /// Sets the vector of registered initiator port(s) (port propagation) reversed list
    inline void set_reversed_initiator_port_list(std::vector<this_type *>& initiator_port_list) {
      m_reversed_initiator_port_list = initiator_port_list;
    }

    /// @}


  protected:

    /// sc_export objects registration 
    /**
     * Direct registration of the bound target port(s) into current initiator port 
     **/
    std::vector<target_port_type *> m_target_port_list;
    
    
    /// sc_port objects registration 
    /**
     * Direct registration of the bound initiator port into current initiator port
     **/
    std::vector<this_type *> m_initiator_port_list;

    /// sc_port objects registration 
    /**
     * Direct registration of the current initiator port into the bound initiator port for 
     * port propagation: all bound initiator ports get back the same target_port_list
     **/
    std::vector<this_type *> m_reversed_initiator_port_list;

    /// Creates the initiator port list from a non NULL initiator_port_type * list
    /** Reentreant function to create the complete list of bound initiator_port 
     **/
    void create_port_list(std::vector<this_type *>& port_list) {
      assert(port_list.size() > 0);      
      this_type& current_port = *port_list[(port_list.size()-1)];    
      // For all the registed initiator port of the port
      for (int i=0;i<(int)current_port.get_initiator_port_list().size();i++) {
	// register only other ports (the list contains the port itself)
	if (static_cast<this_type *>(&current_port) != current_port.get_initiator_port_list()[i]) {
	  port_list.push_back(current_port.get_initiator_port_list()[i]); // register port
	  this->create_port_list(port_list); // find again in the updated port list
	}
      }
    }


  };

#endif /* _TLM_INITIATOR_PORT_BASE_H_ */


