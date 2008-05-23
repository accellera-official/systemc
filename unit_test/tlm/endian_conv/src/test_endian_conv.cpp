/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


/*
This C++ programme runs single transactions through a single
endianness conversion function, then through a simple memory model,
then converts it back.
Takes the initial memory state as input and provides the final
memory state as output.

TO DO:
+ Verification runs very slowly.  One of the reasons for this is the overhead
starting a new process for every test.  Therefore the testing should be
put into a loop rather than being one-shot, with reuse of memory states.
*/


#define BUFFER_SIZE 2048

#include<systemc>
#include "tlm.h"
#include<iostream>
#include<time.h>

using namespace std;
using namespace tlm;
using namespace sc_dt;


// simple set of types with known sizeof(), for testing //
template<int SIZE> class dt {
  char content[SIZE];
};


#define convert(function) \
  switch(data_width) { \
    case 1:  function<dt<1> >(&txn,bus_width); break; \
    case 2:  function<dt<2> >(&txn,bus_width); break; \
    case 4:  function<dt<4> >(&txn,bus_width); break; \
    case 8:  function<dt<8> >(&txn,bus_width); break; \
    case 16:  function<dt<16> >(&txn,bus_width); break; \
    case 32:  function<dt<32> >(&txn,bus_width); break; \
    default:  cout << "bad data width\n"; \
    exit(1); \
  }


// forward declarations - see below
template<class DATAWORD> inline void
local_single_tohe(tlm_generic_payload *txn, unsigned int sizeof_databus);
template<class DATAWORD> inline void
local_single_fromhe(tlm_generic_payload *txn, unsigned int sizeof_databus);


