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


#ifndef _TLM_REQUEST_H_
#define _TLM_REQUEST_H_

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "tlm_byte_enable.h"
#include "tlm_custom_base.h"


/*------------------------------------------------------------------------------
 * Constants
 *----------------------------------------------------------------------------*/
/** \defgroup request_command Initiator request command
 *  Initiator request command type or request opcode 
 * @{
 **/
enum tlm_command {
  READ,            ///< Command to issue a read operation
  WRITE            ///< Command to issue a write operation
};
/* @} */
  

/** \defgroup request_mode Initiator request command mode
 * Initiator request command mode 
 * @{
 **/
enum tlm_mode {
  REGULAR,      ///< Regular access
  DEBUG,        /*!< Debug access, all wait() statements must be by-passed and side effects 
		 *   in target implementation (clear register on read process activation) should not occur
		 **/
  CONTROL       ///< Indicates simulation control transaction

};

/* @} */


/** \defgroup request_block_mode Initiator block request mode
 * Initiator block request mode 
 * @{
 **/
enum tlm_block_mode {
  INCREMENT,         ///< The address is incremented for each chunk of data. The increment value depend on request parameter "block_address_incr"
  STREAMING,         ///< The address is always the same
  WRAP               ///< wraps at word boundaries
};

//
// For example, assuming byte addressing and 32 bit words :
// address = 8, block_size = 4, block_mode = INCREMENT, block_address_incr = 4 would address 0x8, 0xc, 0x10, 0x14
// address = 8, block_size = 4, block_mode = WRAP, block_address_incr = 4 would address 0x8, 0xc, 0x0, 0x4 because
// there is a word boundary at 0x10.
//
/* @} */

  

//----------------------------------------------------------------------------
///  Class tlm_request: information sent by the initiator module to the target module
/**
 * It describes the information sent by the initiator module to the target module.

**/
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE = TLM_PASS_BY_COPY>
class tlm_request {
    
public:
       
  //----------------
  // Constructors
  //---------------

  /// Default constructor
  tlm_request();

  /// Copy constructor
  tlm_request(const tlm_request<ADDRESS,DATA,DATA_MODE>& request);

  //--------------
  // Destructor
  //--------------
  ~tlm_request();    

  //----------------------
  // Cross Mode Assignment operator
  //----------------------
  template< tlm_data_mode mode >
  tlm_request& operator=(const tlm_request<ADDRESS,DATA,mode>& request);

  //----------------------
  // Normal Assignment operator
  //----------------------
  tlm_request& operator=(const tlm_request<ADDRESS,DATA,DATA_MODE>& request );

  //----------------------
  // Copy from other request object
  //----------------------  
  template< tlm_data_mode from_mode >
  inline void copy( const tlm_request<ADDRESS,DATA,from_mode>& request);

  //----------------------
  // copy array (data, byte_enable, custom) from other request object
  //----------------------  
  template< tlm_data_mode from_mode >
  inline void copy_arrays( const tlm_request<ADDRESS,DATA,from_mode>& request);

  //------------------------------------------
  //
  // @name Data members command methods
  /// @{
  inline tlm_command get_command() const;
  inline void set_command(const tlm_command command);

  inline tlm_mode get_mode() const;
  inline void set_mode(const tlm_mode mode);

  inline bool is_regular_access() const;
  inline bool is_debug_access() const;
  inline bool is_timed_access() const;
  inline bool is_control_access() const;

  inline ADDRESS get_address() const;
  inline void set_address(const ADDRESS address);

  inline const DATA& get_data(const unsigned int index = 0) const;
  inline void set_data(const DATA& data,const unsigned int index = 0);
  
  inline const DATA * get_data_ptr() const;
  inline void set_data_array(const unsigned int size,const DATA * data_ptr = NULL);

  inline unsigned int get_block_size() const;
  inline void set_block_size(const unsigned int block_size);

  inline unsigned int get_byte_enable(const unsigned int index = 0) const;
  inline void set_byte_enable(const unsigned int byte_enable,const unsigned int index = 0);

