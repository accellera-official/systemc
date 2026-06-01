#include "systemc.h"

#define PREFIX cout << shifter << " shift @ file line : " << dec << __LINE__ << " "

template<typename T>
void load(T& target, int width, unsigned int value )
{
    target = value;
    for ( int load_i = 32; load_i < width; load_i+=32 ) {
       target = (target << 32) | value;
    }
}

template<typename T>
void test_signed( unsigned int value, T& shift, const std::string& shifter )
{
    sc_bigint<25>  v_bigint;
    sc_biguint<25> v_bigint_left_shift;
    sc_biguint<25> v_bigint_right_shift;
    sc_signed      v_signed_left_shift(25);
    sc_signed      v_signed_right_shift(25);
    sc_signed      v_signed;

    load(v_bigint, 25, value );
    load(v_signed, 25, value );
    shift = 3;

    v_signed_right_shift = v_bigint >> shift;
    PREFIX << hex << v_signed_right_shift << " = " << v_bigint << " >> " << dec << shift << endl;

    v_signed_left_shift = v_bigint << shift;
    PREFIX << hex << v_signed_left_shift << " = " << v_bigint << " << " << dec << shift << endl;

    v_signed_right_shift = v_bigint >> shift;
    PREFIX << hex << v_signed_right_shift << " = " << v_signed << " >> " << dec << shift << endl;

    v_signed_left_shift = v_signed << shift;
    PREFIX << hex << v_signed_left_shift << " = " << v_signed << " << " << dec << shift << endl;

    v_bigint_right_shift = v_signed;
    v_bigint_right_shift >>= (shift-1);
    PREFIX << hex << v_bigint_right_shift << " = " << v_signed << " >>= " << dec << shift << endl;

    cout << endl;

    v_bigint_left_shift = v_signed;
    v_bigint_left_shift >>= (shift-1);
    PREFIX << hex << v_bigint_left_shift << " = " << v_signed << " <<= " << dec << (shift-1)
           << endl;

    v_signed_right_shift = v_signed;
    v_signed_right_shift >>= (shift-1);
    PREFIX << hex << v_signed_right_shift << " = " << v_signed << " >>= " << dec << (shift-1)
           << endl;

    v_signed_left_shift = v_signed;
    v_signed_left_shift >>= (shift-1);
    PREFIX << hex << v_signed_left_shift << " = " << v_signed << " >>= " << dec << (shift-1)
           << endl;

    v_signed_left_shift = v_signed(24,0) << shift;
    PREFIX << hex << v_signed_left_shift << " = " << v_signed << "(24,0) << " << dec
           << shift << endl;

    v_signed_right_shift = v_signed(24,0) >> shift;
    PREFIX << hex << v_signed_right_shift << " = " << v_signed << "(24,0) >> " << dec  
           << shift << endl;
}

void test_signed_shifts( unsigned int value ) 
{
    sc_bigint<64>  v_bigint;
    sc_biguint<64> v_biguint;
    int            v_int;
    int64          v_int64;
    long           v_long;
    sc_signed      v_signed(64);
    unsigned int   v_uint;
    uint64         v_uint64;
    unsigned long  v_ulong;
    sc_unsigned    v_unsigned(64);

    cout << endl << "test_signed_shifts(" << hex << value << ")" << endl << endl;

    test_signed( value, v_bigint, "sc_bigint" );
    test_signed( value, v_biguint, "sc_biguint" );
    test_signed( value, v_int, "int" );
    test_signed( value, v_int64, "int64" );
    test_signed( value, v_long, "long" );
    test_signed( value, v_signed, "sc_signed" );
    test_signed( value, v_uint, "unsigned int" );
    test_signed( value, v_uint64, "uint64" );
    test_signed( value, v_ulong, "unsigned long" );
    test_signed( value, v_unsigned, "sc_unsigned" );
}

