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

  test03.cpp -- Test bit constant initializers.

  Original Author: Andy Goodrich, Accelera, 2024-06-22

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#define TRY(TYPE,TARGET) \
    {   \
	std::cout << "    " << #TYPE << "var(" << #TARGET << ")"; \
        try { \
            TYPE var(TARGET); \
	    std::cout << " is mapped to " << var << std::endl; \
	} \
	catch( const sc_core::sc_report & )\
	{\
	    std::cout << " IS NOT LEGAL!!!" << std::endl; \
	}\
    }

int sc_main(int argc, char* argv[])
{
    // Legal sc_bv values:

    std::cout << std::endl << "Legal sc_bv<W> initialization values: " 
              << std::endl << std::endl;
    TRY(sc_bv<10>, "0bsm11101");
    TRY(sc_bv<5>, "0bus11101");
    TRY(sc_bv<15>, "0d245");
    TRY(sc_bv<5>, "01011");
    TRY(sc_bv<12>, 1366);
    TRY(sc_bv<12>, 2366);
    TRY(sc_bv<8>,"0x61");
    TRY(sc_bv<8>,"0x66");

    // Illegal sc_bv values:

    std::cout << std::endl << "Illegal sc_bv<W> initialization values: " 
              << std::endl << std::endl;
    TRY(sc_bv<5>, "0b0011x");
    TRY(sc_bv<5>, "0bsm1110z");
    TRY(sc_bv<5>, "0bus1110x");
    TRY(sc_bv<5>, "01xz1");

    // Legal sc_lv values:

    std::cout << std::endl << "Legal sc_lv<W> initialization values: " 
              << std::endl << std::endl;
    TRY(sc_lv<5>, "01xz0");
    TRY(sc_lv<5>, "11x01");
    TRY(sc_lv<5>, "0b1x011");
    TRY(sc_lv<5>, "0bsm1101");
    TRY(sc_lv<5>, "0bxxxxx");
    TRY(sc_lv<5>, "0b0zzxx");
    TRY(sc_lv<5>, "XXXX");
    TRY(sc_lv<12>, 0366);
    TRY(sc_lv<12>, 1366);
    TRY(sc_lv<15>, 2366);
    TRY(sc_lv<8>,"0x61");
    TRY(sc_lv<8>,"0x66");
    TRY(sc_lv<15>, "0d245");

    // Illegal sc_lv values:

    std::cout << std::endl << "Illegal sc_lv<W> initialization values: " 
              << std::endl << std::endl;
    TRY(sc_lv<5>, "00x66");
    TRY(sc_lv<5>, "01366");
    TRY(sc_lv<5>, "01x66");
    TRY(sc_lv<5>, "0b1junk");
    TRY(sc_lv<5>, "0bsm1x01");
    TRY(sc_lv<5>, "23456");

    std::cout << "Program completed" << std::endl;
    return 0;
}
