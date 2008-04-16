/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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
+ Pools for data buffers and extensions are not tested.  This also needs
the testing to be in a loop, and printout of the state of the pools at
end of testing.
*/


#define BUFFER_SIZE 2048

#include<systemc>
#include "tlm.h"
#include<iostream>

using namespace std;
using namespace tlm;
using namespace sc_dt;


// simple set of types with known sizeof(), for testing //
template<int SIZE> class dt {
  char content[SIZE];
};


#define convert(function) \
  switch(data_width) { \
    case 1:  function<dt<1> >(txn,bus_width); break; \
    case 2:  function<dt<2> >(txn,bus_width); break; \
    case 4:  function<dt<4> >(txn,bus_width); break; \
    case 8:  function<dt<8> >(txn,bus_width); break; \
    case 16:  function<dt<16> >(txn,bus_width); break; \
    case 32:  function<dt<32> >(txn,bus_width); break; \
    default:  cout << "bad data width\n"; \
    exit(1); \
  }


// forward declarations - see below
template<class DATAWORD> inline void
local_single_tohe(tlm_generic_payload *txn, unsigned int sizeof_databus);
template<class DATAWORD> inline void
local_single_fromhe(tlm_generic_payload *txn, unsigned int sizeof_databus);


void test_a_conversion(char cmd) {
  tlm_generic_payload *txn = new tlm_generic_payload();

  if(cmd == 'R') txn->set_read();
  else txn->set_write();

  cin.ignore(10000,'=');
  uint64 a;
  cin >> a;
  txn->set_address(a);

  cin.ignore(10000,'=');
  int l;
  cin >> l;
  txn->set_data_length(l);

  int bus_width;
  cin.ignore(10000,'=');  cin >> bus_width;

  int data_width;
  cin.ignore(10000,'=');  cin >> data_width;

  int initiator_offset;
  cin.ignore(10000,'=');  cin >> initiator_offset;

  unsigned char *byte_enable_legible =
    new unsigned char[txn->get_data_length() + 1];
  cin.ignore(10000,'=');  cin >> byte_enable_legible;
  if((byte_enable_legible[0] == '1') || (byte_enable_legible[0] == '0')) {
    txn->set_byte_enable_ptr(new unsigned char[txn->get_data_length()]);
    for(unsigned int i=0; i<txn->get_data_length(); i++)
      txn->get_byte_enable_ptr()[i] = byte_enable_legible[i] - '0';
  }

  cout << "enter initiator memory state = ("<< BUFFER_SIZE << " characters)\n";
  unsigned char initiator_mem[BUFFER_SIZE+1];
  cin.ignore(10000,'=');  cin >> initiator_mem;

  txn->set_data_ptr(initiator_mem + initiator_offset);

  cout << "enter target memory state = ("<< BUFFER_SIZE << " characters)\n";
  unsigned char target_mem[BUFFER_SIZE+1];
  cin.ignore(10000,'=');  cin >> target_mem;

  cout << "enter converter choice = (0 => generic, 1 => aligned, 2 => single)\n";
  int converter;
  cin.ignore(10000,'=');  cin >> converter;

#ifdef VERBOSE
  cout << "Initiator Intent\n";
  cout << "  Cmd = " << cmd << endl;
  cout << "  Addr = " << txn->get_address() << endl;
  cout << "  Len = " << txn->get_data_length() << endl;
  cout << "  Bus Width = " << bus_width << endl;
  cout << "  Data Word = " << data_width << endl;
  cout << "  Initiator offset and txn data pointer = " << initiator_offset << ", " << int(txn->data_ptr) << endl;
  cout << "  Byte enables and byte enable pointer = " << byte_enable_legible << ", " << int(txn->byte_enable) << endl;
  cout << "  Initiator memory = " << initiator_mem << endl;
  cout << "  Target memory = " << target_mem << endl;
  cout << "  Converter = " << converter << endl << endl;
#endif

  // initiator //
  switch(converter) {
    case 0:  convert(to_hostendian_generic); break;
    case 1:  convert(to_hostendian_aligned); break;
    case 2:  convert(to_hostendian_single); break;
    case 3:  convert(local_single_tohe); break;
    default:  cout << "no such converter as " << converter << endl;
    exit(1);
  }

#ifdef VERBOSE
  cout << "Converted Transaction\n";
  cout << "  Addr = " << txn->get_address() << endl;
  cout << "  Len = " << txn->get_data_length() << endl;
  cout << "  Txn data pointer = " << int(txn->get_data_ptr()) << endl;
  if(txn->get_byte_enable() != 0) {
    cout << "  Byte enables and byte enable pointer = ";
    for(int i=0; i<txn->get_data_length(); i++)
      cout << char('0' + txn->get_byte_enable_ptr()[i]);
    cout << ", " << int(txn->get_byte_enable_ptr()) << endl;
  }
  cout << endl;
#endif

  // target //
  if(txn->get_byte_enable_ptr() == 0) {
    if(txn->is_read())
      memcpy(txn->get_data_ptr(), target_mem+txn->get_address(),
        txn->get_data_length());
    else
      memcpy(target_mem+txn->get_address(), txn->get_data_ptr(),
        txn->get_data_length());
  } else {
    if(txn->is_read()) {
      for(unsigned int j=0; j<txn->get_data_length(); j++) {
        if(txn->get_byte_enable_ptr()[j])
          (txn->get_data_ptr())[j] = target_mem[j+txn->get_address()];
      }
    } else {
      for(unsigned int j=0; j<txn->get_data_length(); j++) {
        if(txn->get_byte_enable_ptr()[j])
          target_mem[j+txn->get_address()] = (txn->get_data_ptr())[j];
      }
    }
  }

  // initiator again //
  switch(converter) {
    case 0:  convert(from_hostendian_generic); break;
    case 1:  convert(from_hostendian_aligned); break;
    case 2:  convert(from_hostendian_single); break;
    case 3:  convert(local_single_fromhe); break;
    default:  cout << "no such converter as " << converter << endl;
    exit(1);
  }

  // print the results //
  cout << "Memory States after Transaction\n";
  cout << "  initiator = " << initiator_mem << endl;
  cout << "  target = " << target_mem << endl << endl;
}


int main(int argc, char **argv) {
  // no command line parameters //
  // get everything from stdin and build transaction object //
  cout << "\nTLM-2 Endianness Conversion Helper Functions Tester\n";
  cout << "March 2008\n\n";
  cout << "enter {R|W}, addr=a, len=l, bus width=b, word width=w, initiator offset=i, be = {x|01}\n";

  while(true) {
    char command;
    cin >> command;
    if(cin.eof()) break;
    if((command != 'R') && (command != 'W')) break;
    test_a_conversion(command);
  }
}


// converter functions for non-aligned single transactions
// included here for validation but not in the release as not
// regarded as necessary
template<class DATAWORD> inline void
local_single_tohe(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  cout << "not yet implemented: no conversion\n";
}


template<class DATAWORD> inline void
local_single_fromhe(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  cout << "not yet implemented: no conversion\n";
}

