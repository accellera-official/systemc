
The basic user level include is tlm.h. Everything is in the tlm namespace.

There are five sub namespaces

1. tlm_core

This contains the old TLM 1.0 standard, with the addition of a few new methods
such as the reference based form of transport.

2. analysis

This contains the basic mechanisms for doing analysis : analysis_if,
analysis_port, analysis_fifo, analysis_triple.

3. tlm_annotated

This contains all the annotated interfaces and channels.

These interfaces add 

bool nb_put( const T & , const sc_time & );
bool nb_can_put( const sc_time & , tlm_tag<T> * );

to any interface which inherits, however indirectly, from
tlm_nonblocking_put_if, 

and

bool nb_get( const T & , const sc_time & );
bool nb_can_get( const sc_time & , tlm_tag<T> * );

to any interface which inherits, however indirectly, from
tlm_nonblocking_get_if.

The annotated interfaces are all of the form tlm_annotated_* 

The annotated channels are tlm_annotated_fifo and tlm_annotated_req_rsp_channel

This code depends on tlm_core and analysis.

4. tlm_bus

This contains the transaction level interoperability section of the TLM 2.0
standard.

This consists of the request and response datastructures, and everything
required for these datastructures.

In addition, there is 

5. tlm_ports

This contains all the code for tlm_initiator_port and tlm_target_port. These
provide some infrastructure used for transaction recording and target ids (
see To Be Done below ).

In addition to these five directories, there is an extension to the analysis
namespace in :

6. tlm_bus_analysis

This contains prototype code that disables analysis fifo for pointer versions 
of tlm_request and tlm_response, and an adaptor widget to convert from pointer
versions to by-val versions. This code is currently untested !

To be Done :

Analysis
- add analysis_buffer to analysis
- test tlm_bus_anlysis
- merge tlm_initiator_port and tlm_target_port functionality into analysis
- enable / disable for both local analysis and upward propagation
TLM_BUS
- ostream & operators for request and response
GENERAL
- add more docs

