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
#include "tlm.h"

namespace tlm{

//Helper to do the numbering of private extension accessors
inline unsigned int max_num_accessors(bool increment=false)
{
    static unsigned int max_num = 0;
    if (increment) ++max_num;
    return max_num;
}

//Helper to do the index generation for private extensions
inline unsigned int max_num_private_extensions(bool increment=false)
{
    static unsigned int max_num = 0;
    if (increment) ++max_num;
    return max_num;
}

//The private extension base. Similar to normal extension base, but without clone and free
class tlm_private_extension_base
{
public:
    virtual ~tlm_private_extension_base() {}
protected:
    static unsigned int register_private_extension()
    {
        return (max_num_private_extensions(true) - 1);
    };
};

//The templated private extension. Similar to normal extension
template <typename T>
class
tlm_private_extension : public tlm_private_extension_base{
public:
    virtual ~tlm_private_extension() {}
    const static unsigned int privID;
};

template <typename T>
const
unsigned int tlm_private_extension<T>::privID = tlm_private_extension_base::register_private_extension();


//this thing is basically a snippet of the generic_payload
// it contains all the extension specific code (the extension API so to speak)
// the differences are:
// - it calls back to its owner whenever a real (==non-NULL) extension gets set for the first time
// - it calls back to its owner whenever a living (==non-NULL) extension gets cleared
template<typename U>
class tlm_private_extensions_per_accessor{
public:

  typedef void (U::*cb)();

  tlm_private_extensions_per_accessor(U* container, cb inc, cb dec): m_container(container), m_inc(inc), m_dec(dec){
  }

  template <typename T> T* set_extension(T* ext)
  {
      resize_extensions();
      T* tmp = static_cast<T*>(m_extensions[T::privID]);
      m_extensions[T::privID] = static_cast<tlm_private_extension_base*>(ext);
      if (!tmp && ext) (m_container->*m_inc)();
      return tmp;
  }
  // non-templatized version with manual index:
  tlm_private_extension_base* set_extension(unsigned int index,
                                    tlm_private_extension_base* ext)
  {
      resize_extensions();
      tlm_private_extension_base* tmp = m_extensions[index];
      m_extensions[index] = ext;
      if (!tmp && ext) (m_container->*m_inc)();
      return tmp;
  }

  // Check for an extension, ext will point to 0 if not present
  template <typename T> void get_extension(T*& ext) const
  {
      ext = static_cast<T*>(m_extensions[T::privID]);
  }
  // Non-templatized version:
   tlm_private_extension_base* get_extension(unsigned int index) const
  {
      return m_extensions[index];
  }

  // Clear extension, the argument is needed to find the right index:
  template <typename T> void clear_extension(const T* ext)
  {
      resize_extensions();
      if (m_extensions[T::privID]) (m_container->*m_dec)();
      m_extensions[T::privID] = static_cast<tlm_private_extension_base*>(0);
  }
  // Non-templatized version with manual index
  void clear_extension(unsigned int index)
  {
      if (index < m_extensions.size())
      {
          if (m_extensions[index]) (m_container->*m_dec)();
          m_extensions[index] = static_cast<tlm_private_extension_base*>(0);
      }
  }

  // Make sure the extension array is large enough. Can be called once by
  // an initiator module (before issuing the first transaction) to make
  // sure that the extension array is of correct size. This is only needed
  // if the initiator cannot guarantee that the generic payload object is
  // allocated after C++ static construction time.
  void resize_extensions()
  {
      m_extensions.expand(max_num_private_extensions());
  }
  
private:
  tlm_array<tlm_private_extension_base*> m_extensions;
  U* m_container;
  cb m_inc, m_dec;
  
};

class tlm_private_extension_container;


//the pool for the container, plain as can be
class tlm_private_extension_container_pool{
  friend class tlm_private_extension_carrier;
  friend class tlm_private_extension_container;
  inline static tlm_private_extension_container*& getUnused();
  inline static tlm_private_extension_container* create();
  inline static void free(tlm_private_extension_container*);
};

class tlm_private_extension_carrier;

//this thing contains the vector of extensions per accessor
//which can be really large so this one should be pool allocated
// therefore it keeps a use_count of itself to automatically free itself
// - to this end it provides callbacks to the extensions per accessor
//   to increment and decrement the use_count
class tlm_private_extension_container{
  friend class tlm_private_extension_container_pool;
  friend class tlm_extension_accessor;
  friend class tlm_private_extension_carrier;
  
