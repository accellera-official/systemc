In addition to processes created in sc_module constructors via the 
SC_METHOD and SC_THREAD macros, processes can also be created after 
simulation starts with the sc_spawn() API. The implementation uses 
(and ships) a part of the publicly available boost library 
(www.boost.org). In particular the boost::bind templates are used. 
User code must define the macro SC_INCLUDE_DYNAMIC_PROCESSES before 
including "systemc.h" in order for the right header files to get 
included. 

Language Constructs
---
The user-visible constructs for dynamic process creation and synchronization are:

   sc_spawn(...)
   sc_spawn_options(...)
   SC_FORK
   SC_JOIN
   sc_process_handle::wait()
   sc_bind(...)
   sc_ref(...)
   sc_cref(...)


Basic Usage of sc_spawn
---

Given the following function and method declarations:

  returnT my_function( ARGS );

  returnT my_class::my_method ( ARGS );

To spawn these, use:

  returnT r;

  my_class* c;
  c = this; // or point to some other object...

  sc_process_handle h1 = sc_spawn( &r, sc_bind(&my_class::my_method, c, ARGS ));
  sc_process_handle h2 = sc_spawn( &r, sc_bind(&my_function,            ARGS ));


Function Arguments
---

A spawned function can have up to 9 arguments, a spawned class 
method up to 8 arguments (this restriction comes with the usage of 
boost bind library). 

Strict type checking of arguments is done. Arguments can by passed by
value (default), per reference (use sc_ref) or per const reference
(use sc_cref). Example:

     int my_function( double FA1, double &FA2, const double &FA2 );

     int r;
     double A;

     sc_spawn( &r, sc_bind(&my_function, A, sc_ref(A), sc_cref(A)) );

If the spawned function returns no value, or if you do not wish to
use the returned value, the first argument (r above) may be omitted:

     sc_spawn( sc_bind(&my_function, A, sc_ref(A), sc_cref(A)) );
    
If the first argument is included, the pointed to space must be
kept valid until spawned function completes, at which point the returned
value will be stored in the space. 

sc_spawn_options
---

After the sc_bind() argument to sc_spawn() is specified, two more 
optional arguments can be specified to sc_spawn(). The second of 
these two optional arguments is a pointer to sc_spawn_options. 
sc_spawn_options can be used to control the spawning of a thread 
process or a method process, and to specify static sensitivity 
information and dont_initialize information for dynamic spawned 
processes, similar to static processes. For spawned threads, the 
stack size information can also be specified through 
sc_spawn_options. The sc_spawn_options class supports the following 
API: 

void set_stack_size(int stack_size); 
// specify stack size for threads, ignored for methods 

void spawn_method(); 
// spawn a method process, the default is a thread process

void dont_initialize(); 
// don't schedule the spawned process for an initial execution, 
// by default it is scheduled for an initial execution

void set_sensitivity(sc_event* e); 
// make spawned process statically sensitive to the event 

void set_sensitivity(sc_port_base* p); 
// make spawned process statically sensitive to the default event 
// of the interface bound to the port

void set_sensitivity(sc_interface* i); 
// make spawned process statically sensitive to the default event 
// of the interface 

void set_sensitivity(sc_event_finder* f); 
// make spawned process statically sensitive to the event 
// returned by the find_event() member of the event finder 

Each of the set_sensitivity() methods can be called multiple times 
to indicate static sensitivity on multiple objects (e.g., specify 
sensitivity on events e1 and e2, and port p1). 

Naming a spawned process:
---

The first of the 2 optional arguments that sc_spawn() accepts after 
the sc_bind() argument is "const char* proc_name". The "proc_name" 
argument can be provided to name the spawned process. A spawned 
process gets a hierarchical name similar to other sc_objects. If the 
user explicitly provides a name "proc_name" to sc_spawn(), the full 
name of the spawned process is "parent_name.proc_name", where 
"parent_name" is the full name of the parent sc_object that spawned 
the process. Note that, since a sc_spawn_options* argument must be 
provided in order to spawn a method process, a spawned method 
process must also be explicitly named by the user, otherwise the 
design won't compile. If the user spawns a thread process, and does 
not specify an explicit name, then the tool generates a name of the 
form "thread_p_N" where "N" is a number that indicates this is the 
"Nth" child thread of the direct parent, where names are not reused 
when children of the direct parent die. Note that if a currently 
executing process spawns another process, then the currently 
executing process is the direct parent of the spawned process. 



