#include "systemc.h"

int sc_main(int argc, char* argv[])
{

    cout << endl;

    // Test 1: shifting a negative number properly propagates the sign bit:
    {
	long long value;
        int       ulen;

        value = 0x100ull;
        ulen   = 64-9;;
        value = (value << ulen) >> ulen;
        if ( value != 0xffffffffffffff00LL ) {
            cout << "(1) Unexpected behavior for left-right shift of negative number: " << endl;
	    cout << "    Expected: 0xffffffffffffff00" << endl;
	    cout << "    got     : 0x" << hex <<value << endl;
        }
	else {
	    cout << "(1) Got expected behavior for left-right shift of negative number. " << endl;
	}
	cout << endl;
    }
    cout << "Program completed" << endl;
    return 0;
}
