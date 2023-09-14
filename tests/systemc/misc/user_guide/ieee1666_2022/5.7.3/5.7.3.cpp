// COMPLETE TEST
//
// If the execution log matches the golden log this test compiles
// and executes correctly.

#define SC_INCLUDE_DYNAMIC_PROCESSE
#include <systemc>


class if_class : public sc_core::sc_interface {
  public:
    virtual const sc_core::sc_event& ev_func() const = 0;
};

class chan_class : public if_class, public sc_core::sc_prim_channel {
  public:
    virtual const sc_core::sc_event& ev_func() const { return an_event; }
  private:
    sc_core::sc_event an_event;
};

template<int N=1>
class port_class : public sc_core::sc_port<if_class,N> {
  public:
    sc_core::sc_event_finder& event_finder() const {
      return *new sc_core::sc_event_finder_t<if_class>(*this, &if_class::ev_func);
    }
};

SC_MODULE(mod_class)
{
    port_class<1> port_var;
    port_class<0> multiport;
  
    SC_CTOR(mod_class) {
        SC_METHOD(method);
        sensitive << port_var.event_finder();
    }

    void method() { std::cout << "method" << std::endl; }

    void end_of_elaboration() {
        SC_METHOD(method2);
	for ( size_t i = 0; i < multiport.size(); ++i ) {
	    sensitive << multiport[i]->ev_func();
	}
    }

    void method2() { std::cout << "method2" << std::endl; }
};
        

int sc_main( int argc, char* argv[] ) { 

    mod_class  mc("mc");
    chan_class sig0;
    chan_class sig1;
    chan_class sig2;

    mc.port_var(sig0);
    mc.multiport(sig1);
    mc.multiport(sig2);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    std::cout << "program completed" << std::endl;
    return 0;
}
