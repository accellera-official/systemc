// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and
// executes correctly.

#include <systemc.h>

#define SHOW(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ") = " << X.to_string(FMT) << std::endl;}
#define SHOW_FALSE(X,FMT) { \
    std::cout << #X << ".to_string(" << #FMT << ",false) = " << X.to_string(FMT,false) << std::endl;}
#define SHOW_TRUE(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ",true) = " << X.to_string(FMT,true) << std::endl;}

int sc_main( int argc, char* argv[] ) { 


    std::cout << std::endl; 
    sc_dt::sc_int<4> v1;
    v1 = "0b10100";
    SHOW_TRUE(v1,sc_dt::SC_BIN);

    std::cout << std::endl; 
    sc_dt::sc_int<10> v2;
    v2 = "0d478";
    SHOW_FALSE(v2,sc_dt::SC_CSD);

    std::cout << std::endl; 
    sc_dt::sc_uint<8> v3;
    v3 = "0x7";
    SHOW(v3,sc_dt::SC_HEX);

    std::cout << std::endl; 
    sc_dt::sc_lv<16> lv;
    lv = "0xff";
    SHOW(lv,sc_dt::SC_HEX);

    std::cout << std::endl; 
    sc_dt::sc_bv<8> bv;
    bv = "11110000";
    SHOW(bv,sc_dt::SC_BIN);

    std::cout << "program completed" << std::endl;
    return 0;
}
