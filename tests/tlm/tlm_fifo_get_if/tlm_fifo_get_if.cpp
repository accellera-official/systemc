#include <systemc>
#include <tlm>

SC_MODULE(prod)
{
  sc_core::sc_port<tlm::tlm_fifo_put_if<int>> out;

  SC_CTOR(prod)
  {
    SC_THREAD(proc);
  }

  void proc()
  {
    while (true)
    {

      if (out->nb_can_put())
      {
        out->nb_put(cnt);
        std::cout << name() << " put: " << cnt << std::endl;
        cnt++;
      }

      wait(sc_core::SC_ZERO_TIME);

      if (cnt == 20)
        sc_core::sc_stop();
    }
  }

private:
  int cnt{10};
};

SC_MODULE(cons)
{
  sc_core::sc_port<tlm::tlm_fifo_get_if<int>> in;

  SC_CTOR(cons)
  {
    SC_THREAD(proc);
  }

  void proc()
  {
    while (true)
    {
      int resp;
      if (in->nb_peek(resp))
      {
        in->nb_get(resp);
        std::cout << name() << " get: " << resp << std::endl;
      }

      wait(sc_core::SC_ZERO_TIME);
    }
  }
};

int sc_main(int argc, char *argv[])
{
  prod p("producer");
  cons c("consumer");

  tlm::tlm_fifo<int> fifo("fifo");

  p.out(fifo);
  c.in(fifo);

  sc_core::sc_start();

  return 0;
}
