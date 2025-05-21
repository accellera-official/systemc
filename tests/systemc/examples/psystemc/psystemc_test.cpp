#include <systemc.h>

#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "sysc/utils/sc_parallel.h"
#include <unistd.h>

std::mutex mutex;

int do_work() {
    return 1;
  int f = 0;
  for (int j = 0; j < 50000; j++) {
    for (int k = 0; k < 10000; k++) {
      f += (j * k);
    }
  }
  return f;
}

SC_MODULE(my_parallel_module) {
  sc_event send_txn;

  void t1() { SC_REPORT_INFO(name(), "T1"); }
  void t2() {
    SC_REPORT_INFO(name(), "T2");
    wait(1, sc_core::SC_NS);
    SC_REPORT_INFO(name(), "T2 1ns finished wait");
  }

  void t3() {
    SC_REPORT_INFO(name(), "T3");
    for (int i = 0; i < 5; i++) {
      SC_REPORT_INFO(name(), "T3 waiting");
      wait(100, sc_core::SC_MS);

      SC_REPORT_INFO(name(), "T3 do work");
      int f = do_work();
      SC_REPORT_INFO(name(), "Work done");

      SC_REPORT_INFO(name(), "T3 done waiting");
    }
    SC_REPORT_INFO(name(), "T3 finished loop");
  }

  void m2() {
    SC_REPORT_INFO(name(), "Send TXN");
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    socket->b_transport(trans, delay);
    SC_REPORT_INFO(name(), "done TXN");
  }
  void m1() {

    SC_REPORT_INFO(
        name(),
        ("Got signal " + std::string(in.read() ? "True" : "False")).c_str());

    send_txn.notify(10, sc_core::SC_MS);
  }

public:
  SC_CTOR(my_parallel_module) {
    SC_REPORT_INFO(name(), "Ading tasks t1,t2 and t3\n");
    SC_THREAD(t1);
    SC_THREAD(t2);
    SC_THREAD(t3);

    SC_METHOD(m1);
    sensitive << in;
    dont_initialize();

    SC_METHOD(m2);
    sensitive << send_txn;
    dont_initialize();
    SC_REPORT_INFO(name(), "Constructor done");
  }

  void end_of_elaboration() {
    SC_REPORT_INFO(name(), "end_of_elaboration");
  }
  void start_of_simulation() {
    SC_REPORT_INFO(name(), "start_of_simulation");
  }
  sc_in<bool> in;

  tlm_utils::simple_initiator_socket<my_parallel_module> socket;
};

SC_MODULE(My_normal_module) {

  tlm_utils::simple_target_socket<My_normal_module> socket;

  void t1() { SC_REPORT_INFO(name(), "T1"); }
  void t2() {
    SC_REPORT_INFO(name(), "T2");
    wait(1, sc_core::SC_NS);

    SC_REPORT_INFO(name(), "T2 1ns finished wait");
  }

  void t3() {
    SC_REPORT_INFO(name(), "T3");
    for (int i = 0; i < 15; i++) {
      SC_REPORT_INFO(name(), "T3 waiting");
      wait(50, sc_core::SC_MS);
      SC_REPORT_INFO(name(), "T3 do work");

      int f = do_work();
      SC_REPORT_INFO(name(), "Work done");

      SC_REPORT_INFO(
          name(), ("Sending signal" + std::string((i & 0x1) ? "True" : "False"))
                      .c_str());
      out.write(i & 0x1);
      SC_REPORT_INFO(name(), "T3 done waiting");
    }
    SC_REPORT_INFO(name(), "T3 finished loop");
    sc_stop();
  }
  void b_transport(tlm::tlm_generic_payload & trans, sc_core::sc_time & delay) {
    SC_REPORT_INFO(name(), "Got b_transport");
  }
  SC_CTOR(My_normal_module) {
    SC_THREAD(t1);
    SC_THREAD(t2);
    SC_THREAD(t3);
    socket.register_b_transport(this, &My_normal_module::b_transport);
  }
  sc_out<bool> out;

  void end_of_elaboration() {
    SC_REPORT_INFO(name(), "end_of_elaboration");
  }
  void start_of_simulation() {
    SC_REPORT_INFO(name(), "start_of_simulation");
  }
};

void report_handler(const sc_core::sc_report &rep,
                    const sc_core::sc_actions &actions) {
  std::lock_guard<std::mutex> lock(mutex);
  cout << "thread:" << std::this_thread::get_id()
       << " simcontext:" << sc_core::sc_get_curr_simcontext()
       << " time:" << sc_core::sc_time_stamp() << " "
       << " : [" << rep.get_msg_type() << "] " << rep.get_msg() << std::endl;
}

int sc_main(int argc, char *argv[]) {

  ::sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);
  ::sc_core::sc_report_handler::set_handler(report_handler);

  tlm_utils::tlm_quantumkeeper::set_global_quantum(sc_core::sc_time(100, sc_core::SC_MS));

  My_normal_module mn("Normal");
  //my_parallel_module mp("Parallel");
  SC_PARALLEL(my_parallel_module, sc_core::sc_sync_policy_tlm_quantum) mp("Parallel");
  //SC_PARALLEL(my_parallel_module, sc_core::sc_sync_policy_in_sync) mp("Parallel");

  sc_signal<bool> sig;
  mn.out(sig);
  mp.in(sig);

  mp.socket.bind(mn.socket);

  SC_REPORT_INFO("main", "before start");
  sc_start();
  SC_REPORT_INFO("main", "finished");

  return 0;
}