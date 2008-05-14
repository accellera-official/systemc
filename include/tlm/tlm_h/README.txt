User code should only #include the tlm.h header file in the parent directory. Everything is in the tlm namespace.

The header files are organizated, by subdirectory, as follows:

* tlm_analysis
This contains the basic mechanisms for doing analysis : analysis_if,
analysis_port, analysis_fifo, analysis_triple.

* tlm_peq
This contains the payload event queue (PEQ) which associates a transaction object with each queued event notification.

* tlm_quantum
This contains the quantum keeper for temporal decoupling.

* tlm_req_rsp
This provides support for TLM modeling based on a request/response pair that are passed by value; this is essentailly the original TLM 1.0 standard, with the addition of a few new methods such as the reference based form of transport.

* tlm_trans
This provides support for TLM modeling based on an aggregate transaction that is passed by reference.

