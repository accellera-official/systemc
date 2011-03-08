// Process control methods interacting with time-out and event lists

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

struct Top: sc_module
{
  Top(sc_module_name _name)
  {
    SC_THREAD(calling);
    
    SC_THREAD(target1);
      t1 = sc_get_current_process_handle();
      
    SC_THREAD(target2);
      t2 = sc_get_current_process_handle();
      
    count = 0;
    target1_finished = false;
    target2_finished = false;
    target2_reset = false;
  }
  
  sc_process_handle t1, t2;
  sc_event ev1, ev2, ev3;
  int count;
  bool target1_finished;
  bool target2_finished;
  bool target2_reset;

  void calling()
  {
    count = 0;
    wait(SC_ZERO_TIME);
    
    count = 1;
    wait(15, SC_NS);
    
    count = 2;
    t1.disable();
    wait(20, SC_NS);
    
    t1.enable();
    wait(20, SC_NS);
    
    count = 3;
    ev1.notify();
    wait(10, SC_NS);
    
    count = 4;
    t1.reset();  // Seg fault !!!!!!!!  
    
    wait(sc_time(100, SC_NS) - sc_time_stamp());
    
    t2.disable();
    wait(10, SC_NS);
    
    ev2.notify();
    wait(10, SC_NS);
    
    t2.enable();
    wait(10, SC_NS);

    ev3.notify();
    wait(10, SC_NS);
    
    ev2.notify();
    wait(sc_time(200, SC_NS) - sc_time_stamp());
    
    ev1.notify();
    wait(10, SC_NS);
    
    t2.suspend();
    wait(10, SC_NS);
    
    ev2.notify();
    wait(10, SC_NS);
    
    t2.resume();
    wait(10, SC_NS);
    
    ev3.notify();
    wait(sc_time(300, SC_NS) - sc_time_stamp());
    
    ev1.notify();
    wait(10, SC_NS);
    
    ev2.notify();
    wait(10, SC_NS);
    
    target2_reset = true;    
    t2.reset();
    wait(10, SC_NS);
    
    ev3.notify();
    wait(10, SC_NS);
    
    ev1.notify();
    wait(10, SC_NS);
    
    ev2.notify();
    wait(sc_time(400, SC_NS) - sc_time_stamp());
   
    sc_stop();
  }

  void target1()
  {
    switch (count)
    {
        case  0: sc_assert( sc_time_stamp() == sc_time(0, SC_NS) ); break;
        case  4: sc_assert( sc_time_stamp() == sc_time(65, SC_NS) ); break;
        default: sc_assert( false ); break;
    }
    
    for (;;)
    {
      wait(10, SC_NS);
      switch (count)
      {
        case  1: sc_assert( sc_time_stamp() == sc_time(10, SC_NS) ); 
                 target1_finished = true; break;
        default: sc_assert( false ); break;
      }
    }
  }
  
  void target2()
  {
    wait(ev1 & ev2 & ev3);
    
    if (target2_reset == false)
      sc_assert( sc_time_stamp() == sc_time(140, SC_NS) );
    else
    {
      sc_assert( sc_time_stamp() == sc_time(350, SC_NS) );
      target2_finished = true;
    }
    
    wait(ev1 & ev2 & ev3);
    sc_assert( sc_time_stamp() == sc_time(240, SC_NS) );
    
    wait(ev1 & ev2 & ev3);
    sc_assert( false );
  }
  
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start();
  
  sc_assert( top.target1_finished );
  sc_assert( top.target2_finished );
  
  cout << endl << "Success" << endl;
  return 0;
}
  
