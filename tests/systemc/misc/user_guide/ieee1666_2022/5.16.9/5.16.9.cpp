// NEEDS WORK:
//
// THE LRM EXAMPLE HAS ISSUES:
//
// (1) SomeModule::in is a declared a reference and needs to be an instance.
// (2) ~sc_prim_channle is a protected method.

#include <systemc.h>

SC_MODULE(SignalStub) {
    SC_CTOR(SignalStub) {}
    ~SignalStub() {
        for ( auto o : get_child_objects() ) {
	    std::cout << o->name() << endl;
	    // delete dynamic_cast<sc_core::sc_prim_channel*>(o);
	}
    }

    template<typename T>
    void stub( sc_core::sc_in<T>& port) {
        sc_core::sc_hierarchy_scope scope = get_hierarchy_scope();
	sc_core::sc_signal<T>* sig;
	sig = new sc_core::sc_signal<T>( sc_core::sc_gen_unique_name(port.basename()) );
	port.bind(*sig);
    }
};

SC_MODULE(SomeModule) {
    sc_core::sc_in<int> in;
    SC_CTOR(SomeModule) : in("in")  { }
};

int sc_main( int argc, char* argv[] ) { 

    SignalStub SC_NAMED(sigStub);
    SomeModule SC_NAMED(top);
    sigStub.stub(top.in);

    std::cout << "program completed" << std::endl;
    return 0;
}
