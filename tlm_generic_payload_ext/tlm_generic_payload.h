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


#ifndef _TLM_GENERIC_PAYLOAD_H
#define _TLM_GENERIC_PAYLOAD_H

#include <systemc>
#include "tlm_vector.h"

namespace tlm {

//---------------------------------------------------------------------------
// Helper classes for the extension mechanism
//---------------------------------------------------------------------------
// Registry for extensions:
inline unsigned int max_num_extensions(bool increment=false)
{
    static unsigned int max_num = 0;
    if (increment) ++max_num;
    return max_num;
}

// Base class for extension storage in the extension vector:
class tlm_extension_base
{
public:
    virtual tlm_extension_base* clone() = 0;
    static unsigned int register_extension()
    {
        static int extension_index = -1;
        max_num_extensions(true);
        return (unsigned int)++extension_index;
    };
    virtual ~tlm_extension_base() {}
};

// All extension classes must be derived publicly from this class as
// follows:
// class my_extension : public tlm_extension<my_extension> { ...
// This allows for extension registration during C++ static contruction time.
// We require that an extension class implements the clone() method.
template <typename T>
class tlm_extension : public tlm_extension_base
{
public:
    virtual tlm_extension_base* clone() = 0;
    const static unsigned int ID;
    virtual ~tlm_extension() {}
};

// The actual registration call:
template <typename T>
const
unsigned int tlm_extension<T>::ID = tlm_extension_base::register_extension();

//---------------------------------------------------------------------------
// enumeration types
//---------------------------------------------------------------------------
enum tlm_command {
    TLM_READ_COMMAND,
    TLM_WRITE_COMMAND
};

enum tlm_burst_mode {
    TLM_INCREMENT_BURST,
    TLM_STREAMING_BURST,
    TLM_WRAPPING_BURST
};

enum tlm_response_status {
    TLM_OK_RESP,
    TLM_GENERIC_ERROR_RESP,
    TLM_ADDRESS_ERROR_RESP,
    TLM_COMMAND_ERROR_RESP,
    TLM_BURST_ERROR_RESP,
    TLM_INCOMPLETE_RESP
};

//---------------------------------------------------------------------------
// The generic payload class:
//---------------------------------------------------------------------------
class tlm_generic_payload {
public:

    //---------------
    // Constructors
    //---------------
    
    // Default constructor
    tlm_generic_payload() 
        : m_command(tlm::TLM_READ_COMMAND)
        , m_address(0)
        , m_data(0)
        , m_burst_length(0)
        , m_burst_data_size(0)
        , m_burst_mode(tlm::TLM_INCREMENT_BURST)
        , m_response_status(tlm::TLM_INCOMPLETE_RESP)
        , m_extensions(max_num_extensions())
    {
    }
    
    // FIXME: copy constructor, assignment operator, ... NOT needed
    
    //--------------
    // Destructor
    //--------------
    ~tlm_generic_payload() {}
    
    //----------------
    // Setters & getters
    //---------------
    
    tlm_command         get_command()         const {return m_command;}
    sc_dt::uint64       get_address()         const {return m_address;}
    unsigned int        get_burst_length()    const {return m_burst_length;}
    unsigned int        get_burst_data_size() const {return m_burst_data_size;}
    tlm_burst_mode      get_burst_mode()      const {return m_burst_mode;}
    unsigned char*      get_data_ptr()        const {return m_data;}
    tlm_response_status get_response_status() const {return m_response_status;}
    
    void set_command(const tlm_command command)
        {m_command = command;}
    void set_address(const sc_dt::uint64 address)
        {m_address = address;}
    void set_burst_length(const unsigned int burst_length)
        {m_burst_length = burst_length;}
    void set_burst_data_size(const unsigned int burst_data_size)
        {m_burst_data_size = burst_data_size;}
    void set_burst_mode(const tlm_burst_mode burst_mode)
        {m_burst_mode = burst_mode;}
    void set_data_ptr(unsigned char* data)
        {m_data = data;}
    void set_response_status(const tlm_response_status response_status)
        {m_response_status = response_status;}  
    
