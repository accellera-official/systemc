#include "tlm.h"

using namespace std;

int main ( void )
{
    // built-in types
    
    char		b1 = -1;
    unsigned char	b2 = -1;
    short		b3 = -1;
    unsigned short	b4 = -1;
    long		b5 = -1;
    unsigned long	b6 = -1;
    
    cout << endl << "built-in types" << endl << endl;
    
    cout << " char  " << sizeof (char) 		 << " " << (int)b1 << endl;
    cout << "uchar  " << sizeof (unsigned char)  << " " << (unsigned int)b2 << endl;
    cout << " short " << sizeof (short)		 << " " << b3 << endl;
    cout << "ushort " << sizeof (unsigned short) << " " << b4 << endl;
    cout << " long  " << sizeof (long) 		 << " " << b5 << endl;
    cout << "ulong  " << sizeof (unsigned long)  << " " << b6 << endl;
    
    // sc_dt types
    
    cout << endl << "sc_dt types" << endl << endl;
    
    sc_dt::int64  s1 = -1;
    sc_dt::uint64 s2 = -1;
    
    cout << "sc_dt:: int64_t " << sizeof ( sc_dt::int64) << " " << s1 << endl;
    cout << "sc_dt::uint64_t " << sizeof (sc_dt::uint64) << " " << s2 << endl;
    
    // <stdint.h> types
    
    cout << endl << "<stdint.h> types" << endl << endl;
    
    int8_t  v1 = -1;
    uint8_t  v2 = -1;
    int16_t v3 = -1;
    uint16_t v4 = -1;
    int32_t v5 = -1;
    uint32_t v6 = -1;
    int64_t v7 = -1;
    uint64_t v8 = -1;
    
    cout << "  int8_t " << sizeof (  int8_t) << " " << (int)v1 << endl;
    cout << " uint8_t " << sizeof ( uint8_t) << " " << (unsigned int)v2 << endl;
    cout << " int16_t " << sizeof ( int16_t) << " " << v3 << endl;
    cout << "uint16_t " << sizeof (uint16_t) << " " << v4 << endl;
    cout << " int32_t " << sizeof ( int32_t) << " " << v5 << endl;
    cout << "uint32_t " << sizeof (uint32_t) << " " << v6 << endl;
    cout << " int64_t " << sizeof ( int64_t) << " " << v7 << endl;
    cout << "uint64_t " << sizeof (uint64_t) << " " << v8 << endl;
}



