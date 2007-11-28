/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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


#ifndef __TLM_RESPONSE_H__
#define __TLM_RESPONSE_H__

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "tlm_custom_base.h"
#include "tlm_status.h"

//----------------------------------------------------------------------------
///  Class tlm_response: information returned by the target module
/**
 *  It describes the information returned by the target module,including the tlm_status, tlm_metadata and 
 *  tlm_error_reason to the initiator module.
 **/
  
template<typename DATA,tlm_data_mode DATA_MODE = TLM_PASS_BY_COPY>
class tlm_response {
  
  public:
    
  //---------------
  // Constructors
  //---------------
  
  /// Default constructor
  tlm_response();
  
  /// Copy constructor
  tlm_response(const tlm_response<DATA,DATA_MODE>& response);
  
  //--------------
  // Destructor
  //--------------
  ~tlm_response();
  
  
  //----------------------
  // Cross Mode Assignment operator
  //---------------------- 
  template<tlm_data_mode mode>
  tlm_response &operator=( const tlm_response<DATA,mode>& response);
  
  //----------------------
  // Normal Assignment operator
  //----------------------  
  tlm_response &operator=( const tlm_response<DATA,DATA_MODE>& response);
  
  //----------------------
  // Copy from other response object
  //----------------------  
  template<tlm_data_mode mode>
  void copy(const tlm_response<DATA,mode>& response);


  //----------------------
  // copy array (data, custom) from other response object
  //------------------------------------------
  template<tlm_data_mode mode>
  inline void copy_arrays(const tlm_response<DATA,mode>& response);

  //------------------------------------------
  /// @name Data members access methods
  /// @{
  inline const DATA& get_data(const unsigned int index = 0) const;
  inline DATA& get_data(const unsigned int index = 0);
  inline void set_data(const DATA& data,const unsigned int index = 0);
  
  inline const DATA * get_data_ptr() const;
  inline DATA * get_data_ptr();
  inline void set_data_array(const unsigned int size,const DATA * data_ptr = NULL);
  
  inline unsigned int get_block_size() const;
  inline void set_block_size(const unsigned int block_size);
  
  inline const tlm_status& get_status() const;
  inline tlm_status& get_status();
  inline void set_status(const tlm_status& status);
  
  inline unsigned int get_priority() const;
  inline void set_priority(unsigned int priority);
  
  inline unsigned int get_master_thread_id() const;
  inline void set_master_thread_id(unsigned int master_thread_id);
  
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
  DATA * m_data;                                    //!< Response data pointer
  unsigned int m_block_size;                        //!< Determines how many chunks of data are transfered (size of block)
  tlm_status m_status;                              //!< Response status 
  unsigned int m_priority;                          //!< Priority of the response
  unsigned int m_master_thread_id;                  //!< Master module thread ID 
  unsigned int m_transaction_id;                    //!< Transaction ID 
  unsigned int m_tlm_export_id;                     //!< export id (used when several exports are used with single interface)
  std::vector<tlm_custom_base *> *m_custom_vector_ptr;   //!< Pointer to vector of custom extensions

private:  
  //------------------------------------------
  /// Data copy function. 
  static inline void data_copy(DATA * dst,const DATA * src,const unsigned int size);


};



//---------------
// Constructors
//---------------

// Default constructor
template<typename DATA,tlm_data_mode DATA_MODE>
tlm_response<DATA,DATA_MODE>::tlm_response() :
  m_data(0),
  m_block_size(0),
  m_status(),
  m_priority(0),
  m_master_thread_id( 0 ),
  m_transaction_id(0),
  m_tlm_export_id(0),
  m_custom_vector_ptr( 0 )
{}


// Copy constructor
template<typename DATA,tlm_data_mode DATA_MODE>
tlm_response<DATA,DATA_MODE>::tlm_response(const tlm_response<DATA,DATA_MODE>& response) :
  m_data(0),
  m_block_size(response.m_block_size),
  m_status(response.m_status),
  m_priority(response.m_priority),
  m_master_thread_id( response.m_master_thread_id ),
  m_transaction_id(response.m_transaction_id),
  m_tlm_export_id( response.m_tlm_export_id ),
  m_custom_vector_ptr(0)
{
  copy_arrays( response );
}
 