    /* --------------------------------------------------------------------- */
    /* Generic Payload attributes:                                           */
    /* --------------------------------------------------------------------- */
    /* - m_command         : Type of transaction. Two values supported:      */
    /*                       - TLM_WRITE_COMMAND                             */
    /*                       - TLM_READ_COMMAND                              */
    /* - m_address         : Transaction base address (byte-addressing).     */
    /* - m_data            : When m_command = TLM_WRITE_COMMAND contains a   */
    /*                       pointer to the data to be written in the target.*/
    /*                       When m_command = TLM_READ_COMMAND contains a    */
    /*                       pointer where to copy the data read from the    */
    /*                       target.                                         */
    /* - m_burst_length    : Number of data transfers in the transaction.    */
    /*			     This is an element count, not a byte count.     */
    /*                       Size in bytes of each data transfer is indicated*/
    /*                       by the burst_data_size attribute.               */
    /* - m_burst_data_size : Size in bytes of each data transfer in the      */
    /*                       burst. It has to be smaller than or equal to    */
    /*                       the data size of the port/socket that initiates */
    /*                       the transaction.                                */
    /* - m_burst_mode	   : Indicates how address has to be incremented for */
    /*                       each data transfer in the burst.                */
    /*                       Three modes supported:                          */
    /*                             - TLM_INCREMENT_BURST                     */
    /*                             - TLM_STREAMING_BURST                     */
    /*                             - TLM_WRAPPING_BURST                      */
    /* - m_response_status : This attribute indicates whether an error has   */
    /*                       occurred during the transaction.                */
    /*                       Values supported are: TBD                       */
    /* --------------------------------------------------------------------- */
    tlm_command          m_command;
    sc_dt::uint64        m_address;			
    unsigned char*       m_data;
    unsigned int         m_burst_length;		
    unsigned int         m_burst_data_size;	
    tlm_burst_mode       m_burst_mode;
    tlm_response_status  m_response_status;

    // unsigned int*        m_byte_enable
    // bool                 m_lock
    // int                  m_identifier
    
public:

    // serialisation helper functions
    // for these, the index is not in bytes but in units of sizeof(T)
    template<class T> inline T read_data(unsigned int index)
    {
        return reinterpret_cast<T*>(m_data)[index];
    }
    
    template<class T> inline void write_data(T data, unsigned int index)
    {
        reinterpret_cast<T*>(m_data)[index] = data;
    }
    
    /* --------------------------------------------------------------------- */
    /* Extension mechanism:                                                  */
    /* --------------------------------------------------------------------- */
    /* The extension mechanism is intended to enable initiator modules to    */
    /* optionally and transparently add data fields to the                   */
    /* tlm_generic_payload. Any target module may check if a specific        */
    /* extension is present, and should respond gracefully if it is present  */
    /* or not. */
    /* This mechanism is mainly intended, but not restricted to,             */
    /* communicate non-protocol relevant data, e.g. debugging information,   */
    /* in a transparent manner over the system.                              */
    /*                                                                       */
    /* The following rules apply:                                            */
    /* - Every extension class must be derived from tlm_extension, like:     */
    /*      class my_extension : public tlm_extension<my_extension> { ...    */
    /* - A tlm_generic_payload object must only be constructed after C++     */
    /*   static initialization time. Only this way we can guarantee that the */
    /*   extension vector is of sufficient size to hold all possible         */
    /*   extensions. This restriction can be dropped if the initiator        */
    /*   module makes sure that the resize_extensions() method is called     */
    /*   once before the first transaction with this payload object is       */
    /*   initiated.                                                          */
    /* --------------------------------------------------------------------- */

    tlm_vector<tlm_extension_base*> m_extensions;

    // Add extension:
    template <typename T> void add_extension(T* ext)
    {
        resize_extensions();
        m_extensions[T::ID] = static_cast<tlm_extension_base*>(ext);
    }

    // Clear extension, the argument is needed to find the right index:
    template <typename T> void clear_extension(T* ext)
    {
        resize_extensions();
        m_extensions[T::ID] = static_cast<tlm_extension_base*>(0);
    }

    // Check for an extension, returns 0 if not present
    template <typename T> void get_extension(T*& ext)
    {
#if 0
        // Enabling this branch would make the use model independend of
        // the contruction restriction. However, as checking for extensions
        // may be done very frequently in the system, we think it's more
        // efficient to require that an initiator always uses a proper
        // payload object either by constructing at a safe time or by calling
        // resize_extensions().
        if(T::ID < m_extensions.size())
        {
            ext = static_cast<T*>(m_extensions[T::ID]);
        }
        else
        {
            ext = 0;
        }
#else
        ext = static_cast<T*>(m_extensions[T::ID]);
#endif
    }

    // Make sure the extension vector is large enough:
    void resize_extensions()
    {
        m_extensions.expand(max_num_extensions());
    }
    
};

} // end namespace
	


#endif
