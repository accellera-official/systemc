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

  6.5.3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// SCREW BALL EXAMPLE
//
// This example does not make sense to me:
//
// We have the following SC_MODULE skeleton:
//
// SC_MODULE(M) {
//     sc_core::sc_signal<bool> clk;
//
//     .  .  .
//
//     void thread_process() {
//         for (;;) {
//             if ( clk.posedge() ) {
//                 wait( clk.negedge_event) );
//                 .  .  .
//             }
//         }
//      }
// };
//
// I don't see how we avoid an infinite loop when the wait on the negedge_event is satified,
// as we then go to the top of the for loop and the if check on the posedge fails. 
// This might be side-stepped by an additional wait for the posedge_event in the .  .  .
// portion of the if, but it seems like that should be shown in the example.
//
// @@@@ ISSUES @@@@


#include <systemc>

int sc_main( int argc, char* argv[] ) { 

    std::cout << "program completed" << std::endl;
    return 0;
}
