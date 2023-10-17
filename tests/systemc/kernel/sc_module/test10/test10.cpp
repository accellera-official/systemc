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

#define STACK_SIZE 0x600000 
#define ARRAY_SIZE ((long)(STACK_SIZE/4.79267))

void my_dump( const long value )
{       
    std::cout << "0x" << std::hex << value << " (" << std::dec << value << ") = ";
}  

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
	    int  x[ARRAY_SIZE];    // Grab a lot of stack...

	    for (;;) 
	    {
		// modify the first and last locations:
		x[ARRAY_SIZE-1] = ~x[ARRAY_SIZE-1];
		x[0] = ~x[0];
		std::cout << sc_time_stamp() <<std:: endl;
		wait();
	    }
	}
	sc_in_clk m_clk;
};

int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	A        a("a");
	a.m_clk(clock);

        my_dump(STACK_SIZE); std::cout << "stack size" << std::endl;
        my_dump(4*ARRAY_SIZE); std::cout << "array size" << std::endl;
        my_dump( (STACK_SIZE-(4*ARRAY_SIZE)) ); std::cout << "stack margin" << std::endl;

	sc_start(2, SC_NS);
        std::cout << "Program complete" << std::endl;
	return 0;
}