Synchronization
-------

A Fork/Join construct is provided:
  SC_FORK 
      sc_spawn(...) , 
      sc_spawn(...) , 
      ...  
  SC_JOIN

Please note that individual sc_spawn(...) sections are separated by
commas and that there is no curly braces ("{","}") used, nor a
semicolon at the end of SC_JOIN. 

The code will only wait until all spawned processes have returned.

It is also possible to wait for an individual spawned process to
finish with the sc_process_handle::wait()  function:

  sc_process_handle h = sc_spawn(...);
  ...
  h.wait();

Note that SC_FORK/SC_JOIN as well as sc_process_handle::wait()
indirectly calls wait(some_event) and therefore can only be used
within a thread context. If you call SC_FORK/SC_JOIN within a method
context or outside any process, then SystemC will produce a runtime
error.

It is also an error to call sc_process_handle::wait() on a handle 
associated with a spawned method process, because a method process 
never finishes. Similarly, SC_FORK-SC_JOIN cannot be used to spawn 
any method process. 

sc_spawn() merely creates a process and schedules it for an initial 
execution (unless dont_initialize is specified through 
sc_spawn_options) . It does NOT execute the process. The spawned 
process executes when control goes back to the scheduler. 

It is important to note that sc_spawn is a strict superset of the 
functionality available via the SC_THREAD and SC_METHOD macros. The 
SC_THREAD and SC_METHOD macros are retained for compatibility with 
earlier versions of SystemC. However in SystemC 2.1 and in future 
versions of SystemC, it is not possible to invoke the SC_THREAD and 
SC_METHOD macros after simulation starts. In addition, it IS 
possible to call sc_spawn both before, and after simulation starts. 

Example
---

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

int function_method(double d)
{
  cout << endl << sc_time_stamp() << ", "
       << sc_get_curr_process_handle()->name()
       << ": function_method sees " << d << endl;
  return int(d);
}

class module1 : public sc_module
{
private:
  sc_event& ev;
public:

  SC_HAS_PROCESS(module1);

  module1(sc_module_name name, sc_event& event) : sc_module(name),
    ev(event)
  {
    int r;
    SC_THREAD(main);
    cout << endl << sc_time_stamp() << ": CTOR, Before spawning function_method" << endl;
    sc_spawn_options o1;
    o1.spawn_method();
    o1.dont_initialize();
    o1.set_sensitivity(&ev);
    sc_process_handle h4 = sc_spawn(&r, sc_bind(&function_method, 1.2345), "event_sensitive_method", &o1);

  }

  void main()
  {
    sc_event e1, e2, e3, e4;

    cout << endl << sc_time_stamp() << ", "
    << sc_get_curr_process_handle()->name()
    << ": main thread, Before spawning round robin threads."
    << endl << endl;

    e1.notify(100, SC_NS);

    // Spawn several threads that co-operatively execute in round robin order

    SC_FORK
      sc_spawn(
        sc_bind(&module1::round_robin, this, "1", sc_ref(e1), sc_ref(e2), 3), "1") ,
      sc_spawn(
        sc_bind(&module1::round_robin, this, "2", sc_ref(e2), sc_ref(e3), 3), "2") ,
      sc_spawn(
        sc_bind(&module1::round_robin, this, "3", sc_ref(e3), sc_ref(e4), 3), "3") ,
      sc_spawn(
        sc_bind(&module1::round_robin, this, "4", sc_ref(e4), sc_ref(e1), 3), "4") ,
    SC_JOIN

    cout << endl << sc_time_stamp() << ", "
         << sc_get_curr_process_handle()->name()
         << ": Done main thread." << endl;
  }

  void round_robin(const char *str, sc_event& receive, sc_event& send, int cnt)
  {
    while (--cnt >= 0)
    {
      wait(receive);
      cout << sc_time_stamp() << ": " << sc_get_curr_process_handle()->name()
           << ": Round robin thread " << str << endl;
      wait(10, SC_NS);
      send.notify();
    }
  }

};

int sc_main (int argc , char *argv[])
{
  sc_event event1;
  event1.notify(55, SC_NS);

  module1 mod1("mod1", event1);
  sc_start(500, SC_NS);
  return 0;
}


