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

// Native integer extension in sc_biguint part-select concatenations.

#include <systemc>

using std::cout;
using std::endl;

template <class T>
void test_extension( T src )
{
    sc_dt::sc_biguint<134> extended = src;

    for( int width = 63; width <= 65; ++width ) {
        for( int fill = 0; fill <= 1; ++fill ) {
            sc_dt::sc_biguint<72> high = fill ? -1 : 0;
            sc_dt::sc_biguint<65> low = fill ? -1 : 0;
            sc_dt::sc_biguint<72> expected_high = high;
            sc_dt::sc_biguint<65> expected_low = low;

            // Direct range assignment provides the expected extension and
            // preserves the bits outside the selected ranges.
            expected_high.range(70, 2) = extended.range(width + 68, width);
            expected_low.range(width - 1, 0) = extended.range(width - 1, 0);

            (high.range(70, 2), low.range(width - 1, 0)) = src;
            sc_assert( high == expected_high );
            sc_assert( low == expected_low );
        }
    }
}

int
sc_main( int, char*[] )
{
    sc_dt::sc_biguint<65> x = 0;
    (x.range(64, 64), x.range(63, 0)) = 0;
    sc_assert( x == 0 );

    test_extension( 0 );
    test_extension( 42 );
    test_extension( -1 );
    test_extension( sc_dt::int64(-2) );
    test_extension( sc_dt::uint64(0) );
    test_extension( ~sc_dt::uint64(0) );

    cout << "Native integer concatenation extension passed" << endl;
    return 0;
}
