//****************************************************************************
//****************************************************************************
//
// Copyright (c) 2002 Thorsten Groetker, Stan Liao, Grant Martin, Stuart Swan
//
// Permission is hereby granted to use, modify, and distribute this source
// code in any way as long as this entire copyright notice is retained
// in unmodified form within the source code.
//
// This software is distributed on an "AS IS" basis, without warranty
// of any kind, either express or implied.
//
// This source code is from the book "System Design with SystemC".
// For detailed discussion on this example, see the relevant section
// within the "System Design with SystemC" book.
//
// To obtain the book and find additional source code downloads, etc., visit
//     www.systemc.org 
// Look in the "Products & Solutions" -> "SystemC Books". Then look at the
// entry for "System Design with SystemC".
//
//****************************************************************************
//****************************************************************************

// $Log: scx_mutex_w_policy.cpp,v $
// Revision 1.3  2011/08/26 22:26:05  acg
//  Torsten Maehne: eliminated unused argument warnings.
//
// Revision 1.2  2011/08/15 16:43:24  acg
//  Torsten Maehne: changes to remove unused argument warnings.
//
// Revision 1.1.1.1  2006/12/15 20:20:03  acg
// SystemC 2.3
//
// Revision 1.3  2006/03/21 01:31:34  acg
//  Andy Goodrich: changed over to sc_get_current_process_b() from
//  sc_get_current_process_base() since the function's name changed.
//
// Revision 1.2  2006/02/04 00:09:35  acg
//   Andy Goodrich: changed over to sc_get_current_process_b() from
//   sc_get_curr_process_handle() to eliminate deprecation warning.
//





// Specialization of sc_mutex to allow FIFO or true RANDOM
//  arbitration policies when more than one process is
//  contending for the mutex
//
// This mutex is also significantly more efficient than sc_mutex
//  if there is a lot of actual contention for ownership of the mutex,
//  since only the process that actually acquires the mutex is resumed.
//
// See section 7.4, page 120 of "System Design with SystemC" for
//  additional discussion

#include <systemc.h>

class scx_mutex_w_policy : public sc_mutex 
{ 
public: 
  enum policy_type { FIFO, RANDOM };

  explicit scx_mutex_w_policy(policy_type policy) : policy_(policy) {}

  virtual int lock()
  {
    if (in_use()) { 
      sc_event my_event; 
      event_list.push_back(&my_event); 
      wait(my_event); 
    } 

    m_owner = sc_get_current_process_b();
    return 0; 
  }

  virtual int unlock()
  { 
    if (m_owner != sc_get_current_process_b()) return -1; 

    m_owner = 0; 
    sc_event* e = next_event(); 
    if (e) e->notify(); 

    return 0; 
  }

private: 
  sc_event* next_event()
  {
    if (event_list.empty())
      return 0;

    if (policy_ == FIFO)
    {
      return event_list.pop_front();
    }
    else
    { // RANDOM
      sc_plist_iter<sc_event*> ev_itr(&event_list); 
      int index = rand() % event_list.size(); 
      for (int i = 0; i < index; i++)
        ev_itr++; 

      sc_event* e = ev_itr.get(); 
      ev_itr.remove(); 
      return e; 
    }
  } 

  sc_plist<sc_event*> event_list; 
  policy_type policy_;
}; 

class top : public sc_module
{
public:
  SC_HAS_PROCESS(top);

  top(sc_module_name name) : sc_module(name), mutex_(scx_mutex_w_policy::FIFO)
  {
    SC_THREAD(t1);
    SC_THREAD(t2);
    SC_THREAD(t3);
  }

  void t1() {
    wait(1, SC_NS);
    mutex_.lock();
    cout << "t1 got mutex at " << sc_time_stamp() << endl;
    wait(10, SC_NS);
    mutex_.unlock();
  }

  void t2() {
    wait(2, SC_NS);
    mutex_.lock();
    cout << "t2 got mutex at " << sc_time_stamp() << endl;
    wait(10, SC_NS);
    mutex_.unlock();
  }

  void t3() {
    wait(3, SC_NS);
    mutex_.lock();
    cout << "t3 got mutex at " << sc_time_stamp() << endl;
    wait(10, SC_NS);
    mutex_.unlock();
  }

private:
    scx_mutex_w_policy mutex_;
};

int sc_main (int, char*[]) 
{
  top top1("Top1");
  sc_start(1000, SC_NS);
  cout << endl << endl;
  return 0;
}