  inline const unsigned int * get_byte_enable_ptr() const;
  inline void set_byte_enable_array(const unsigned int size,const unsigned int * byte_enable_ptr = NULL);

  inline unsigned int get_byte_enable_period() const;
  inline void set_byte_enable_period(const unsigned int byte_enable_period);

  inline tlm_block_mode get_block_mode() const;
  inline void set_block_mode(tlm_block_mode block_mode);

  inline unsigned int get_block_address_incr() const;
  inline void set_block_address_incr(unsigned int block_address_incr);

  inline unsigned int get_master_thread_id() const;
  inline void set_master_thread_id(unsigned int master_thread_id);

  inline unsigned int get_priority() const;
  inline void set_priority(unsigned int priority);

  inline unsigned int get_transaction_id() const;
  inline void set_transaction_id(unsigned int transaction_id);

  inline unsigned int get_tlm_export_id() const;
  inline void set_tlm_export_id(unsigned int tlm_export_id);

  template<typename T >
  inline bool get_custom_ptr( T *& t ) const;
  inline std::vector<tlm_custom_base *> *get_custom_vector_ptr() const;
  inline void set_custom_vector_ptr( std::vector<tlm_custom_base *> *t );


  /// @}


protected:
  tlm_command m_command;                     //!< Command type (READ,WRITE)
  tlm_mode m_mode;                           //!< Request access mode (REGULAR,DEBUG and CONTROL)
  ADDRESS m_address;                         //!< Request address  
  DATA * m_data;                             //!< Request data pointer
  unsigned int m_block_size;                 //!< Determines how many chunks of data are transferred (size of block)
  const unsigned int * m_byte_enable;        //!< Byte enable array pointer (no byte_enable if NULL)
  unsigned int m_byte_enable_period;         /*!< Byte enable period: in case of block transfer (m_block_size != 1) 
					      *   with byte enable, size of byte enable array if not null.
					      **/
  tlm_block_mode m_block_mode;               //!< Specifies address behavior in case of block tranfer (INCREMENT,STREAMING,WRAPP)
  unsigned int m_block_address_incr;         //!< In case of INCREMENT block mode, value of the address increment
  unsigned int m_priority;                   //!< Priority of the request
  unsigned int m_master_thread_id;           //!< Master module thread ID 
  unsigned int m_transaction_id;             //!< Transaction ID 
  unsigned int m_tlm_export_id;              //!< export id (used when several exports are used with single interface)
  std::vector<tlm_custom_base *> *m_custom_vector_ptr;   //!< Pointer to vector of custom extensions

private:
  //------------------------------------------
  /// Data copy function. 
  static inline void data_copy(DATA * dst,const DATA * src,const unsigned int size); 

};




//----------------
// Constructors
//---------------

// Default constructor
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
tlm_request<ADDRESS,DATA,DATA_MODE>::tlm_request() : 
  m_command(READ),
  m_mode(REGULAR),
  m_address(0),
  m_data(NULL),
  m_block_size(1),
  m_byte_enable(NULL),
  m_byte_enable_period(0),
  m_block_mode(INCREMENT),
  m_block_address_incr(0),
  m_master_thread_id(0),
  m_transaction_id(0) ,
  m_tlm_export_id( 0 ) ,
  m_custom_vector_ptr( 0 )
{}


// Copy constructor
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
tlm_request<ADDRESS,DATA,DATA_MODE>::tlm_request(const tlm_request<ADDRESS,DATA,DATA_MODE>& request) :
  m_command(request.m_command),
  m_mode(request.m_mode),
  m_address(request.m_address),
  m_data(0),
  m_block_size(request.m_block_size),
  m_byte_enable(0),
  m_byte_enable_period(request.m_byte_enable_period),
  m_block_mode(request.m_block_mode),
  m_block_address_incr(request.m_block_address_incr),
  m_priority(request.m_priority),
  m_master_thread_id(request.m_master_thread_id),
  m_transaction_id(request.m_transaction_id),
  m_tlm_export_id( request.m_tlm_export_id ),
  m_custom_vector_ptr( 0 )
{
  copy_arrays( request );
}


