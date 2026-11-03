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

//
// tlm_fifo_poke.cpp --
//
// Check the index semantics of tlm_fifo<T>::nb_peek(T&,int) and
// tlm_fifo<T>::nb_poke(const T&,int) against IEEE Std 1666-2023, 17.3.3:
// position 0 holds the item that will next be retrieved by get or nb_get,
// and position used()-1 holds the item most recently inserted.
//

#include <systemc>
#include <tlm>

#include <iostream>

using namespace sc_core;
using namespace tlm;

static int errors = 0;

static void check(int got, int expected, const char* what)
{
  std::cout << what << " = " << got << " (expected " << expected << ")"
            << (got == expected ? "" : "  <-- MISMATCH") << std::endl;
  if (got != expected) ++errors;
}

int sc_main(int, char*[])
{
  tlm_fifo<int> fifo("fifo", 8);

  // Items inserted in the current evaluation phase shall not be visible
  // to nb_peek or nb_poke before the next update phase (17.4.6).
  for (int i = 1; i <= 8; ++i) fifo.nb_put(i);
  check(fifo.used(), 0, "used() before update");
  check(fifo.nb_poke(99, 0), 0, "nb_poke(0) before update");

  sc_start(SC_ZERO_TIME);
  check(fifo.used(), 8, "used() after update");

  // Full fifo: the oldest item is at position 0.
  int v = 0;
  int ok = fifo.nb_peek(v, 0); check(ok, 1, "nb_peek(0)");
  check(v, 1, "peek(0)");
  ok = fifo.nb_peek(v, 7); check(ok, 1, "nb_peek(7)");
  check(v, 8, "peek(7)");

  // Move the read and the write position around the end of the buffer.
  for (int i = 1; i <= 6; ++i) {
    int got = 0;
    fifo.nb_get(got);
    check(got, i, "nb_get()");
  }
  sc_start(SC_ZERO_TIME);
  check(fifo.used(), 2, "used() after six reads");

  fifo.nb_put(9);
  sc_start(SC_ZERO_TIME);
  check(fifo.used(), 3, "used() after one write");

  // Items 7, 8, 9 are now stored at buffer positions 6, 7, 0.
  ok = fifo.nb_peek(v, 0); check(ok, 1, "nb_peek(0)");
  check(v, 7, "peek(0)");
  ok = fifo.nb_peek(v, 2); check(ok, 1, "nb_peek(2)");
  check(v, 9, "peek(2)");

  // nb_poke(n) shall overwrite the item that nb_peek(n) returns.
  ok = fifo.nb_poke(70, 0); check(ok, 1, "nb_poke(0)");
  ok = fifo.nb_peek(v, 0); check(ok, 1, "nb_peek(0) after nb_poke(0)");
  check(v, 70, "peek(0) after nb_poke(0)");
  ok = fifo.nb_peek(v, 1); check(ok, 1, "nb_peek(1) after nb_poke(0)");
  check(v, 8, "peek(1) after nb_poke(0)");

  // Position used()-1 holds the item most recently inserted.
  ok = fifo.nb_poke(90, fifo.used() - 1); check(ok, 1, "nb_poke(used()-1)");
  ok = fifo.nb_peek(v, 2); check(ok, 1, "nb_peek(2) after nb_poke(2)");
  check(v, 90, "peek(2) after nb_poke(2)");

  // Poking shall not change used() and shall be visible to nb_get.
  check(fifo.used(), 3, "used() after nb_poke");
  int got = 0;
  ok = fifo.nb_get(got); check(ok, 1, "nb_get()");
  check(got, 70, "nb_get() value");

  // Out of range positions are rejected.
  check(fifo.nb_poke(0, fifo.used()), 0, "nb_poke(used())");
  check(fifo.nb_poke(0, -1), 0, "nb_poke(-1)");

  std::cout << "tlm_fifo_poke: " << (errors ? "FAILED" : "PASSED") << std::endl;
  return errors ? 1 : 0;
}
