/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <vector>
using std::cout;
using std::endl;
using std::hex;
using std::dec;
#include "systemc.h"

#if !defined(SC_VECTOR_UTILS_H)
  
int sc_main( int argc, char* argv[] )
{
    cout << endl << "The vector utiliies package is not implemented in this systemc library" 
         << endl << endl;
    return 0;
}

#else

void test_negative_trims()
{
    int            hod;
    sc_bigint<132> result;
    sc_bigint<128> x;
    sc_bigint<2>   y;

    y = 1;

    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | ~0u;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 0 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }

    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0xc0000000u;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 0 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }


    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0x80000000u;
    x = (x << 32) | 16;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 1 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }


    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0x7fffffffu;
    x = (x << 32) | 0;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 2 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }


    x = ~0u;
    x = (x << 32) | 0;
    x = (x << 32) | 0;
    x = (x << 32) | 16;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 3 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }

}

void test_positive_trims()
{
    int            hod;
    sc_bigint<130> result;
    sc_bigint<128> x;
    sc_bigint<2>   y;

    y = 1;

    x = 0;
    x = (x << 32) | 0;
    x = (x << 32) | 0;
    x = (x << 32) | 0x7fffffffu;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 0 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }


    x = 0;
    x = (x << 32) | 0;
    x = (x << 32) | 0;
    x = (x << 32) | 0x80000000u;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 1 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }


    x = 0;
    x = (x << 32) | 0;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0;
    hod = sc_dt::vector_find_significant_hod( 3, x.get_raw() );
    if ( 2 != hod ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected 0" << endl;
	cout << "  actual   " << hod << endl;
    }
    result = x * y;
    if ( result != x ) {
        cout << __FILE__ << ":" << __LINE__ << " : ERROR " << endl;
	cout << "  value    " << hex << x << dec << endl;
	cout << "  expected " << x << endl;
	cout << "  actual   " << result << endl;
    }

}

int sc_main(int argc, char* argv[])
{
    test_negative_trims();
    test_positive_trims();

    cout << "Vector trim testing completed" << endl;
    return 0;
}

#endif // #if !defined(SC_VECTOR_UTILS_H)
