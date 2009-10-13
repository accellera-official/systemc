// test of l-value concatenation

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_bv<2> bv2a;
    sc_bv<2> bv2b;
    const sc_bv<2> bv2c( "10" );
    const sc_bv<2> bv2d( "01" );
    sc_bv<4> bv4a;
    sc_bv<4> bv4b( "1111" );

    cout << "( bv2a, bv2b ) = \"0110\"" << endl;
    ( bv2a, bv2b ) = "0110";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    cout << "bv4a = ( bv2a, bv2b )" << endl;
    bv4a = ( bv2a, bv2b );
    cout << "bv4a = " << bv4a << endl;

    cout << "bv4a = ( bv2c, bv2d )" << endl;
    bv4a = ( bv2c, bv2d );
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a.range( 2, 1 ), bv2a ) = \"0110\"" << endl;
    ( bv4a.range( 2, 1 ), bv2a ) = "0110";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;

#if 0
    cout << "( bv2a, bv4a.range( 1, 2 ) ) = \"0101\"" << endl;
    ( bv2a, bv4a.range( 1, 2 ) ) = "0101";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
#endif // 0

    cout << "( bv4a.range( 2, 1 ), bv4b.range( 2, 1 ) ) = \"1100\"" << endl;
    ( bv4a.range( 2, 1 ), bv4b.range( 2, 1 ) ) = "1100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    cout << "( ( bv2a, bv2b ), bv4a ) = \"00110011\"" << endl;
    ( ( bv2a, bv2b ), bv4a ) = "00110011";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a, ( bv2a, bv2b ) ) = \"11001100\"" << endl;
    ( bv4a, ( bv2a, bv2b ) ) = "11001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    cout << "( ( bv2a, bv2b ), ( bv4a, bv4b ) ) = \"011000001111\"" << endl;
    ( ( bv2a, bv2b ), ( bv4a, bv4b ) ) = "011000001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;
    cout << "bv4b = " << bv4b << endl;

    cout << "( ( bv2a, bv2b ), bv4a.range( 2, 1 ) ) = \"001111\"" << endl;
    ( ( bv2a, bv2b ), bv4a.range( 2, 1 ) ) = "001111";
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;
    cout << "bv4a = " << bv4a << endl;

    cout << "( bv4a.range( 2, 1 ), ( bv2a, bv2b ) ) = \"001100\"" << endl;
    ( bv4a.range( 2, 1 ), ( bv2a, bv2b ) ) = "001100";
    cout << "bv4a = " << bv4a << endl;
    cout << "bv2a = " << bv2a << endl;
    cout << "bv2b = " << bv2b << endl;

    return 0;
}
