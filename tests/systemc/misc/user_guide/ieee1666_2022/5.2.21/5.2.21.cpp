#include <systemc.h>


SC_MODULE(Top)
{
    SC_CTOR(Top) { }
    sc_core::sc_signal<int>  SC_NAMED(A);
    sc_core::sc_signal<bool> SC_NAMED(B);
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    std::vector<sc_core::sc_object*> children = top.get_child_objects();
    for ( size_t i = 0; i < children.size(); ++i ) {
        std::cout << children[i]->name() << " " << children[i]->kind() << std::endl;
    }

    std::cout << "program completed" << std::endl;
}
