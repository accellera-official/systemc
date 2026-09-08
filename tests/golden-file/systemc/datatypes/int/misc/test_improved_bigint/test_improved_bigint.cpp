#include "systemc.h"

using namespace std;

int sc_main( int argc, char* argv[] )
{
	sc_biguint<128> v = 0;

  // 128 bits Reference value is this:
  //                0x362e'eb34'c44f'a8fb'd37e'c3ca'1f9a'3de4;
  v.range(63,  0) =                     0xd37e'c3ca'1f9a'3de4;
  v.range(127,64) = 0x362e'eb34'c44f'a8fb;

  sc_assert(v.to_string(SC_HEX)=="0x0362eeb34c44fa8fbd37ec3ca1f9a3de4");

  v = v >> 16;
  sc_assert(v.to_string(SC_HEX)=="0x00000362eeb34c44fa8fbd37ec3ca1f9a");


  cout << "test passed." << endl;
	return 0;
}
