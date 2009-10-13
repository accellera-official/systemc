//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
//   All Rights Reserved Worldwide
//   Copyright 2009 Forte Design Systems
//----------------------------------------------------------------------

#include <systemc.h>
    
using sc_core::sc_kill;

class A {
public:
  A(const char* name) : m_name(name) {}
  ~A() { cerr << sc_time_stamp() << " " << m_name << ": in A's dtor" << endl; }
  const char* m_name;
};

SC_MODULE(top) {
public:
  SC_CTOR(top) {
    SC_THREAD(victim);
    h = sc_get_current_process_handle();
    SC_THREAD(perpetrator);
  }

  void level1()
  {
    A a("level1");
    try {
        cerr << sc_time_stamp() << " " << "level1: entry" << endl;
	level2();
    } 
    catch(sc_kill)
    {
        cerr << sc_time_stamp() << " " << "level1: caught sc_kill" << endl;
	throw sc_kill();
    }
  }

  void level2()
  {
    A a("level2");
    try {
        cerr << sc_time_stamp() << " " << "level2: entry" << endl;
	::sc_core::wait(100, SC_NS);
    } 
    catch(sc_kill)
    {
        cerr << sc_time_stamp() << " " << "level2: caught sc_kill" << endl;
	throw sc_kill();
    }
  }

  void victim() {
    try {
        A a("victim");
        cerr << sc_time_stamp() << ": starting victim thread" << endl;
	level1();
        cerr << sc_time_stamp() << ": ending victim thread" << endl;
    } 
    catch(sc_kill) 
    { 
        cerr << sc_time_stamp() << " " << "victim: caught sc_kill" << endl;
    }
  }

  void perpetrator() {
    wait(10, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator killing victim " << endl;
    h.kill();
    wait(10, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator after killing victim " << endl;
    sc_stop();
  }

protected:
  sc_process_handle h;
  sc_process_handle h2;
};

int sc_main (int argc, char *argv[])
{
  top t("top");
  sc_start();
  return 0;
}