//--------------
// Destructor
//--------------
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
tlm_request<ADDRESS,DATA,DATA_MODE>::~tlm_request() {

  if (DATA_MODE == TLM_PASS_BY_COPY) {

    if (m_data) delete [] m_data;
    if (m_byte_enable) delete [] m_byte_enable;

    if( m_custom_vector_ptr != 0 ) {
      for( unsigned int i = 0; i < m_custom_vector_ptr->size(); i++ ) {
	delete (*m_custom_vector_ptr)[i];
      }
      delete m_custom_vector_ptr;
    }

  }
}
    


//----------------------
// Cross Mode Assignment operator
//----------------------
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
template< tlm_data_mode mode >
tlm_request<ADDRESS,DATA,DATA_MODE>& tlm_request<ADDRESS,DATA,DATA_MODE>::operator=(const tlm_request<ADDRESS,DATA,mode>& request)
{
  copy( request );
  return *this;
}

//----------------------
// Normal Assignment operator
//----------------------
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
tlm_request<ADDRESS,DATA,DATA_MODE>& tlm_request<ADDRESS,DATA,DATA_MODE>::operator=(const tlm_request<ADDRESS,DATA,DATA_MODE>& request )
{
  copy( request );
  return *this;
}

//----------------------
// Copy from other request object
//----------------------  
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
template< tlm_data_mode from_mode >
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::copy( const tlm_request<ADDRESS,DATA,from_mode>& request) {

  copy_arrays( request );
  
  m_command = request.get_command();
  m_mode = request.get_mode();
  m_address = request.get_address();  
  m_byte_enable_period = request.get_byte_enable_period();
  m_block_size = request.get_block_size();
  m_block_mode = request.get_block_mode();
  m_block_address_incr = request.get_block_address_incr();
  m_priority = request.get_priority();
  m_master_thread_id = request.get_master_thread_id();
  m_transaction_id = request.get_transaction_id();    
}

