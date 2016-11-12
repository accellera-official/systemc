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

  main.cpp -- This example shows the use of the sc_ttd classes to demonstrate
              a communication channel that uses a toggle-toggle handshake.

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

// $Log: main.cpp,v $
// Revision 1.2  2011/08/15 16:43:24  acg
//  Torsten Maehne: changes to remove unused argument warnings.
//
// Revision 1.1  2011/06/14 21:25:39  acg
//  Andy Goodrich: moved examples from 2.2.1 potential release.
//
// Revision 1.1  2010/08/20 14:14:10  acg
//  Andy Goodrich: new example using a toggle-toggle handshake for communication.
//

#include "systemc.h"
#include <iomanip>
#include "sc_ttd.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_CTHREAD(thread,m_clk.pos());
        reset_signal_is(m_reset, false);
    }
    void thread()
    {
        sc_uint<8> data[10];

        m_input.reset();
        m_output.reset();
	wait();
	for (;;)
	{
	    for ( int outer_i = 0; outer_i < 10; outer_i++ )
	    {
		for ( int inner_i = 0; inner_i < outer_i; inner_i++ )
		{
		    data[inner_i] = m_input.read();
		    cout << "         " << std::setw(3) << data[inner_i] 
			 << "          " << sc_time_stamp() << endl;
		}
		for ( int inner_i = 0; inner_i < outer_i; inner_i++ )
		{
		    m_output = data[inner_i];
		}
	    }
	}
    }
    sc_in<bool>              m_clk;
    sc_ttd<sc_uint<8> >::in  m_input;
    sc_ttd<sc_uint<8> >::out m_output;
    sc_in<bool>              m_reset;
};

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_CTHREAD(consumer,m_clk.pos());
        reset_signal_is(m_reset, false);
        SC_CTHREAD(producer,m_clk.pos());
        reset_signal_is(m_reset, false);
    }

    void consumer()
    {
	sc_uint<8> data;

        m_from_dut.reset();
	wait();

        for ( int i = 0; i < 40; i++ )
        {
	    data = m_from_dut.read();
	    cout << "               " << std::setw(3) << data << "    "
	         << sc_time_stamp() << endl;
        }
	sc_stop();
    }

    void producer()
    {
	sc_uint<8> data;

        m_to_dut.reset();
	wait();
        for ( int i = 0;; i++ )
        {
	    cout << "  " << std::setw(3) << i << "                 "
	         << sc_time_stamp() << endl;
	    data = i;
	    m_to_dut = data;
	    if ( i && (i % 6 == 0) ) wait(i);
        }
    }

    sc_in<bool>              m_clk;
    sc_ttd<sc_uint<8> >::in  m_from_dut;
    sc_in<bool>              m_reset;
    sc_ttd<sc_uint<8> >::out m_to_dut;
};

int sc_main(int, char* [])
{
    sc_clock            clock;
    DUT                 dut("dut");
    sc_ttd<sc_uint<8> > dut_to_tb;
    sc_signal<bool>     reset;
    TB                  tb("tb");
    sc_ttd<sc_uint<8> > tb_to_dut;

    dut.m_clk(clock);
    dut.m_reset(reset);
    dut.m_input(tb_to_dut);
    dut.m_output(dut_to_tb);

    tb.m_clk(clock);
    tb.m_reset(reset);
    tb.m_from_dut(dut_to_tb);
    tb.m_to_dut(tb_to_dut);

    cout << "producer dut consumer " << endl;
    reset = false;
    sc_start(1, SC_NS);
    reset = true;
    sc_start();

    cout << "Program completed" << endl;
    return 0;
}
