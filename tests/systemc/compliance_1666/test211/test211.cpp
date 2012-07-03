#include <systemc>
using namespace sc_core;
using namespace sc_dt;
using std::cout;
using std::endl;

// 11) sc_is_running()

struct my_port: sc_port<sc_signal_in_if<int> >
{
  my_port()
  {
    assert(sc_is_running() == false);
  }
  ~my_port()
  {
    assert(sc_is_running() == false);
  }
  void before_end_of_elaboration()
  {
    assert(sc_is_running() == false);
  }
  void end_of_elaboration()
  {
    assert(sc_is_running() == false);
  }
  void start_of_simulation()
  {
    assert(sc_is_running() == false);
  }
  void end_of_simulation()
  {
    assert(sc_is_running() == false);
  }
  int read()
  {
    assert(sc_is_running() == true);
    return (*this)->read();
  }
};

SC_MODULE(M)
{
  my_port p;
  SC_CTOR(M)
  {
    assert(sc_is_running() == false);
    SC_THREAD(T);
    SC_METHOD(ME);
  }
  ~M()
  {
    assert(sc_is_running() == false);
  }
  void T()
  {
    int i = p.read();
    assert(sc_is_running() == true);
    wait (1, SC_NS);
    assert(sc_is_running() == true);
    wait (1, SC_NS);
    assert(sc_is_running() == true);
    wait (1, SC_NS);
  }
  void ME()
  {
    assert(sc_is_running() == true);
  }
  void before_end_of_elaboration()
  {
    assert(sc_is_running() == false);
  }
  void end_of_elaboration()
  {
    assert(sc_is_running() == false);
  }
  void start_of_simulation()
  {
    assert(sc_is_running() == false);
  }
  void end_of_simulation()
  {
    assert(sc_is_running() == false);
  }
};

struct Top: sc_module
{
  M *m;
  sc_signal<int> sig;
  Top(sc_module_name)
  {
    assert(sc_is_running() == false);
    m = new M("m");
    m->p.bind(sig);
  }
};

int sc_main(int argc, char* argv[])
{
  cout << "Should be silent..." << endl;

  assert(sc_is_running() == false);
  Top top("top");
  assert(sc_is_running() == false);
  sc_start();

  cout << endl << "Success" << endl;
  return 0;
}
