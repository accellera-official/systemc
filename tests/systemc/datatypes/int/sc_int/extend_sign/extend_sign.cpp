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

/*****************************************************************************

  extend_sign.cpp -- Test sign extension of sc_int in construction/assignment

  Original Author: Philipp A. Hartmann, Intel Corp.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc>
#include <iomanip>

template<typename T>
static inline void do_print(const char* nm, const T& v) {
  std::cout << nm << " = " << std::left
            << std::setw(10) << std::dec << std::noshowbase << v
            << " | " << std::setw(11) << std::hex << std::showbase << v
            << " | " << v.to_string(sc_dt::SC_BIN)
            << std::endl;
}
#define PRINT(v) \
  do_print( SC_STRINGIFY_HELPER_(v), v )

int sc_main( int ac, char *av[] )
{
  using sc_dt::sc_int;
  using sc_dt::sc_uint;

  sc_int<4> i04 = (1<<4); // construction out of range
  PRINT(i04);
  sc_assert( i04 == 0 );
  i04 = (1<<3);           // overflow, expect negative
  PRINT(i04);
  sc_assert( i04 == -8 );

  sc_uint<4> u04( i04 );  // clipping of sign bits
  PRINT(u04);
  sc_assert( u04 == 8 );
  sc_int<8>  i08 = i04;   // sign extension to new width
  PRINT(i08);
  sc_assert( i08 == -8 );
  i08 = u04;              // no sign extension
  PRINT(i08);
  sc_assert( i08 == u04 );
  sc_assert( i08 == 8 );

  // "0" bits above MSB, remove during sign extension
  sc_uint<32> u32 = ~((1u<<24) + (1u<<8));
  PRINT(u32);
  sc_assert( u32 == 0xfefffeffu );
  sc_int<16>  i16( u32 ); // remve 0 bits above MSB
  PRINT(i16);
  sc_assert( i16 == -257 );

  std::cout << "OK" << std::endl;
  return 0;
}
