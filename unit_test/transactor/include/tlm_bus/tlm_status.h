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



#ifndef _TLM_STATUS_H_
#define _TLM_STATUS_H_

  //----------------------------------------------------------------------------
  ///  Class status: status of a the TLM transaction (included in the response)
  /**
    * This class defines the status of an initiator request within the a tlm protocol.
    * \n The status value is set by the target (slave or router) and used by initiators if required
    * \n A status object is a member of the response structure and is passed
    * by value.
    * The status value is set to TLM_ERROR in the constructor.
  **/
  class tlm_status {
 
  public:

    //---------------
    // Constructor
    //---------------  
    /// @name Constructor  
    /// Status is set to "TLM_ERROR" by default
    /// @{
    tlm_status() :
      m_status(TLM_ERROR_FLAG) {
    }
    /// @}

    //------------------------------------------
    // Data member access methods
    //------------------------------------------

    /** @name success flag accessors
     * This flag is reset by the target of a transaction to
     * indicate the success of the transport operation
     * @{
     **/
    inline bool is_ok() const {
      return (!is_error());
    }  
    inline void set_ok() {
      reset_error();
    }
    /* @} */

    /** @name Error flag accessors
     * This flag is set by the target of a transaction to
     * indicate the failure of the transport operation
     * @{
     **/
    inline bool is_error() const {
      return (m_status & TLM_ERROR_FLAG);
    }
    inline void set_error() {
      m_status |= TLM_ERROR_FLAG;
    }
    inline void reset_error() {
      m_status &= ~TLM_ERROR_FLAG;
    }
    /* @} */

    /** @name No response flag accessors
     * This flag is set by the router to
     * indicate a failure of the routing  operation: 
     * no target mapped at the requested address
     * @{
     **/
    inline bool is_no_response() const {
      return (m_status & TLM_NO_RESPONSE_FLAG);
    }
    inline void set_no_response() {
      m_status |= TLM_NO_RESPONSE_FLAG;
    }
    inline void reset_no_response() {
      m_status &= ~TLM_NO_RESPONSE_FLAG;
    }
    /* @} */

  protected :

    /// Status content
    unsigned int m_status;
    
    /// Status codes list
    enum status_list {
      TLM_SUCCESS = 0,     ///< Successful completion
      TLM_ERROR,           ///< Target indicates an error occurs during the access
      TLM_NO_RESPONSE      ///< No target response to the initiator's request

    };
    
    /// Bits flags definitions
    enum status_flags {
      TLM_ERROR_FLAG       = 0x01ul << TLM_ERROR,           ///< Mask to set and reset TLM_ERROR flag
      TLM_NO_RESPONSE_FLAG = 0x01ul << TLM_NO_RESPONSE,     ///< Mask to set and reset TLM_NO_RESPONSE flag
    };

  };

#endif /* _TLM_STATUS_H_ */


