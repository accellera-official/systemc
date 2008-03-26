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

  main.cpp -- Example program for specialized signal support.

  Original Author: Andy Goodrich, Forte Design Systems, 11 April 2005

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include "systemc.h"
#include "scx_signal_int.h"
#include "scx_signal_signed.h"
#include "scx_signal_uint.h"
#include "scx_signal_unsigned.h"


#define DISPLAY(exp,base) cout << #exp << " = " << base << exp << endl;
#define TEST(exp1,exp2,base) \
    if ( exp1 != exp2 ) \
	{ \
	    cout << #exp1 << "(" << base << exp1 << ") != " \
		     << #exp2 << "(" << base << exp2 << ")" << endl; \
    } 

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_THREAD(exec)
		sensitive << m_clk.pos();
       
        // CONNECT PORTS TO THEIR BACKING SIGNALS:

		m_in_bigint4(m_back_in_bigint4);
		m_inout_bigint4(m_back_inout_bigint4);
		m_in_biguint4(m_back_in_biguint4);
		m_inout_biguint4(m_back_inout_biguint4);
		m_in_int4(m_back_in_int4);
		m_inout_int4(m_back_inout_int4);
		m_in_uint4(m_back_in_uint4);
		m_inout_uint4(m_back_inout_uint4);
	}


	void exec()
	{
		// TEST RIGHT HAND CONCATENATION:

	    m_back_in_bigint4 = 1;
	    m_back_inout_bigint4 = 2;
	    m_back_in_biguint4 = 3;
	    m_back_inout_biguint4 = 4;
	    m_back_in_int4 = 5;
	    m_back_inout_int4 = 6;
	    m_back_in_uint4 = 7;
	    m_back_inout_uint4 = 8;
        wait();
        DISPLAY((
			m_in_bigint4,
			m_inout_bigint4,
			m_in_biguint4,
			m_inout_biguint4,
			m_in_int4,
			m_inout_int4,
			m_in_uint4,
			m_inout_uint4
        ),hex);
		m_signal_bigint8 = ( m_in_bigint4,m_inout_uint4);
		m_signal_biguint8 = ( m_inout_bigint4,m_in_uint4);
		m_signal_int8 = ( m_in_biguint4,m_inout_int4);
		m_signal_uint8 = ( m_inout_biguint4,m_in_int4);
		wait();
		DISPLAY(m_signal_bigint8,hex);
		DISPLAY(m_signal_biguint8,hex);
		DISPLAY(m_signal_int8,hex);
		DISPLAY(m_signal_uint8,hex);

		// TEST LEFT HAND CONCATENATION:

        (m_inout_uint4,m_signal_biguint8) = 0x123;
		wait();
		(m_signal_bigint8,m_inout_bigint4) = (m_inout_uint4,m_signal_biguint8);
		(m_signal_biguint8,m_inout_biguint4) = 0x456;
		(m_inout_int4,m_signal_int8) = 0x978;
		(m_inout_uint4,m_signal_uint8) = 0xcab;
        wait();
		DISPLAY(m_signal_bigint8,hex);
		DISPLAY(m_inout_bigint4,hex);
		DISPLAY(m_signal_biguint8,hex);
		DISPLAY(m_inout_biguint4,hex);
		DISPLAY(m_signal_int8,hex);
		DISPLAY(m_inout_int4,hex);
		DISPLAY(m_signal_uint8,hex);
		DISPLAY(m_inout_uint4,hex);

		// TEST MATH OPERATIONS:
		//
		// Note there is not need to call .read() methods!

        m_nb_signal_bigint8 = -1; 
		m_nb_signal_biguint8 = 255;
        m_nb_signal_int8 = -2; 
		m_nb_signal_uint8 = 128;
        m_signal_bigint8 = -1; 
		m_signal_biguint8 = 255;
        m_signal_int8 = -2; 
		m_signal_uint8 = 128;
		wait();
		TEST(m_signal_bigint8*m_signal_biguint8,
		     m_nb_signal_bigint8*m_nb_signal_biguint8,dec);
		TEST(m_signal_bigint8*m_signal_uint8,
		     m_nb_signal_bigint8*m_nb_signal_uint8,dec);
		TEST(m_signal_int8*m_signal_biguint8,
		     m_nb_signal_int8*m_nb_signal_biguint8,dec);
		TEST(m_signal_int8*m_signal_uint8,
		     m_nb_signal_int8*m_nb_signal_uint8,dec);

		TEST(m_signal_bigint8+m_signal_biguint8,
		     m_nb_signal_bigint8+m_nb_signal_biguint8,dec);
		TEST(m_signal_bigint8+m_signal_uint8,
		     m_nb_signal_bigint8+m_nb_signal_uint8,dec);
		TEST(m_signal_int8+m_signal_biguint8,
		     m_nb_signal_int8+m_nb_signal_biguint8,dec);
		TEST(m_signal_int8+m_signal_uint8,
		     m_nb_signal_int8+m_nb_signal_uint8,dec);

		TEST(m_signal_bigint8-m_signal_biguint8,
		     m_nb_signal_bigint8-m_nb_signal_biguint8,dec);
		TEST(m_signal_bigint8-m_signal_uint8,
		     m_nb_signal_bigint8-m_nb_signal_uint8,dec);
		TEST(m_signal_int8-m_signal_biguint8,
		     m_nb_signal_int8-m_nb_signal_biguint8,dec);
		TEST(m_signal_int8-m_signal_uint8,
		     m_nb_signal_int8-m_nb_signal_uint8,dec);

		TEST(m_signal_int8+m_signal_uint8*m_signal_bigint8-m_signal_biguint8,
		     m_nb_signal_int8+m_nb_signal_uint8*m_nb_signal_bigint8-m_nb_signal_biguint8,dec);

        m_nb_in_bigint4 = -1; 
		m_nb_in_biguint4 = 15;
        m_nb_in_int4 = -2; 
		m_nb_in_uint4 = 8;
        m_back_in_bigint4 = -1; 
		m_back_in_biguint4 = 15;
        m_back_in_int4 = -2; 
		m_back_in_uint4 = 8;
		wait();
		TEST(m_in_bigint4*m_in_biguint4,
		     m_nb_in_bigint4*m_nb_in_biguint4,dec);
		TEST(m_in_bigint4*m_in_uint4,
		     m_nb_in_bigint4*m_nb_in_uint4,dec);
		TEST(m_in_int4*m_in_biguint4,
		     m_nb_in_int4*m_nb_in_biguint4,dec);
		TEST(m_in_int4*m_in_uint4,
		     m_nb_in_int4*m_nb_in_uint4,dec);

		TEST(m_in_bigint4+m_in_biguint4,
		     m_nb_in_bigint4+m_nb_in_biguint4,dec);
		TEST(m_in_bigint4+m_in_uint4,
		     m_nb_in_bigint4+m_nb_in_uint4,dec);
		TEST(m_in_int4+m_in_biguint4,
		     m_nb_in_int4+m_nb_in_biguint4,dec);
		TEST(m_in_int4+m_in_uint4,
		     m_nb_in_int4+m_nb_in_uint4,dec);

		TEST(m_in_bigint4-m_in_biguint4,
		     m_nb_in_bigint4-m_nb_in_biguint4,dec);
		TEST(m_in_bigint4-m_in_uint4,
		     m_nb_in_bigint4-m_nb_in_uint4,dec);
		TEST(m_in_int4-m_in_biguint4,
		     m_nb_in_int4-m_nb_in_biguint4,dec);
		TEST(m_in_int4-m_in_uint4,
		     m_nb_in_int4-m_nb_in_uint4,dec);

		TEST(m_in_int4+m_in_uint4*m_in_bigint4-m_in_biguint4,
		     m_nb_in_int4+m_nb_in_uint4*m_nb_in_bigint4-m_nb_in_biguint4,dec);
	    sc_stop();
	}

	sc_in<bool>                m_clk;

    sc_signal<sc_bigint<4> >   m_back_in_bigint4;
    sc_signal<sc_bigint<4> >   m_back_inout_bigint4;

    sc_signal<sc_biguint<4> >  m_back_in_biguint4;
    sc_signal<sc_biguint<4> >  m_back_inout_biguint4;

    sc_signal<sc_int<4> >      m_back_in_int4;
    sc_signal<sc_int<4> >      m_back_inout_int4;

    sc_signal<sc_uint<4> >     m_back_in_uint4;
    sc_signal<sc_uint<4> >     m_back_inout_uint4;

    sc_in<sc_bigint<4> >       m_in_bigint4;
    sc_inout<sc_bigint<4> >    m_inout_bigint4;
    sc_signal<sc_bigint<8> >   m_signal_bigint8;

    sc_in<sc_biguint<4> >      m_in_biguint4;
    sc_inout<sc_biguint<4> >   m_inout_biguint4;
    sc_signal<sc_biguint<8> >  m_signal_biguint8;

    sc_in<sc_int<4> >          m_in_int4;
    sc_inout<sc_int<4> >       m_inout_int4;
    sc_signal<sc_int<8> >      m_signal_int8;

    sc_in<sc_uint<4> >         m_in_uint4;
    sc_inout<sc_uint<4> >      m_inout_uint4;
    sc_signal<sc_uint<8> >     m_signal_uint8;

    sc_bigint<4>               m_nb_in_bigint4;
    sc_bigint<4>               m_nb_inout_bigint4;
    sc_bigint<8>               m_nb_signal_bigint8;

    sc_biguint<4>              m_nb_in_biguint4;
    sc_biguint<4>              m_nb_inout_biguint4;
    sc_biguint<8>              m_nb_signal_biguint8;

    sc_int<4>                  m_nb_in_int4;
    sc_int<4>                  m_nb_inout_int4;
    sc_int<8>                  m_nb_signal_int8;

    sc_uint<4>                 m_nb_in_uint4;
    sc_uint<4>                 m_nb_inout_uint4;
    sc_uint<8>                 m_nb_signal_uint8;

};


int sc_main( int argc, char* argv[] )
{
	sc_clock clock;
	DUT      dut("dut");
	dut.m_clk(clock);
	sc_start();

	return 0;
}
