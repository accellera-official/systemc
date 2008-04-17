/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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
#ifndef PAYLOAD_EVENT_QUEUE_H
#define PAYLOAD_EVENT_QUEUE_H

#include <vector>
#include <systemc>
#include "tlm.h"

template <typename PAYLOAD>
class timeorderedlist
{
public:
  struct element
  {
    struct element  *next;
    PAYLOAD p;
    sc_core::sc_time t;
    sc_dt::uint64 d;
    element(PAYLOAD& p, sc_core::sc_time t, sc_dt::uint64 d): p(p),t(t),d(d) {}
    element(){}
  };
  
  element *nill;
  element *empties;
  element *list;
  unsigned int size;
    
  timeorderedlist() 
    : nill(new element()),
      empties(NULL),
      list(nill),
      size(0)
  {
  }
  
  ~timeorderedlist() {
    while(size) {
      delete_top();
    }

    while(empties){
      struct element *e=empties->next;
      delete empties;
      empties=e;
    }
    delete nill;
  }
  
  void insert(const PAYLOAD& p, sc_core::sc_time t) {
    if (!empties) {
      empties=new struct element();
      empties->next=NULL;
    }
    
    struct element *e=empties;
    empties=empties->next;
    e->p=p;
    e->t=t;
    e->d=sc_core::sc_delta_count();
    
    struct element * ancestor=nill;
    struct element * iterator=list;
    while (iterator!=nill && iterator->t<=t){
      ancestor=iterator;
      iterator=iterator->next;
    }
    if (ancestor==nill){
      e->next=list;
      list=e;
    }
    else {
      e->next=iterator;
      ancestor->next=e;
    }
    size++;
  }
  
  void delete_top(){
    if (list != nill) {
      struct element *e=list;
      list=list->next;
      e->next=empties;
      empties=e;
      size--;
    }
  }
  
  unsigned int getSize()
  {
    return size;
  }
  
  PAYLOAD &top()
  {
    return list->p;
  }
  sc_core::sc_time top_time()
  {
    return list->t;
  }

  sc_dt::uint64& top_delta()
  {
    return list->d;
  }

  sc_core::sc_time next_time()
  {
    return list->next->t;
  }
};

//---------------------------------------------------------------------------
/**
 * An event queue that can contain any number of pending
 * notifications. Each notification have an associate payload.
 */
//---------------------------------------------------------------------------
template<typename OWNER,typename TYPES=tlm::tlm_generic_payload_types>
class payload_event_queue: 
  public sc_core::sc_module
{

  typedef typename TYPES::tlm_payload_type tlm_payload_type;
  typedef typename TYPES::tlm_phase_type   tlm_phase_type;
  typedef std::pair<tlm_payload_type*, tlm_phase_type> PAYLOAD;
  typedef void (OWNER::*cb)(tlm_payload_type&, const tlm_phase_type&);
  
  class delta_list{
  public:
    delta_list(){
      reset();
      entries.resize(100);
    }
    
    inline void insert(const PAYLOAD& p){
      if (size==entries.size()){
        entries.resize(entries.size()*2);
      }
      entries[size++]=p;
    }
    
    inline PAYLOAD& get(){
      return entries[out++];
    }
    
    inline bool next(){
      return out<size;
    }
    
    inline void reset(){
      size=0;
      out=0;
    }
  public:
    unsigned int size;
  private:
    std::vector<PAYLOAD> entries;
    unsigned int out;
  };

public:

  //callback????
  
  SC_HAS_PROCESS( payload_event_queue );
  
  payload_event_queue(OWNER* _owner, cb _cb)
    :sc_module( sc_core::sc_gen_unique_name( "payload_event_queue" ) )
    ,m_owner(_owner)
    ,m_cb(_cb)
  {
    SC_METHOD( fec );
    sensitive << m_e;
    dont_initialize();  
    end_module();
  }

  payload_event_queue(sc_core::sc_module_name name_,OWNER* _owner,cb _cb)
    : sc_core::sc_module( name_ )
    ,m_owner(_owner)
    ,m_cb(_cb)
  {
    SC_METHOD( fec );
    sensitive << m_e;
    dont_initialize();
    end_module();
  }
  
  ~payload_event_queue(){}
  
  void notify (tlm_payload_type& t, tlm_phase_type& p, const sc_core::sc_time& when){
    if (when==sc_core::SC_ZERO_TIME) {
      if (sc_core::sc_delta_count() & (sc_dt::uint64)0x1) //uneven delta cycle so delta delay is for even cylce
        m_evenDelta.insert(PAYLOAD(&t,p));
      else
        m_unevenDelta.insert(PAYLOAD(&t,p)); //even delta cycle so delta delay is for uneven delta
      m_e.notify(sc_core::SC_ZERO_TIME);
    }
    else {
      m_ppq.insert(PAYLOAD(&t,p),  when + sc_core::sc_time_stamp() );
      m_e.notify(when); // note, this will only over-right the "newest" event.
    }
  }

  void notify (tlm_payload_type& t, tlm_phase_type& p){
    m_immediateYield.insert(PAYLOAD(&t,p));
    m_e.notify(); // immediate notification
  }
  
private:
  
  void fec(){
    //immediate yield notifications
    while(m_immediateYield.next()) {PAYLOAD& tmp=m_immediateYield.get(); (m_owner->*m_cb)(*tmp.first, tmp.second);}
    m_immediateYield.reset();
    
    //delta notifications
    if (sc_core::sc_delta_count() & (sc_dt::uint64) 0x1) {//uneven delta so put out all payloads for uneven delta
      while (m_unevenDelta.next()) {PAYLOAD& tmp=m_unevenDelta.get(); (m_owner->*m_cb)(*tmp.first, tmp.second);}
      m_unevenDelta.reset();
      if (m_evenDelta.size) m_e.notify(sc_core::SC_ZERO_TIME);
    }
    else {
      while (m_evenDelta.next()) {PAYLOAD& tmp=m_evenDelta.get(); (m_owner->*m_cb)(*tmp.first, tmp.second);}
      m_evenDelta.reset();  
      if (m_unevenDelta.size) m_e.notify(sc_core::SC_ZERO_TIME);
    }
    if (!m_ppq.getSize()) return; //there were only delta notification
    
    //timed notifications
    const sc_core::sc_time now=sc_core::sc_time_stamp();
    sc_core::sc_time top=m_ppq.top_time();

    while(m_ppq.getSize() && top==now) { // push all active ones into target
      PAYLOAD& tmp=m_ppq.top();
      (m_owner->*m_cb)(*tmp.first, tmp.second);
      m_ppq.delete_top();
      top=m_ppq.top_time();
    }
    if ( m_ppq.getSize()) {
      m_e.notify( top - now) ;
    }
    
  }
  
  OWNER* m_owner;
  cb     m_cb;
  
  timeorderedlist<PAYLOAD> m_ppq;
  delta_list m_unevenDelta;
  delta_list m_evenDelta;
  delta_list m_immediateYield;
  
  sc_core::sc_event m_e;   // default event
};

#endif // PAYLOAD_EVENT_QUEUE_H
