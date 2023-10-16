// SCREW BALL EXAMPLE
//
// This example does not make sense to me:
//
// We have the following SC_MODULE skeleton:
//
// SC_MODULE(M) {
//     sc_core::sc_signal<bool> clk;
//
//     .  .  .
//
//     void thread_process() {
//         for (;;) {
//             if ( clk.posedge() ) {
//                 wait( clk.negedge_event) );
//                 .  .  .
//             }
//         }
//      }
// };
//
// I don't see how we avoid an infinite loop when the wait on the negedge_event is satified,
// as we then go to the top of the for loop and the if check on the posedge fails. 
// This might be side-stepped by an additional wait for the posedge_event in the .  .  .
// portion of the if, but it seems like that should be shown in the example.
//
// @@@@ ISSUES @@@@


#include <systemc>

int sc_main( int argc, char* argv[] ) { 

    std::cout << "program completed" << std::endl;
    return 0;
}
