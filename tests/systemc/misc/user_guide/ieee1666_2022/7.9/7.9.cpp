// COMPLETE TEST
//
// If the execution log matches the golden log this example compiled and
// executed correctly.

#include <systemc.h>

#define SHOW(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ") = " << X.to_string(FMT) << std::endl;}
#define SHOW_FALSE(X,FMT) { \
    std::cout << #X << ".to_string(" << #FMT << ",false) = " << X.to_string(FMT,false) << std::endl;}
#define SHOW_TRUE(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ",true) = " << X.to_string(FMT,true) << std::endl;}


int sc_main( int argc, char* argv[] ) { 

    std::cout << std::endl 
              << "================ 7.9.3.6 ==================" << std::endl << std::endl;
    sc_dt::sc_bv_base b(4);
    b = "0xf";
    SHOW_FALSE(b,sc_dt::SC_BIN)
    SHOW(b,sc_dt::SC_BIN)
    std::cout << "b.to_string() = " << b.to_string() << std::endl;


    std::cout << std::endl 
              << "================ 7.9.4.6 ==================" << std::endl << std::endl;
    sc_dt::sc_bv_base l(4);
    l = "0xf";
    SHOW_FALSE(l,sc_dt::SC_BIN)
    SHOW(l,sc_dt::SC_BIN)
    std::cout << "l.to_string() = " << l.to_string() << std::endl;

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