void test_a_conversion(char cmd) {
  tlm_generic_payload txn;

  if(cmd == 'R') txn.set_read();
  else txn.set_write();

  cin.ignore(10000,'=');
  uint64 a;
  cin >> a;
  txn.set_address(a);

  cin.ignore(10000,'=');
  int l;
  cin >> l;
  txn.set_data_length(l);

  int bus_width;
  cin.ignore(10000,'=');  cin >> bus_width;

  int data_width;
  cin.ignore(10000,'=');  cin >> data_width;

  int initiator_offset;
  cin.ignore(10000,'=');  cin >> initiator_offset;

  unsigned char *original_byte_enable = 0;
  unsigned char *byte_enable_legible =
    new unsigned char[txn.get_data_length() + 1];
  cin.ignore(10000,'=');  cin >> byte_enable_legible;
  if((byte_enable_legible[0] == '1') || (byte_enable_legible[0] == '0')) {
    txn.set_byte_enable_ptr(new unsigned char[txn.get_data_length()]);
    txn.set_byte_enable_length(txn.get_data_length());
    original_byte_enable = txn.get_byte_enable_ptr();
    for(unsigned int i=0; i<txn.get_data_length(); i++) {
      if(byte_enable_legible[i] == '0') {
        txn.get_byte_enable_ptr()[i] = TLM_BYTE_DISABLED;
      } else if(byte_enable_legible[i] == '1') {
        txn.get_byte_enable_ptr()[i] = TLM_BYTE_ENABLED;
      } else {
        // not enough byte enables
        txn.set_byte_enable_length(i);
        break;
      }
    }
  }

  int stream_width;
  cin.ignore(10000,'=');  cin >> stream_width;
  txn.set_streaming_width(stream_width);

  cout << "enter initiator memory state = ("<< BUFFER_SIZE << " characters)\n";
  unsigned char initiator_mem[BUFFER_SIZE+1];
  cin.ignore(10000,'=');  cin >> initiator_mem;

  txn.set_data_ptr(initiator_mem + initiator_offset);

  cout << "enter target memory state = ("<< BUFFER_SIZE << " characters)\n";
  unsigned char target_mem[BUFFER_SIZE+1];
  cin.ignore(10000,'=');  cin >> target_mem;

  cout << "enter converter choice = (0 => generic, 1 => word, 2 => aligned, 3 => single)\n";
  int converter;
  cin.ignore(10000,'=');  cin >> converter;

  cout << "Initiator Intent\n";
  cout << "  Cmd = " << cmd << endl;
  cout << "  Addr = " << txn.get_address() << endl;
  cout << "  Len = " << txn.get_data_length() << endl;
  cout << "  Bus Width = " << bus_width << endl;
  cout << "  Data Word = " << data_width << endl;
#ifdef VERBOSE
  cout << "  Initiator offset and txn data pointer = " << initiator_offset << ", " << int(txn.get_data_ptr()) << endl;
  cout << "  Byte enables and byte enable pointer = " << byte_enable_legible << ", " << int(txn.get_byte_enable_ptr()) << endl;
#else
  cout << "  Initiator offset = " << initiator_offset << endl;
  cout << "  Byte enables = " << byte_enable_legible << endl;
#endif
  cout << "  Byte enable length = " << txn.get_byte_enable_length() << endl;
  cout << "  Streaming width = " << txn.get_streaming_width() << endl;
  cout << "  Initiator memory = " << initiator_mem << endl;
  cout << "  Target memory = " << target_mem << endl;
  cout << "  Converter = " << converter << endl << endl;

  // initiator //
  switch(converter) {
    case 0:  convert(tlm_to_hostendian_generic); break;
    case 1:  convert(tlm_to_hostendian_word); break;
    case 2:  convert(tlm_to_hostendian_aligned); break;
    case 3:  convert(tlm_to_hostendian_single); break;
    case 4:  convert(local_single_tohe); break;
    default:  cout << "no such converter as " << converter << endl;
    exit(1);
  }

  cout << "Converted Transaction\n";
  cout << "  Addr = " << txn.get_address() << endl;
  cout << "  Len = " << txn.get_data_length() << endl;
#ifdef VERBOSE
  cout << "  Txn data pointer = " << int(txn.get_data_ptr()) << endl;
  if(txn.get_byte_enable_ptr() != 0) {
    cout << "  Byte enables and byte enable pointer = ";
    for(unsigned int i=0; i<txn.get_data_length(); i++)
      cout << (txn.get_byte_enable_ptr()[i] ? '1' : '0');
    cout << ", " << int(txn.get_byte_enable_ptr()) << endl;
  }
#else
  cout << "  Txn data pointer = " <<
    (txn.get_data_ptr() == initiator_mem+initiator_offset ? "unchanged" : "changed") << endl;
  if(txn.get_byte_enable_ptr() != 0) {
    cout << "  Byte enables and byte enable pointer = ";
    for(unsigned int i=0; i<txn.get_data_length(); i++)
      cout << (txn.get_byte_enable_ptr()[i] ? '1' : '0');
    cout << ", " <<
     (txn.get_byte_enable_ptr() == original_byte_enable ? "unchanged" : "changed") << endl;
  }
#endif
  cout << "  Byte enable length = " << txn.get_byte_enable_length() << endl;
  cout << "  Streaming width = " << txn.get_streaming_width() << endl;
  cout << endl;

  // target //
  int sw = txn.get_streaming_width();
  if((txn.get_data_length()/sw)*sw != txn.get_data_length()) {
    cout << "ERROR: Data length not a multiple of streaming width\n";
    exit(1);
  }
  for(unsigned int ss = 0; ss < txn.get_data_length(); ss += sw) {
    if(txn.get_byte_enable_ptr() == 0) {
      // simple transaction can be processed by mem-copy
      if(txn.is_read())
        memcpy(ss+txn.get_data_ptr(), target_mem+txn.get_address(), sw);
      else
        memcpy(target_mem+txn.get_address(), ss+txn.get_data_ptr(), sw);
    } else {
      // complex transaction, byte enables, maybe shorter than data
      int bel = txn.get_byte_enable_length();
      if(txn.is_read()) {
        for(int j=0; j<sw; j++) {
          if(txn.get_byte_enable_ptr()[(ss+j) % bel])
            (txn.get_data_ptr())[ss+j] = target_mem[j+txn.get_address()];
        }
      } else {
        for(int j=0; j<sw; j++) {
          if(txn.get_byte_enable_ptr()[(ss+j) % bel])
            target_mem[j+txn.get_address()] = (txn.get_data_ptr())[ss+j];
        }
      }
    }
  }

  // initiator again //
  if((rand() & 0x100) && (converter < 4)) {
#ifdef VERBOSE
    cout << "using single entry point for response\n";
#endif
    tlm_from_hostendian(&txn);
  } else {
#ifdef VERBOSE
    cout << "using specific entry point for response\n";
#endif
    switch(converter) {
      case 0:  convert(tlm_from_hostendian_generic); break;
      case 1:  convert(tlm_from_hostendian_word); break;
      case 2:  convert(tlm_from_hostendian_aligned); break;
      case 3:  convert(tlm_from_hostendian_single); break;
      case 4:  convert(local_single_fromhe); break;
      default:  cout << "no such converter as " << converter << endl;
      exit(1);
    }
  }

  // print the results //
  cout << "Memory States after Transaction\n";
  cout << "  initiator = " << initiator_mem << endl;
  cout << "  target = " << target_mem << endl << endl;

  // clean up
  delete [] byte_enable_legible;
  if(original_byte_enable != 0) delete [] original_byte_enable;
  if(txn.get_extension<tlm_endian_context>() != 0) {
    txn.get_extension<tlm_endian_context>()->free();
    txn.clear_extension<tlm_endian_context>();
  }
}