//----------------------
// copy array (data, byte_enable, custom) from other request object
//----------------------
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
template< tlm_data_mode from_mode >
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::copy_arrays( const tlm_request<ADDRESS,DATA,from_mode>& request) {
  
  if (request.get_data_ptr() ) set_data_array(request.get_block_size(),request.get_data_ptr());
  else m_data = NULL;
  
  if (request.get_byte_enable_ptr()) {
    int byte_enable_period = request.get_byte_enable_period();
    
    if( byte_enable_period == 0 ) byte_enable_period = request.get_block_size();
    
    set_byte_enable_array( byte_enable_period ,
			   request.get_byte_enable_ptr());
  }  
  else m_byte_enable = NULL;
  
  set_custom_vector_ptr( request.get_custom_vector_ptr() );
}

 
//------------------------------------------
// Data members command methods
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline tlm_command tlm_request<ADDRESS,DATA,DATA_MODE>::get_command() const { return(m_command); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_command(const tlm_command command) { m_command = command; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline tlm_mode tlm_request<ADDRESS,DATA,DATA_MODE>::get_mode() const{ return (m_mode); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_mode(const tlm_mode mode) { m_mode = mode; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline bool tlm_request<ADDRESS,DATA,DATA_MODE>::is_regular_access() const { return(m_mode == REGULAR); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline bool tlm_request<ADDRESS,DATA,DATA_MODE>::is_debug_access() const { return(m_mode == DEBUG); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline bool tlm_request<ADDRESS,DATA,DATA_MODE>::is_control_access() const { return(m_mode == CONTROL); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline ADDRESS tlm_request<ADDRESS,DATA,DATA_MODE>::get_address() const { return(m_address); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_address(const ADDRESS address) { m_address = address; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline const DATA& tlm_request<ADDRESS,DATA,DATA_MODE>::get_data(const unsigned int index) const {
  assert(m_data != 0);
  assert(index < m_block_size);
  return(m_data[index]);
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_data(const DATA& data,const unsigned int index) { 
  if (DATA_MODE == TLM_PASS_BY_COPY) {
    if (!m_data) m_data = new DATA[index+1]; 
    else {
      if (m_block_size<(index+1)) {
	DATA * tmp = new DATA[m_block_size];
	data_copy(tmp,m_data,m_block_size);
	delete [] m_data;
	m_data = new DATA[index+1];
	this->data_copy(m_data,tmp,m_block_size);
	delete [] tmp;	    
      }
    }
    m_block_size = index+1;
  }
  else {
    assert(m_data != 0);
    assert(index < m_block_size);
  }
  m_data[index] = data;
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline const DATA * tlm_request<ADDRESS,DATA,DATA_MODE>::get_data_ptr() const { return(m_data); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_data_array(const unsigned int size,const DATA * data_ptr) { 
  if (size) m_block_size = size;
  else m_block_size = 1; //default value
  if (DATA_MODE == TLM_PASS_BY_COPY) {
    if (m_data) delete [] m_data;
    if (size) m_data = new DATA[size];
    else m_data = NULL; // default value
    if (data_ptr) data_copy(m_data,data_ptr,size);
  }
  else m_data = const_cast<DATA * >(data_ptr);
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_block_size() const { return(m_block_size); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_block_size(const unsigned int block_size) { m_block_size = block_size; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_byte_enable(const unsigned int index) const {
  unsigned int be_size;
  if (!m_byte_enable_period) be_size = m_block_size;
  else be_size = m_byte_enable_period;
  assert(index < be_size);
  if (m_byte_enable)
    return(m_byte_enable[index]);
  else 
    return(NO_BE);
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_byte_enable(const unsigned int byte_enable,const unsigned int index) {
  unsigned int be_size;
  if (!m_byte_enable_period) be_size = m_block_size;
  else be_size = m_byte_enable_period;
    
  if (DATA_MODE == TLM_PASS_BY_COPY) {
    if (!m_byte_enable) m_byte_enable = new unsigned int[index+1]; 
    else {
	
      if (be_size<(index+1)) {
	unsigned int * tmp = new unsigned int[be_size];
	memcpy(tmp,m_byte_enable,be_size*get_nb_byte<unsigned int>());
	delete [] m_byte_enable;
	m_byte_enable = new unsigned int[index+1];
	memcpy(const_cast<unsigned int *>(m_byte_enable),tmp,be_size*get_nb_byte<unsigned int>());
	delete [] tmp;	    
      }
    }
    m_byte_enable_period = index+1;
  }
  else {
    assert(m_byte_enable != 0);
    assert(index < be_size);
  }
  const_cast<unsigned int *>(m_byte_enable)[index] = byte_enable;
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline const unsigned int * tlm_request<ADDRESS,DATA,DATA_MODE>::get_byte_enable_ptr() const { return(m_byte_enable); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_byte_enable_array(const unsigned int size,
								       const unsigned int * byte_enable_ptr) { 
  m_byte_enable_period = size;
  if (DATA_MODE == TLM_PASS_BY_COPY) {
    if (m_byte_enable) delete [] m_byte_enable;
    if (size) m_byte_enable = new unsigned int[size];
    else m_byte_enable = NULL; // default value
    if (byte_enable_ptr) memcpy(const_cast<unsigned int *>(m_byte_enable),byte_enable_ptr,size*get_nb_byte<unsigned int>());
  }
  else m_byte_enable = const_cast<unsigned int * >(byte_enable_ptr);
}

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_byte_enable_period() const { return(m_byte_enable_period); }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_byte_enable_period(const unsigned int byte_enable_period) { m_byte_enable_period = byte_enable_period; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline tlm_block_mode tlm_request<ADDRESS,DATA,DATA_MODE>::get_block_mode() const { return(m_block_mode); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_block_mode(tlm_block_mode block_mode) { m_block_mode = block_mode; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_block_address_incr() const { return(m_block_address_incr); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_block_address_incr(unsigned int block_address_incr) { m_block_address_incr = block_address_incr; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_master_thread_id() const { return(m_master_thread_id); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_priority() const { return(m_priority); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_priority(unsigned int priority) { m_priority = priority; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_master_thread_id(unsigned int master_thread_id) { m_master_thread_id = master_thread_id; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_transaction_id() const { return(m_transaction_id); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_transaction_id(unsigned int transaction_id) { m_transaction_id = transaction_id; }

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_request<ADDRESS,DATA,DATA_MODE>::get_tlm_export_id() const { return(m_tlm_export_id); }  

template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::set_tlm_export_id(unsigned int tlm_export_id) { m_tlm_export_id = tlm_export_id; }

template<typename DATA,typename ADDRESS,tlm_data_mode DATA_MODE>
template<typename T >
inline bool tlm_request<DATA,ADDRESS,DATA_MODE>::get_custom_ptr( T *& t ) const {
  
  if( m_custom_vector_ptr == 0 ) return false;
  
  std::vector<tlm_custom_base *> &v = *m_custom_vector_ptr;
  
  for( unsigned int i = 0; i < v.size(); i++ ) {
    if( (t = dynamic_cast< T *>( v[i] )) ) {
      return true;
    }
  }  
  return false;  
}

template<typename DATA,typename ADDRESS,tlm_data_mode DATA_MODE>
inline std::vector<tlm_custom_base *> *
tlm_request<DATA,ADDRESS,DATA_MODE>::get_custom_vector_ptr() const {return m_custom_vector_ptr;}

template<typename DATA,typename ADDRESS,tlm_data_mode DATA_MODE>
inline void tlm_request<DATA,ADDRESS,DATA_MODE>::set_custom_vector_ptr( std::vector< tlm_custom_base * > *t ) {
  
  if( DATA_MODE == TLM_PASS_BY_POINTER ) {
    m_custom_vector_ptr = t;
    return;
  }

  if( m_custom_vector_ptr != 0 ) {
    for( unsigned int i = 0; i < m_custom_vector_ptr->size(); i++ ) {
      delete (*m_custom_vector_ptr)[i];
    }
    delete m_custom_vector_ptr;
  }

  if( t == 0 ) {
    m_custom_vector_ptr = 0;
    return;
  }

  m_custom_vector_ptr = new std::vector<tlm_custom_base *> ( t->size() );

  tlm_custom_base *t_custom_ptr;
  for( unsigned int i = 0; i < m_custom_vector_ptr->size(); i++ ) {
    if( (t_custom_ptr = (*t)[i]) ) {
      (*m_custom_vector_ptr)[i] = t_custom_ptr->clone();
    }
  }
}




//------------------------------------------
/// Data copy function. Generic implementation
template<typename ADDRESS,typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_request<ADDRESS,DATA,DATA_MODE>::data_copy(DATA * dst,const DATA * src,const unsigned int size) {
  for(unsigned int i = 0;i < size;i++)
    dst[i] = src[i];
}

  
#define TLM_REQUEST_SCALAR_DATA_COPY(ADDRESS,DATA,DATA_MODE)\
  template<>\
  inline void tlm_request<ADDRESS,DATA,DATA_MODE>::data_copy(DATA * dst,const DATA * src,const unsigned int size) { \
    memcpy(dst,src,size*get_nb_byte<DATA>());\
  }

TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(int,tlm_sint64_t,TLM_PASS_BY_COPY);  

TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(unsigned int,tlm_sint64_t,TLM_PASS_BY_COPY);  

TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint32_t,tlm_sint64_t,TLM_PASS_BY_COPY);  

TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint32_t,tlm_sint64_t,TLM_PASS_BY_COPY);  

TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_uint64_t,tlm_sint64_t,TLM_PASS_BY_COPY);  

TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_sint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_uint8_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_uint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_sint16_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,unsigned int,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_uint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_sint32_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_uint64_t,TLM_PASS_BY_COPY);
TLM_REQUEST_SCALAR_DATA_COPY(tlm_sint64_t,tlm_sint64_t,TLM_PASS_BY_COPY); 

#endif /* _TLM_REQUEST_H_ */


