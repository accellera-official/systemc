/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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


#ifndef __TLM_GP_H__
#define __TLM_GP_H__

#include <systemc>
#include "tlm_array.h"

namespace tlm {

//---------------------------------------------------------------------------
// Classes and helper functions for the extension mechanism
//---------------------------------------------------------------------------
// Helper function:
inline unsigned int max_num_extensions(bool increment=false)
{
    static unsigned int max_num = 0;
    if (increment) ++max_num;
    return max_num;
}

// This class can be used for storing pointers to the extension classes, used
// in tlm_generic_payload:
class tlm_extension_base
{
public:
    static unsigned int register_extension()
    {
        static int extension_index = -1;
        max_num_extensions(true);
        return (unsigned int)++extension_index;
    };
};

// Base class for all extension classes, derive your extension class in
// the following way:
// class my_extension : public tlm_extension<my_extension> { ...
// This triggers proper extension registration during C++ static
// contruction time. my_extension::ID will hold the unique index in the
// tlm_generic_payload::m_extensions array.
template <typename T>
class tlm_extension : public tlm_extension_base
{
public:
    const static unsigned int ID;
};

template <typename T>
const
unsigned int tlm_extension<T>::ID = tlm_extension_base::register_extension();

//---------------------------------------------------------------------------
// enumeration types
//---------------------------------------------------------------------------
enum tlm_command {
    TLM_READ_COMMAND,
    TLM_WRITE_COMMAND,
    TLM_NOP
};

enum tlm_burst_mode {
    TLM_INCREMENT_BURST,
    TLM_STREAMING_BURST,
    TLM_WRAPPING_BURST
};

enum tlm_response_status {
    TLM_OK_RESP = 1,
    TLM_INCOMPLETE_RESP = 0,
    TLM_GENERIC_ERROR_RESP = -1,
    TLM_ADDRESS_ERROR_RESP = -2,
    TLM_COMMAND_ERROR_RESP = -3,
    TLM_BURST_ERROR_RESP = -4
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
        : m_command(TLM_NOP)
        , m_address(0)
        , m_data(0)
        , m_burst_length(1)
        , m_burst_data_size(0)
        , m_burst_mode(TLM_INCREMENT_BURST)
        , m_response_status(TLM_INCOMPLETE_RESP)
        , m_extensions(max_num_extensions())
        , mDMI(false)
    {
    }
    
    // FIXME: copy constructor, assignment operator, ... NOT needed
    
    //--------------
    // Destructor
    //--------------
    virtual ~tlm_generic_payload() {}
    
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
    bool                get_lock()            const {return m_lock;}
    
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
	void set_lock(const bool lock)
	    {m_lock = lock;}
    
    /* --------------------------------------------------------------------- */
    /* Generic Payload attributes:                                           */
    /* --------------------------------------------------------------------- */
    /* - m_command         : Type of transaction. Three values supported:    */
    /*                       - TLM_WRITE_COMMAND                             */
    /*                       - TLM_READ_COMMAND                              */
    /*                       - TLM_NOP                                       */
    /* - m_address         : Transaction base address (byte-addressing).     */
    /* - m_data            : When m_command = TLM_WRITE_COMMAND contains a   */
    /*                       pointer to the data to be written in the target.*/
    /*                       When m_command = TLM_READ_COMMAND contains a    */
    /*                       pointer where to copy the data read from the    */
    /*                       target.                                         */
    /* - m_burst_length    : Number of data transfers in the transaction.    */
    /*			             This is an element count, not a byte count.     */
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
    bool                 m_lock;

    // unsigned int*        m_byte_enable
    // int                  m_identifier
    
public:

   
    /* --------------------------------------------------------------------- */
    /* Dynamic extension mechanism:                                          */
    /* --------------------------------------------------------------------- */
    /* The extension mechanism is intended to enable initiator modules to    */
    /* optionally and transparently add data fields to the                   */
    /* tlm_generic_payload. Target modules are free to check for extensions  */
    /* and may or may not react to the data in the extension fields. The     */
    /* definition of the extensions' semantics is solely in the              */
    /* responsibility of the user.                                           */
    /*                                                                       */
    /* The following rules apply:                                            */
    /*                                                                       */
    /* - Every extension class must be derived from tlm_extension, e.g.:     */
    /*     class my_extension : public tlm_extension<my_extension> { ... }   */
    /*                                                                       */
    /* - A tlm_generic_payload object should be constructed after C++        */
    /*   static initialization time. This way it is guaranteed that the      */
    /*   extension array is of sufficient size to hold all possible          */
    /*   extensions. Alternatively, the initiator module can enforce a valid */
    /*   extension array size by calling the resize_extensions() method      */
    /*   once before the first transaction with the payload object is        */
    /*   initiated.                                                          */
    /*                                                                       */
    /* - Initiators should use the the set_extension(e) or clear_extension(e)*/
    /*   methods for manipulating the extension array. The type of the       */
    /*   argument must be a pointer to the specific registered extension     */
    /*   type (my_extension in the above example) and is used to             */
    /*   automatically locate the appropriate index in the array.            */
    /*                                                                       */
    /* - Targets can check for a specific extension by calling               */
    /*   get_extension(e). e will point to zero if the extension is not      */
    /*   present.                                                            */
    /*                                                                       */
    /* --------------------------------------------------------------------- */

    // Stick the pointer to an extension into the vector:
    template <typename T> void set_extension(T* ext)
    {
        resize_extensions();
        m_extensions[T::ID] = static_cast<tlm_extension_base*>(ext);
    }
    // non-templatized version with manual index:
    void set_extension(unsigned int index, tlm_extension_base* ext)
    {
        resize_extensions();
        m_extensions[index] = ext;
    }

    // Check for an extension, ext will point to 0 if not present
    template <typename T> void get_extension(T*& ext)
    {
        ext = static_cast<T*>(m_extensions[T::ID]);
    }
    // Non-templatized version:
     tlm_extension_base* get_extension(unsigned int index)
    {
        return m_extensions[index];
    }

    // Clear extension, the argument is needed to find the right index:
    template <typename T> void clear_extension(const T* ext)
    {
        resize_extensions();
        m_extensions[T::ID] = static_cast<tlm_extension_base*>(0);
    }
    // Non-templatized version with manual index
    void clear_extension(unsigned int index)
    {
        if (index < m_extensions.size())
        {
            m_extensions[index] = static_cast<tlm_extension_base*>(0);
        }
    }

    // Make sure the extension array is large enough. Can be called once by
    // an initiator module (before issuing the first transaction) to make
    // sure that the extension array is of correct size. This is only needed
    // if the initiator cannot guarantee that the generic payload object is
    // allocated after C++ static construction time.
    void resize_extensions()
    {
        m_extensions.expand(max_num_extensions());
    }

protected:
    tlm_array<tlm_extension_base*> m_extensions;

public:
  // This is the "DMI-hint" a slave can set this to true if it
  // wants to indicate that a DMI request would be supported:
  void set_dmi_allowed(bool dmiAllowed) { mDMI = dmiAllowed; }
  bool get_dmi_allowed() const { return mDMI; }

protected:
  bool mDMI;
};

} // namespace tlm

#endif /* __TLM_GP_H__ */
