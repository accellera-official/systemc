/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  reset_signal_is.cpp -- This example shows the use of the method
             sc_module::reset_signal_is() to provide reset semantics for 
             a clocked thread (SC_CTHREAD). 

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h" 
SC_MODULE(CONSUMER)
{
	SC_CTOR(CONSUMER)
	{
		SC_CTHREAD(consumer,m_clk.pos());
		reset_signal_is(m_reset,false);
	}
	void consumer()
	{
		cout << sc_time_stamp() << ": reset" << endl;
		while (1)
		{
			m_ready = true;
			do { wait(); } while ( !m_valid);
			cout << sc_time_stamp() << ": " << m_value.read() << endl;
		}
	}
	sc_in_clk       m_clk;
	sc_inout<bool>  m_ready;
	sc_in<bool>     m_reset;
	sc_in<bool>     m_valid;
	sc_in<int>      m_value;
};

SC_MODULE(PRODUCER)
{
	SC_CTOR(PRODUCER)
	{
		SC_CTHREAD(producer,m_clk.pos());
		reset_signal_is(m_reset,false);
	}
	void producer()
	{
		m_value = 0;
		while (1)
		{
			m_valid = true;
			do { wait(); } while (!m_ready);
			m_value = m_value + 1;
		}
	}
	sc_in_clk       m_clk;
	sc_in<bool>     m_ready;
	sc_in<bool>     m_reset;
	sc_inout<bool>  m_valid;
	sc_inout<int>   m_value;
};

SC_MODULE(TESTBENCH)
{
	SC_CTOR(TESTBENCH) : m_consumer("consumer"), m_producer("producer")
	{
		SC_CTHREAD(testbench,m_clk.pos());
		m_consumer.m_clk(m_clk);
		m_consumer.m_ready(m_ready);
		m_consumer.m_reset(m_reset);
		m_consumer.m_valid(m_valid);
		m_consumer.m_value(m_value);
		m_producer.m_clk(m_clk);
		m_producer.m_ready(m_ready);
		m_producer.m_reset(m_reset);
		m_producer.m_valid(m_valid);
		m_producer.m_value(m_value);
	}
	void testbench()
	{
		for ( int state = 0; state < 100; state++ )
		{
			m_reset = ( state%20 == 12 ) ? false : true;
			wait();
		}
		sc_stop();
	}
	sc_in_clk       m_clk;
	CONSUMER        m_consumer;
	PRODUCER        m_producer;
	sc_signal<bool> m_ready;
	sc_signal<bool> m_reset;
	sc_signal<bool> m_valid;
	sc_signal<int>  m_value;
};

int sc_main(int, char*[])
{
	sc_clock  clock;
	TESTBENCH testbench("testbench");

	testbench.m_clk(clock);
	sc_start(5000, SC_NS);
	cout << "Done" << endl;
	return 0;
}

