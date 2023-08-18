// COMPLETE TEST
//
// If the log file matches the golden log the insertion of an attribute was successful.

#include <systemc>

int sc_main( int argc, char* argv[] ) { 
    using namespace sc_core;

    sc_signal<int> sig;
    sc_attribute<int> a("number", 1);
    sig.add_attribute(a);
    sc_attribute<int>* ap;
    ap = (sc_attribute<int>*)sig.get_attribute("number");
    ++ap->value;
    ap = (sc_attribute<int>*)sig.get_attribute("number");
    std::cout << "attribute is " << ap->value << std::endl;

    std::cout << "program completed" << std::endl;
    return 0;
}
