#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

int f() { return 0; }

struct Functor {
    typedef int result_type;
    result_type operator() ();
};

Functor::result_type Functor::operator() () { return f(); }

int h(int a, int& b, const int& c);

struct MyMod : sc_core::sc_module {
    sc_core::sc_signal<int> SC_NAMED(sig);
    void g();

    SC_CTOR(MyMod) {
        SC_THREAD(T);
    }

    int ret;

    void T() {
        using namespace sc_core;

	sc_spawn(&f);

	sc_process_handle handle = sc_spawn(&f);

	Functor fr;
	sc_spawn(&ret, fr);

	sc_spawn_options opt;
	    opt.spawn_method();
	    opt.set_sensitivity(&sig);
	    opt.dont_initialize();
        sc_spawn(f, "f1", &opt);
	sc_spawn(&ret,fr,"f2",&opt);
	sc_spawn(sc_bind(&MyMod::g,this));

	int A=0, B, C;
	sc_spawn(&ret,sc_bind(&h,A,sc_ref(B),sc_cref(C)));
    }
};
        

int sc_main( int argc, char* argv[] ) { 

    std::cout << "program completed" << std::endl;
}