  tlm_private_extension_container(): use_count(0), next(NULL){resize();}
  
  void resize(){
    //TODO: eoe done check
    m_private_extensions_per_accessor.resize(max_num_accessors());
    for (unsigned int i=0; i<m_private_extensions_per_accessor.size(); i++) {
      m_private_extensions_per_accessor[i]=new tlm_private_extensions_per_accessor<tlm_private_extension_container>(this, 
                                                                                   &tlm_private_extension_container::inc_use_count, 
                                                                                   &tlm_private_extension_container::dec_use_count
                                                                                   );
      m_private_extensions_per_accessor[i]->resize_extensions();
    }
  }
 
  void inc_use_count(){use_count++;}
  void dec_use_count(){
    if ((--use_count)==0) { //if this container isn't used any more
      tlm_private_extension_container_pool::free(this);  //we send it back to our pool
      my_txn->clear_extension(my_carrier); //and remove it from the transaction's extension array
    }
  }
  
  std::vector<tlm_private_extensions_per_accessor<tlm_private_extension_container>* > m_private_extensions_per_accessor; 
  unsigned int use_count;
  tlm_generic_payload* my_txn;
  tlm_private_extension_carrier* my_carrier;
  tlm_private_extension_container* next; //for pooling
};


tlm_private_extension_container*& tlm_private_extension_container_pool::getUnused(){
  static tlm_private_extension_container* unused=NULL;
  return unused;
}

tlm_private_extension_container* tlm_private_extension_container_pool::create(){  //dummy for now
  tlm_private_extension_container*& unused=getUnused();
  if (!unused) {unused=new tlm_private_extension_container();}
  tlm_private_extension_container* tmp=unused;
  unused=unused->next;
  return tmp; 
}

void tlm_private_extension_container_pool::free(tlm_private_extension_container* cont){
  tlm_private_extension_container*& unused=getUnused();
  cont->next=unused;
  unused=cont;
}

//This is the class that actually sits in the extension array
//we keep this small since that one gets allocated and deallocated all the times
class tlm_private_extension_carrier: public tlm_extension<tlm_private_extension_carrier>{
  friend class tlm_extension_accessor;

public:
  tlm_private_extension_carrier(){
    m_container=tlm_private_extension_container_pool::create();
    m_container->my_carrier=this;
  }
  
  virtual tlm_extension_base* clone() const {
    //we don't clone since private info is instance specific and associated to a given txn (the original)
    //so the deep copied txn will be virgin in terms of private info
    return NULL;
  }
private:
  tlm_private_extension_container* m_container;
};

//This class 'hides' all the instance specific extension stuff from the user
// he instantiates one of those (e.g. tlm_extension_accessor extAcc;) and can then access
// the private and public extensions
//  if he wants to access private extensions he calls 
//    extAcc.privateAccess(txn).extensionAPIFnCall()
//  if he wants to access public extensions he calls 
//    extAcc.publicAccess(txn).extensionAPIFnCall()
//  where extensionAPIFnCall is set_extension, get_extension, clear_extension,...
class tlm_extension_accessor{
public:
  tlm_extension_accessor(): m_index(max_num_accessors(true)-1){}
  
  template<typename T>
  inline tlm_private_extensions_per_accessor<tlm_private_extension_container>& privateAccess(T& txn){
    tlm_private_extension_carrier* carrier;
    txn.get_extension(carrier);
    if (!carrier){
      carrier=new tlm_private_extension_carrier();
      carrier->m_container->my_txn=&txn;
      txn.set_extension(carrier);
    }
    return *(carrier->m_container->m_private_extensions_per_accessor[m_index]);
  }
  
  template<typename T>
  inline T& publicAccess(T& txn){
    //this is a no-op. It's only purpose is to a a symetric API for public and private access
    return txn;
  }
    
protected:
  unsigned int m_index;
};

}
