/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  peripheral.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "common.h"

SC_MODULE( peripheral )
{
  SC_HAS_PROCESS( peripheral );

  sc_in_clk clk;

  /* Ports( other than clk) */
  const signal_bool_vector16&       mem_addr;
  const signal_bool_vector8&        mem_data_out;
  signal_bool_vector8&              mem_data_in;
  const sc_signal<bool>&            mem_wr_n;
  const sc_signal<bool>&            mem_rd_n;
  const sc_signal<bool>&            mem_pswr_n;
  const sc_signal<bool>&            mem_psrd_n;
  const sc_signal<bool>&            mem_ale;
  sc_signal<bool>&                  mem_ea_n;
  
  const sc_signal<bool>&            p0_mem_reg_n;
  const sc_signal<bool>&            p0_addr_data_n;
  const sc_signal<bool>&            p2_mem_reg_n;
  
  peripheral(sc_module_name              NAME,
	     sc_clock&                   TICK,
	     const signal_bool_vector16& MEM_ADDR,
	     const signal_bool_vector8&  MEM_DATA_OUT,
	     signal_bool_vector8&        MEM_DATA_IN,
	     const sc_signal<bool>&      MEM_WR_N,
	     const sc_signal<bool>&      MEM_RD_N,
	     const sc_signal<bool>&      MEM_PSWR_N,
	     const sc_signal<bool>&      MEM_PSRD_N,
	     const sc_signal<bool>&      MEM_ALE,
	     sc_signal<bool>&            MEM_EA_N,
	     
	     const sc_signal<bool>&      P0_MEM_REG_N,
	     const sc_signal<bool>&      P0_ADDR_DATA_N,
	     const sc_signal<bool>&      P2_MEM_REG_N
	     )
    : 
      mem_addr(MEM_ADDR), 
      mem_data_out(MEM_DATA_OUT),
      mem_data_in(MEM_DATA_IN),
      mem_wr_n(MEM_WR_N),
      mem_rd_n(MEM_RD_N),
      mem_pswr_n(MEM_PSWR_N),
      mem_psrd_n(MEM_PSRD_N),
      mem_ale(MEM_ALE),      
      mem_ea_n(MEM_EA_N),
      p0_mem_reg_n(P0_MEM_REG_N),
      p0_addr_data_n(P0_ADDR_DATA_N),
      p2_mem_reg_n(P2_MEM_REG_N)
    {
      clk(TICK);
	  SC_CTHREAD( entry, clk.pos() );
    }
  
  /* Process functionality in member function below */
  void entry();
};