//--------------
// Destructor
//--------------
template<typename DATA,tlm_data_mode DATA_MODE>
tlm_response<DATA,DATA_MODE>::~tlm_response() {

  if (DATA_MODE == TLM_PASS_BY_COPY) {

    if (m_data) delete [] m_data;

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
template<typename DATA,tlm_data_mode DATA_MODE>
template<tlm_data_mode mode>
tlm_response<DATA,DATA_MODE>& tlm_response<DATA,DATA_MODE>::operator=( const tlm_response<DATA,mode>& response) {
  copy( response );
  return *this;
}

//----------------------
// Normal Assignment operator
//----------------------
template<typename DATA,tlm_data_mode DATA_MODE>  
tlm_response<DATA,DATA_MODE>& tlm_response<DATA,DATA_MODE>::operator=( const tlm_response<DATA,DATA_MODE>& response) {
  copy( response );
  return *this;
}



//----------------------
// Copy from other response object
//----------------------  
template<typename DATA,tlm_data_mode DATA_MODE> 
template<tlm_data_mode mode>
inline void tlm_response<DATA,DATA_MODE>::copy(const tlm_response<DATA,mode>& response) {

  copy_arrays( response );

  m_block_size = response.get_block_size();
  m_status = response.get_status();
  m_priority = response.get_priority();
  m_master_thread_id = response.get_master_thread_id();
  m_transaction_id = response.get_transaction_id();
  m_tlm_export_id = response.get_tlm_export_id();

} 

//----------------------
// copy array (data, custom) from other response object
//----------------------
template<typename DATA,tlm_data_mode DATA_MODE>
template<tlm_data_mode mode>
inline void tlm_response<DATA,DATA_MODE>::copy_arrays(const tlm_response<DATA,mode>& response) {
  
  if ( response.get_data_ptr() ) set_data_array( response.get_block_size(), response.get_data_ptr() );
  else m_data = NULL;

  set_custom_vector_ptr( response.get_custom_vector_ptr() );
}


//------------------------------------------
// Data members access methods

template<typename DATA,tlm_data_mode DATA_MODE>
inline const DATA& tlm_response<DATA,DATA_MODE>::get_data(const unsigned int index) const {
  assert(m_data != 0);
  assert(index < m_block_size);
  return(m_data[index]);
}

template<typename DATA,tlm_data_mode DATA_MODE>
inline DATA& tlm_response<DATA,DATA_MODE>::get_data(const unsigned int index) {
  assert(m_data != 0);
  assert(index < m_block_size);
  return(m_data[index]);
}
  
template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_data(const DATA& data,const unsigned int index) { 
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

template<typename DATA,tlm_data_mode DATA_MODE>
inline const DATA * tlm_response<DATA,DATA_MODE>::get_data_ptr() const { return(m_data); }

template<typename DATA,tlm_data_mode DATA_MODE>
inline DATA * tlm_response<DATA,DATA_MODE>::get_data_ptr() { return(m_data); }

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_data_array(const unsigned int size,const DATA * data_ptr) { 
  if (size) m_block_size = size;
  else m_block_size = 1; //default value
  if (DATA_MODE == TLM_PASS_BY_COPY) {
    if (m_data) delete [] m_data;
    if (size) m_data = new DATA[size];
    else m_data = NULL; // default value
    if (data_ptr) data_copy(m_data,data_ptr,size);
  }
  else m_data = const_cast<DATA *>(data_ptr);
}


template<typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_response<DATA,DATA_MODE>::get_block_size() const { return(m_block_size); }

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_block_size(const unsigned int block_size) { m_block_size = block_size; }

template<typename DATA,tlm_data_mode DATA_MODE>
inline const tlm_status& tlm_response<DATA,DATA_MODE>::get_status() const { return(m_status); }

template<typename DATA,tlm_data_mode DATA_MODE>
inline tlm_status& tlm_response<DATA,DATA_MODE>::get_status() { return(m_status); }

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_status(const tlm_status& status) { m_status = status; }


template<typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_response<DATA,DATA_MODE>::get_priority() const { return(m_priority); }  

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_priority(unsigned int priority) { m_priority = priority; }

template<typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_response<DATA,DATA_MODE>::get_transaction_id() const { return(m_transaction_id); }  

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_transaction_id(unsigned int transaction_id) { m_transaction_id = transaction_id; }

template<typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_response<DATA,DATA_MODE>::get_master_thread_id() const { return(m_master_thread_id); }  

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_master_thread_id(unsigned int master_thread_id ) { m_master_thread_id = master_thread_id; }

template<typename DATA,tlm_data_mode DATA_MODE>
inline unsigned int tlm_response<DATA,DATA_MODE>::get_tlm_export_id() const { return(m_tlm_export_id); }  

template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_tlm_export_id(unsigned int tlm_export_id) { m_tlm_export_id = tlm_export_id; }


template<typename DATA,tlm_data_mode DATA_MODE> 
template<typename T >
inline bool tlm_response<DATA,DATA_MODE>::get_custom_ptr( T *& t ) const {
  
  if( m_custom_vector_ptr == 0 ) return false;
  
  std::vector<tlm_custom_base *> &v = *m_custom_vector_ptr;
  
  for( unsigned int i = 0; i < v.size(); i++ ) {
    if( (t = dynamic_cast< T *>( v[i] )) ) {
      return true;
    }
  }  
  return false;  
}


template<typename DATA,tlm_data_mode DATA_MODE>
inline std::vector<tlm_custom_base *> *
tlm_response<DATA,DATA_MODE>::get_custom_vector_ptr() const { return m_custom_vector_ptr;}


template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::set_custom_vector_ptr( std::vector<tlm_custom_base *> *t ) {

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

  m_custom_vector_ptr = new std::vector<tlm_custom_base *>( t->size() );

  tlm_custom_base *t_custom_ptr;
  for( unsigned int i = 0; i < m_custom_vector_ptr->size(); i++ ) {
    if( (t_custom_ptr = (*t)[i]) ) {
      (*m_custom_vector_ptr)[i] = t_custom_ptr->clone();
    }
  }

}




//------------------------------------------
/// Data copy function. Generic implementation
template<typename DATA,tlm_data_mode DATA_MODE>
inline void tlm_response<DATA,DATA_MODE>::data_copy(DATA * dst,const DATA * src,const unsigned int size) {
  for(unsigned int i = 0;i < size;i++)
    dst[i] = src[i];
}
  


  
#define TLM_RESPONSE_SCALAR_DATA_COPY(DATA,DATA_MODE)\
  template<>\
  inline void tlm_response<DATA,DATA_MODE>::data_copy(DATA * dst,const DATA * src,const unsigned int size) { \
    memcpy(dst,src,size*get_nb_byte<DATA>());\
  }
  
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_sint8_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_uint8_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_uint16_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_sint16_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(int,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(unsigned int,TLM_PASS_BY_COPY)
#ifndef _MSC_VER
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_uint32_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_sint32_t,TLM_PASS_BY_COPY)
#endif
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_uint64_t,TLM_PASS_BY_COPY)
TLM_RESPONSE_SCALAR_DATA_COPY(tlm_sint64_t,TLM_PASS_BY_COPY)


#endif /* _TLM_RESPONSE_H_ */


