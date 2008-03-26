/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  main.cpp -- This example shows the use of the sc_export class to 
              demonstrate the use of sc_export in the implementation of
              a delay pipe.

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"

// This program implements a delay pipe which passes values through a fifo
// in a specified number of clocks. A value may be inserted at each clock
// edge and it will appear as the output of the pipe after the specified
// number of clocks. The template arguments to this class are:
//    T the class specifying the value to be piped.
//    N is the number of stages in the pipe, and should be >= 2.
//
// For this example change the typedef below to the class that you want a 
// delay pipe for.

typedef sc_biguint<121> atom;  // Value to be pipe delayed.


//==============================================================================
// dpipe<T,N> - DELAY PIPELINE FOR AN ARBITRARY CLASS:
//==============================================================================
template<class T, int N>
SC_MODULE(dpipe) {
    typedef sc_export<sc_signal_inout_if<T> > in;   // To pipe port type.
    typedef sc_export<sc_signal_in_if<T> >    out;  // From pipe port type.

    SC_CTOR(dpipe)
    {
        m_in(m_pipe[0]);
        m_out(m_pipe[N-1]);
        SC_METHOD(rachet);
        sensitive << m_clk.pos();
    }

    void rachet()
    {
        for ( int i = N-1; i > 0; i-- )
        {
            m_pipe[i].write(m_pipe[i-1].read());
        }
    }

    sc_in_clk    m_clk;     // Pipe synchronization.
    in           m_in;      // Input to delay pipe.
    out          m_out;     // Output from delay pipe.
    sc_signal<T> m_pipe[N]; // Pipeline stages.
};


// Testbench reader of values from the pipe:

SC_MODULE(Reader)
{
  public:
    SC_CTOR(Reader)
    {
        SC_METHOD(extract)
        sensitive << m_clk.pos();
        dont_initialize();
    }

  protected:
    void extract()
    {
        cout << sc_time_stamp().to_double() << ": " << m_from_pipe.read() 
	     << endl;
    }

  public:
    sc_in_clk    m_clk;         // Module synchronization.
    sc_in<atom > m_from_pipe;   // Output from delay pipe.
};

            

// Testbench writer of values to the pipe:

SC_MODULE(Writer)
{
    SC_CTOR(Writer)
    {
        SC_METHOD(insert)
        sensitive << m_clk.pos();
        m_counter = 0;
    }

    void insert()
    {
        m_to_pipe.write(m_counter);
        m_counter++;
    }

    sc_in_clk       m_clk;       // Module synchronization.
    atom            m_counter;   // Write value.
    sc_inout<atom > m_to_pipe;   // Input for delay pipe.
};

// Main program

int sc_main(int argc, char* argv[])
{
    sc_clock      clock;
    dpipe<atom,4> delay("pipe");
    Reader        reader("reader");
    Writer        writer("writer");

    delay.m_clk(clock);

    reader.m_clk(clock);
    reader.m_from_pipe(delay.m_out);

    writer.m_clk(clock);
    writer.m_to_pipe(delay.m_in);

    sc_start(10, SC_NS);

    return 0;
}
