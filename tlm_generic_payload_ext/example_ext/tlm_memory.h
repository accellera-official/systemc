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
#ifndef _TLM_MEMORY_H
#define _TLM_MEMORY_H

#include "tlm_helpers.h"

//
// TLM Memory: stores data as array of bytes
//
class tlm_memory
{
public:
    tlm_memory(unsigned int size,
               tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN)
        : mSize(size)
        , mStorage(new unsigned char[size])
        , mEndianness(endianness)
    {
    }

    ~tlm_memory()
    {
        delete mStorage;
    }
    
    void write(const unsigned char* v,
               unsigned int address,
               unsigned int burst_data_size)
    {
        tlm::copy_data(v, mStorage + address, burst_data_size, mEndianness);
    }
    
    void read(unsigned char* v,
              unsigned int address,
              unsigned int burst_data_size)
    {
        tlm::copy_data(mStorage + address, v, burst_data_size, mEndianness);
    }
    
private:
    unsigned int mSize;
    unsigned char * mStorage;
    tlm::tlm_endianness mEndianness;
};

#endif
