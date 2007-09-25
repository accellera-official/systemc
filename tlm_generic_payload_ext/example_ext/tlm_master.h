/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef _TLM_MASTER_H
#define _TLM_MASTER_H

#include "systemc.h"

#include "tlm_gp_port.h"

#include "tlm_extensions.h"

class tlm_master : public sc_module
{
public:
    
	tlm::tlm_gp_port< 32 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_master);

	// Constructor
    tlm_master(sc_module_name _name)
        : sc_module(_name)
        , bus_port("bus_port")
	{
            m_ext1.data1 = 10;
            m_ext2.data2 = 20;
            m_ext3.data3 = 30;
            SC_THREAD(main);
	}
    
    // Destructor
    ~tlm_master()
	{
	}
    
    void main()
	{
            while(true)
            {
                test_single_write(false, false, false);
                test_single_write(false, false, true);
                test_single_write(false, true,  false);
                test_single_write(false, true,  true);
                test_single_write(true,  false, false);
                test_single_write(true,  false, true);
                test_single_write(true,  true,  false);
                test_single_write(true,  true,  true);
                test_single_read();
                sc_stop();
                sc_core::wait();
            }
	}

private:

    tlm::tlm_generic_payload  m_gp;
    tlm_extension1 m_ext1;
    tlm_extension2 m_ext2;
    tlm_extension3 m_ext3;

    // GP tests
    void test_single_write(bool use_ex1,
                           bool use_ex2,
                           bool use_ex3);
    void test_single_read();

};

#endif
