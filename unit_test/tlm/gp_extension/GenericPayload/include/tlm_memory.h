#ifndef _TLM_MEMORY_H
#define _TLM_MEMORY_H

#include "tlm.h"
#include <vector>

//
// TLM Memory: stores data as array of bytes
//
class tlm_memory
{
public:
  tlm_memory(unsigned int size,
             tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN) :
    mSize(size),
    mStorage(new unsigned char[size]),
    mEndianness(endianness)
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
