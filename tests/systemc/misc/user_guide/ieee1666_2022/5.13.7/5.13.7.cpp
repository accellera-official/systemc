// THERE IS A WARMING ABOUT HIDING sc_object::print() by Chan::print()

#include <systemc.h>

struct i_f : virtual sc_core::sc_interface {
    virtual void print() = 0;
};

struct Chan : sc_core::sc_channel, i_f {
    SC_CTOR(Chan) {}
    virtual void print() { std::cout << "I'm Chan.name=" << name() << std::endl; }
};

struct Caller : sc_core::sc_module {
    sc_core::sc_port<i_f> p;
    SC_CTOR(Caller) { }
};

struct Bottom : sc_core::sc_module {
    sc_core::sc_export<i_f> xp;
    Chan ch;
    SC_CTOR(Bottom) : xp("xp"), ch("ch") {
        xp.bind(ch);
    }
};

struct Middle : sc_core::sc_module {
    sc_core::sc_export<i_f> xp;
    Bottom* b_p;
    SC_CTOR(Middle) : xp("xp") {
        b_p = new Bottom("b");
	xp.bind(b_p->xp);
	b_p->xp->print();
    }
};

struct Top : sc_core::sc_module {
    Caller* c;
    Middle* m;
    SC_CTOR(Top) {
        c = new Caller("c");
	m = new Middle("m");
	c->p(m->xp);
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    std::cout << "program completed" << std::endl;
}
