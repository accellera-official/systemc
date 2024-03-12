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

  test10.cpp -- Test sc_module::set_stack_size

  Original Author: Andy Goodrich, Forte Design Systemc, Inc. 2003-10-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#define STACK_SIZE  0x600000

// The #if bracketed code below will work with quick threads

#if 0
#include "sysc/kernel/sc_thread_process.h"
#include "sysc/kernel/sc_cor_qt.h"


void my_dump( size_t value )
{
    std::cout << "0x" << std::hex << value << " (" << std::dec << value << ") = ";
}

void* thread_stack_location()
{
    sc_cor_qt* qt_p = (sc_cor_qt*)get_cor_pointer(sc_get_current_process_b());
    return qt_p->m_stack;
}

void* thread_stack_pointer()
{
    sc_cor_qt* qt_p = (sc_cor_qt*)get_cor_pointer(sc_get_current_process_b());
    return qt_p->m_sp;
}

size_t thread_stack_size()
{
    sc_cor_qt* qt_p = (sc_cor_qt*)get_cor_pointer(sc_get_current_process_b());
    return qt_p->m_stack_size;
}

void qt_stack_info()
{
    size_t stack_start = (size_t)thread_stack_location();
    size_t sp = (size_t)thread_stack_pointer();
    size_t stack_end = stack_start + STACK_SIZE;

    my_dump( thread_stack_size() );    std::cout << "stack size " << std::endl;
    my_dump( stack_start );            std::cout << "stack start " << std::endl;
    my_dump( stack_end );              std::cout << "stack end" << std::endl;
    my_dump( sp );                     std::cout << "m_sp " << std::endl;
    my_dump( stack_end - sp );         std::cout << "stack_end - m_sp " << std::endl;
    my_dump( sp - stack_start );       std::cout << "m_sp - start" << std::endl;
}
#endif

SC_MODULE(A)
{
  SC_CTOR(A)
  {
    SC_THREAD(thread);
    sensitive << m_clk;
    set_stack_size(STACK_SIZE);
  }

  void thread()
  {
    int sum = 0;
    int x[1];
    size_t stack_end = (size_t)x;
    size_t stack_start = (size_t)stack_end - 0x300000;

#if 0
    std::cout << "stack start at 0x" << std::hex << stack_start << std::endl;
    std::cout << "stack end at 0x" << std::hex << stack_end << std::endl;
#endif

    for ( size_t stack_p = stack_start + 0x1000; stack_p < stack_end; stack_p += 0x1000 ) {
#if 0
    std::cout << "access at 0x" << std::hex << stack_p
              << std::hex << " " << (stack_end - stack_p) << " "
              << *(int*)stack_p << std::endl;
#else
    sum = sum + *(int*)stack_p;
#endif
    }
    std::cout << sum << std::endl;
  }
  sc_in_clk m_clk;
};


int sc_main(int argc, char* argv[])
{
  sc_clock clock;
  A        a("a");
  a.m_clk(clock);

  std::cout << "stack size = 0x" << std::hex << STACK_SIZE << " (" << std::dec
            << STACK_SIZE << ")" << std::endl;

  sc_start(1, SC_NS );
  std::cout << "Program complete" << std::endl;
  return 0;
}