template<typename T>
void test_unsigned( unsigned int value, T& shift, const std::string& shifter )
{
    sc_biguint<25> v_biguint;
    sc_biguint<25> v_biguint_left_shift;
    sc_biguint<25> v_biguint_right_shift;
    sc_unsigned    v_unsigned_left_shift(25);
    sc_unsigned    v_unsigned_right_shift(25);
    sc_unsigned    v_unsigned;

    load(v_biguint, 25, value );
    load(v_unsigned, 25, value );
    shift = 3;

    v_unsigned_right_shift = v_biguint >> shift;
    PREFIX << hex << v_unsigned_right_shift << " = " << v_biguint << " >> " << dec << shift << endl;

    v_unsigned_left_shift = v_biguint << shift;
    PREFIX << hex << v_unsigned_left_shift << " = " << v_biguint << " << " << dec << shift << endl;

    v_unsigned_right_shift = v_unsigned >> shift;
    PREFIX << hex << v_unsigned_right_shift << " = " << v_unsigned << " >> " << dec << shift << endl;

    v_unsigned_left_shift = v_unsigned << shift;
    PREFIX << hex << v_unsigned_left_shift << " = " << v_unsigned << " << " << dec << shift << endl;

    cout << endl;

    v_biguint_left_shift = v_unsigned;
    v_biguint_left_shift <<= (shift-1);
    PREFIX << hex << v_biguint_left_shift << " = " << v_unsigned << " <<= " << dec << (shift-1)
           << endl;

    v_biguint_right_shift = v_unsigned;
    v_biguint_right_shift >>= (shift-1);
    PREFIX << hex << v_biguint_right_shift << " = " << v_unsigned << " >>= " << dec << (shift-1)
           <<endl;

    v_unsigned_left_shift = v_unsigned;
    v_unsigned_left_shift <<= (shift-1);
    PREFIX << hex << v_unsigned_left_shift << " = " << v_unsigned << " <<= " << dec << (shift-1)
           << endl;

    v_unsigned_right_shift = v_unsigned;
    v_unsigned_right_shift >>= (shift-1);
    PREFIX << hex << v_unsigned_right_shift << " = " << v_unsigned << " >>= " << dec << (shift-1)
           << endl;

    v_unsigned_left_shift = v_unsigned(24,0) << shift;
    PREFIX << hex << v_unsigned_left_shift << " = " << v_unsigned << "(24,0) << " << dec 
           << shift << endl;

    v_unsigned_right_shift = v_unsigned(24,0) >> shift;
    PREFIX << hex << v_unsigned_right_shift << " = " << v_unsigned << "(24,0) >> " << dec 
           << shift << endl;
}

void test_unsigned_shifts( unsigned int value ) 
{
    sc_bigint<64>  v_bigint;
    sc_biguint<64> v_biguint;
    int            v_int;
    int64          v_int64;
    long           v_long;
    sc_signed      v_signed(64);
    unsigned int   v_uint;
    uint64         v_uint64;
    unsigned long  v_ulong;
    sc_unsigned    v_unsigned(64);

    cout << endl << "test_unsigned_shifts(" << hex << value << ")" << endl << endl;

    test_unsigned( value, v_bigint, "sc_bigint" );
    test_unsigned( value, v_biguint, "sc_biguint" );
    test_unsigned( value, v_int, "int" );
    test_unsigned( value, v_int64, "int64" );
    test_unsigned( value, v_long, "long" );
    test_unsigned( value, v_signed, "sc_signed" );
    test_unsigned( value, v_uint, "unsigned int" );
    test_unsigned( value, v_uint64, "uint64" );
    test_unsigned( value, v_ulong, "unsigned long" );
    test_unsigned( value, v_unsigned, "sc_unsigned" );
}

int sc_main(int argc, char* argv[])
{
    test_signed_shifts( ~0u );
    test_signed_shifts( 0xaaaaaaaau );

    test_unsigned_shifts( ~0u );
    test_unsigned_shifts( 0xaaaaaaaau);

    return 0;
}
