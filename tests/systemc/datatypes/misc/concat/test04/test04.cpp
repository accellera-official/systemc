// test of l-value concatenation

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_bv<2> bv2a;
    sc_lv<2> lv2b;
    const sc_bv<2> bv2c( "10" );
    const sc_bv<2> bv2d( "01" );
    sc_bv<4> bv4a;

    ( bv2a, lv2b ) = "0110";
    cout << "bv2a = " << bv2a << endl;
    cout << "lv2b = " << lv2b << endl;

    bv4a = ( bv2a, lv2b );
    cout << "bv4a = " << bv4a << endl;

    bv4a = ( bv2c, bv2d );
    cout << "bv4a = " << bv4a << endl;

    ( bv4a.range( 2, 1 ), bv2a ) = "0110";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

    ( bv2a, bv4a.range( 1, 2 ) ) = "0101";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

    ( ( bv2a, lv2b ), bv4a ) = "00110011";
    cout << "bv2a = " << bv2a << endl;
    cout << "lv2b = " << lv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    ( bv4a, ( bv2a, lv2b ) ) = "11001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "lv2b = " << lv2b << endl;

    ( ( bv2a, lv2b ), bv4a.range( 2, 1 ) ) = "001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "lv2b = " << lv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    ( bv4a.range( 2, 1 ), ( bv2a, lv2b ) ) = "001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "lv2b = " << lv2b << endl;

    return 0;
}
