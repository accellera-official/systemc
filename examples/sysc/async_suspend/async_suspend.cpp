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

  Original Author: Mark Burton, GreenSocs

 *****************************************************************************/

#include <systemc.h>
#include "node.h"
#include <vector>

#define NODES 10

int sc_main(int argc, char **argv)
{
    collector col;
    srand(0); // try to keep things as determanistic as possible....

    tlm_utils::tlm_quantumkeeper::set_global_quantum(sc_core::sc_time(1000, SC_NS));

    sc_vector<asynctestnode> nodes("nodes", NODES, [&](const char *n, size_t i) { return new asynctestnode(n, col); });

    for (int n = 0; n < NODES; n++)
    {
        for (int nn = 0; nn < NODES; nn++)
        {
            if (n != nn)
                nodes[n].init_socket(nodes[nn].target_socket);
        }
    }

    sc_start(sc_time(10000, SC_NS));

    col.report();

    return 0;
}