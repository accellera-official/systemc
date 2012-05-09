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

  simple_fifo.cpp -- SystemC 2.0 Performance Modeling Example.

                     This example is derived from the design presented within
                     "An Introduction to System Level Modeling in SystemC 2.0"
                     By Stuart Swan, Cadence Design Systems.
                     Available at www.systemc.org

                     The system being modeled has a producer block that
                     sends characters to a consumer block via a fifo.
                     The fifo will suspend the producer or consumer as
                     necessary to insure all characters are reliably
                     delivered.

                     The consumer block will consume exactly one
                     character every 100 ns unless it is suspended
                     waiting for input from the fifo.

                     The producer block produces between one and
                     19 characters every 1000 ns unless it is
                     suspended waiting to write to the fifo.
                     On average, the producer block produces
                     one character every 100 ns (unless suspended by
                     the fifo) since a random linear distribution is 
                     used for the character count.

                     If the fifo size is sufficiently large, the average
                     transfer time per character will approach 100 ns
                     since the producer and consumer will rarely be
                     blocked. However, as the fifo size decreases,
                     the average transfer time will increase because
                     the producer will sometimes be suspended when
                     it writes (due to a full fifo) and the consumer
                     will sometimes be suspended when it reads 
                     (due to an empty fifo).

                     The fifo size can be set via a command line argument
                     when running this program. By default, the fifo size
                     is 10. When the design is simulated, one hundred
                     thousand characters are transferred from the 
                     producer to the consumer and then performance
                     statistics are displayed.

                     Using this system level model, determine the size
                     of the fifo needed to sustain:

                     A) An average transfer time of 110 ns per character
                     B) An average transfer time of 105 ns per character

                     Hint: The answer to (A) is between 10 and 20.

  Original Author: Stuart Swan, Cadence Design Systems, 2001-06-18

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <systemc.h>

class write_if : virtual public sc_interface
{
  public:
    virtual void write(char) = 0;
    virtual void reset() = 0;
};

class read_if : virtual public sc_interface
{
  public:
    virtual void read(char &) = 0;
    virtual int num_available() = 0;
};

class fifo : public sc_channel, public write_if, public read_if
{
  public:
    fifo(sc_module_name name, int size_) : sc_channel(name), size(size_)
    {
	data = new char[size];
	num_elements = first = 0;
	num_read = max_used = average = 0;
        last_time = SC_ZERO_TIME;
    }

    ~fifo()
    {
      delete[] data;

      cout << endl << "Fifo size is: " << size << endl;
      cout << "Average fifo fill depth: " << 
		double(average) / num_read << endl;
      cout << "Maximum fifo fill depth: " << max_used << endl;
      cout << "Average transfer time per character: " 
		<< last_time / num_read << endl;
      cout << "Total characters transferred: " << num_read << endl;
      cout << "Total time: " << last_time << endl;
    }

    void write(char c) {
      if (num_elements == size)
        wait(read_event);

      data[(first + num_elements) % size] = c;
      ++ num_elements;
      write_event.notify();
    }

    void read(char &c){
      last_time = sc_time_stamp();
      if (num_elements == 0)
        wait(write_event);

      compute_stats();

      c = data[first];
      -- num_elements;
      first = (first + 1) % size;
      read_event.notify();
    }

    void reset() { num_elements = first = 0; }

    int num_available() { return num_elements;}

  private:
    char *data;
    int num_elements, first;
    sc_event write_event, read_event;
    int size, num_read, max_used, average;
    sc_time last_time;

    void compute_stats()
    {
      average += num_elements;

      if (num_elements > max_used)
         max_used = num_elements;

      ++num_read;
    }
};

class producer : public sc_module
{
  public:
    sc_port<write_if> out;

    SC_HAS_PROCESS(producer);

    producer(sc_module_name name) : sc_module(name)
    {
      SC_THREAD(main);
    }

    void main()
    {
      const char *str =
	"Visit www.systemc.org and see what SystemC can do for you today!\n";
      const char *p = str;
      int total = 100000;

      while (true)
      {
	int i = 1 + int(19.0 * rand() / RAND_MAX);  //  1 <= i <= 19

	while (--i >= 0)
	{
          out->write(*p++);
	  if (!*p) p = str;
	  -- total;
	}

	if (total <= 0)
	  break;

	wait(1000, SC_NS);
      }
    }
};

class consumer : public sc_module
{
  public:
    sc_port<read_if> in;

    SC_HAS_PROCESS(consumer);

    consumer(sc_module_name name) : sc_module(name)
    {
      SC_THREAD(main);
    }

    void main()
    {
      char c;

      while (true) {
        in->read(c);
	wait(100, SC_NS);
      }
    }
};

class top : public sc_module
{
  public:
    fifo fifo_inst;
    producer prod_inst;
    consumer cons_inst;

    top(sc_module_name name, int size) :
        sc_module(name) ,
	fifo_inst("Fifo1", size) , 
	prod_inst("Producer1") , 
	cons_inst("Consumer1")
    {
      prod_inst.out(fifo_inst);
      cons_inst.in(fifo_inst);
    }
};

int sc_main (int argc , char *argv[]) 
{
  int size = 10;

  if (argc > 1)
    size = atoi(argv[1]);

  if (size < 1)
    size = 1;

  if (size > 100000)
    size = 100000;

  top top1("Top1", size);
  sc_start();
  return 0;
}