int main(int argc, char **argv) {
  // no command line parameters //
  // get everything from stdin and build transaction object //
  cout << "\nTLM-2 Endianness Conversion Helper Functions Tester\n";
  cout << "March 2008\n\n";

  srand(time(NULL));

  while(true) {
    cout << "enter {R|W}, addr=a, len=l, bus width=b, word width=w, initiator offset=i, be={x|01}, stream width=s\n";
    char command;
    cin >> command;
    if(cin.eof()) break;
    if((command != 'R') && (command != 'W')) break;
    test_a_conversion(command);
  }

  if(local_buffer_pool.get_pool_size() > 2) {
    // expect a pool of 2, one for data and one for byte enable
    cout << "Error: buffer pool is not working properly\n";
    cout << "Pool contains " << local_buffer_pool.get_pool_size() << " buffers\n";
    exit(1);
  }
  cout << "Buffer pool buffer size has reached " <<
    local_buffer_pool.get_buffer_size() << " bytes\n";
}


// converter functions for non-aligned single transactions
// included here for validation only.  not designed for general use.

unsigned char *original_dptr;
sc_dt::uint64 original_addr;

template<class DATAWORD> inline void
local_single_tohe(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  if(txn->get_data_length() != sizeof(DATAWORD)) {
    cout << "Error:  local_single_tohe() wrongly called\n";
    exit(1);
  }

  sc_dt::uint64 mask = sizeof_databus - 1;

  // set up new buffers, length and address
  if(sizeof(DATAWORD) > sizeof_databus)
    txn->set_data_length(sizeof_databus + sizeof(DATAWORD));
  else
    txn->set_data_length(2 * sizeof_databus);
  txn->set_streaming_width(txn->get_data_length());
  unsigned char *new_data = new unsigned char[txn->get_data_length()];
  unsigned char *new_be = new unsigned char[txn->get_data_length()];
  // drive all BEs to zero initially
  for(unsigned int i=0; i<txn->get_data_length(); i++) new_be[i] = 0;
  sc_dt::uint64 new_addr = txn->get_address() & ~mask;

  // Comments assume arithmetic mode big endian initiator modelled on little
  // endian host (but the functionality is the same for LE initiator on BE host)

  // iterate over the initiator word byte by byte, MSB first
  unsigned char *curr_d = txn->get_data_ptr() + sizeof(DATAWORD) - 1;
  unsigned char *curr_b = txn->get_byte_enable_ptr() + sizeof(DATAWORD) - 1;

  // initiator intent is to put the MSB at the address given in the transaction
  sc_dt::uint64 curr_a = txn->get_address();

  // iterate copying data and byte enables
  for( ; curr_d >= txn->get_data_ptr(); curr_d--, curr_b--, curr_a++) {
    // work out the address in the TLM interpretation of the initiator's intent
    sc_dt::uint64 he_addr = curr_a ^ mask;
    int idx = he_addr - new_addr;
    if(txn->is_write()) new_data[idx] = *curr_d;
    if(txn->get_byte_enable_ptr() == 0) new_be[idx] = 1;
    else new_be[idx] = *curr_b;
  }

  // replace the pointers
  original_dptr = txn->get_data_ptr();
  txn->set_data_ptr(new_data);
  txn->set_byte_enable_ptr(new_be);
  txn->set_byte_enable_length(txn->get_data_length());
  original_addr = txn->get_address();
  txn->set_address(new_addr);
}


template<class DATAWORD> inline void
local_single_fromhe(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  sc_dt::uint64 mask = sizeof_databus - 1;

  // Comments assume arithmetic mode big endian initiator modelled on little
  // endian host (but the functionality is the same for LE initiator on BE host)

  // iterate over the initiator word byte by byte, MSB first
  unsigned char *curr_d = original_dptr + sizeof(DATAWORD) - 1;

  // initiator intent is to put the MSB at the address given in the transaction
  sc_dt::uint64 curr_a = original_addr;

  // iterate copying data and byte enables
  for( ; curr_d >= original_dptr; curr_d--, curr_a++) {
    // work out the address in the TLM interpretation of the initiator's intent
    sc_dt::uint64 he_addr = curr_a ^ mask;
    int idx = he_addr - txn->get_address();
    if((txn->is_read()) && (txn->get_byte_enable_ptr()[idx] != 0))
      *curr_d = txn->get_data_ptr()[idx];
  }

  // clean up
  delete [] txn->get_data_ptr();
  delete [] txn->get_byte_enable_ptr();
}

