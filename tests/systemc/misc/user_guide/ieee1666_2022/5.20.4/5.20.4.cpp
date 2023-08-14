//  COMPLETE TEST
//
//  If the execution log matches the golden log the example works.
#include <systemc.h>
using namespace sc_core;


int sc_main( int argc, char* argv[] ) { 

    sc_signal<int> sig;

    sc_attribute<int> a("number",42);
    sig.add_attribute(a);

    sc_attr_cltn& c = sig.attr_cltn();
    for ( sc_attr_cltn::iterator i = c.begin(); i < c.end(); ++i ) {
        sc_attribute<int>* ap = dynamic_cast<sc_attribute<int>*>(*i);
	if ( ap ) {
	    std::cout << ap->name() << "  " << ap->value << std::endl;
	}
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
