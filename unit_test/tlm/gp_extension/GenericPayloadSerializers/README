Serializers example:
----------------------------

This example shows how a master module (tlm_master), with a fixed 256-bit 
bus data width port, cand send transactions with different burst_data_size to 
a slave module (tlm_slave) with also a 256-bit bus data width port.

This example combines standard C++ types (char, short, int, etc) and standard SystemC 
types (sc_uint<32>, sc_bigint<128>, sc_biguint<256>, etc) both in the master and slave 
modules in combination with the generic transaction payload (GP).

Serializers functions (copy_from_array, copy_to_array) are used to read/write data 
from the GP to the specific data types. The signatures of these serialize functions are:

  template< class T >
  inline void copy_from_array( T& data, unsigned int index, unsigned char* m_data);
  
  template< class T >
  inline void copy_to_array( const T& data, unsigned int index, unsigned char* m_data)

Where: 
- T can be any standard C++ data type and/or the sc_int<w>, sc_uint<w>, sc_bigint<w>, 
sc_biguint<w> standard SystemC data types. 

- index is on units of sizeof(T) for C++ data types and on units of w/8 for SystemC types. 

- m_data is a pointer to the array and can be used in combination with the GP as follows:

	tlm::copy_from_array< sc_biguint<128> >(reg128, 0, gp->get_data_ptr()); 
	(this copy 128 bits (16 bytes) from GP to reg128 variable)
	
	tlm::copy_to_array< unsigned long long >(reg64, 0, gp->get_data_ptr());
  (this copy 64 bits (8 bytes) from reg64 to GP)
  
----------------------------

In order to compile this example you have to set these environment variables:
SYSTEMC_HOME -> your SystemC 2.2 installation
TLM_HOME     -> your (draft) TLM 2.0 installation

make or gmake builds the executable, and the test can be run with:

./tlm_gp_serializers

You can observe the following trace from execution:

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 256-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 128-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 64-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 48-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 32-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 16-bit operation : OK

master_1 : Single WRITE transaction :  OK
master_1 : Single READ transaction :  OK
master_1 : read and write 8-bit operation : OK

SystemC: simulation stopped by user.
Press any key to continue . . .