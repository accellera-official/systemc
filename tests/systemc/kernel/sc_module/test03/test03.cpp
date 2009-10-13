/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test03.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of positional binding -- general test of operator ()

#include "systemc.h"

template <class T>
SC_MODULE( prim_source )
{
    sc_out<T>                     out;
    sc_port<sc_signal_out_if<T> > port_out;
    
    SC_CTOR( prim_source ) {}
};

template <class T>
SC_MODULE( prim_transfer )
{
    sc_in<T>                      in;
    sc_port<sc_signal_in_if<T> >  port_in;
    sc_out<T>                     out;
    sc_port<sc_signal_out_if<T> > port_out;

    
    SC_CTOR( prim_transfer ) {}
};

template <class T>
SC_MODULE( prim_sink )
{
    sc_in<T>                     in;
    sc_port<sc_signal_in_if<T> > port_in;
    
    SC_CTOR( prim_sink ) {}
};

template <class T>
SC_MODULE( hier_source )
{
    sc_out<T>                     out;
    sc_port<sc_signal_out_if<T> > port_out;

    prim_source<T> prim_source1;

    SC_CTOR( hier_source )
    : prim_source1( "prim_source1" )
    {
	prim_source1( out, port_out );
    }
};

template <class T>
SC_MODULE( hier_transfer )
{
    sc_in<T>                      in;
    sc_port<sc_signal_in_if<T> >  port_in;
    sc_out<T>                     out;
    sc_port<sc_signal_out_if<T> > port_out;

    prim_transfer<T> prim_transfer1;

    SC_CTOR( hier_transfer )
    : prim_transfer1( "prim_transfer1" )
    {
	prim_transfer1( in, port_in, out, port_out );
    }
};

template <class T>
SC_MODULE( hier_sink )
{
    sc_in<T>                     in;
    sc_port<sc_signal_in_if<T> > port_in;

    prim_sink<T> prim_sink1;

    SC_CTOR( hier_sink )
    : prim_sink1( "prim_sink1" )
    {
	prim_sink1( in, port_in );
    }
};

template <class T>
SC_MODULE( hier1 )
{
    sc_signal<T> sig1;
    sc_signal<T> sig2;
    sc_signal<T> sig3;
    sc_signal<T> sig4;

    prim_source<T>   prim_source1;
    prim_transfer<T> prim_transfer1;
    prim_sink<T>     prim_sink1;

    SC_CTOR( hier1 )
    : prim_source1( "prim_source1" ),
      prim_transfer1( "prim_transfer1" ),
      prim_sink1( "prim_sink1" )
    {
	prim_source1( sig1, sig2 );
	prim_transfer1( sig1, sig2, sig3, sig4 );
	prim_sink1( sig3, sig4 );
    }
};

template <class T>
SC_MODULE( hier2 )
{
    sc_signal<T> sig1;
    sc_signal<T> sig2;
    sc_signal<T> sig3;
    sc_signal<T> sig4;

    hier_source<T>   hier_source1;
    hier_transfer<T> hier_transfer1;
    hier_sink<T>     hier_sink1;

    SC_CTOR( hier2 )
    : hier_source1( "hier_source1" ),
      hier_transfer1( "hier_transfer1" ),
      hier_sink1( "hier_sink1" )
    {
	hier_source1( sig1, sig2 );
	hier_transfer1( sig1, sig2, sig3, sig4 );
	hier_sink1( sig3, sig4 );
    }
};

int
sc_main( int, char*[] )
{
    hier1<int> hier1_int( "hier1_int" );
    hier1<bool> hier1_bool( "hier1_bool" );
    hier1<sc_logic> hier1_logic( "hier1_logic" );

    hier2<int> hier2_int( "hier2_int" );
    hier2<bool> hier2_bool( "hier2_bool" );
    hier2<sc_logic> hier2_logic( "hier2_logic" );

    sc_initialize();

    return 0;
}
