
The basic rules for all the interfaces in the TLM 2.0 kit are effective pass by
value.Effective pass-by-value is used to implement message passing semantics.

However, an exception is made for the pointer versions of tlm_request and
tlm_response. These use pass by pointer for the data and byte enable fields.
However, the basic message passing semantics will be preserved by obeying the
rules below.

1. Allocate and Deallocation

1.1 If a process deallocates the arrays, the same process must deallocate the
arrays.

1.2 If you can't or won't deallocate, use pass-by-val

1.3 People may call you a master, system initiator or whatever if you allocate

2 Read / Write Permissions

2.1 If you're a master and the transaction is a write, you have write
permission on the arrays in the request

2.2 If you're a master and the transaction is a read, you have read permission
on the arrays in the response

2.3 If you're a slave and the transaction is a write, you have read permission
on the data in the request

2.4 If you're a slave and the transaction is a read, you have write permission
on the data in the response

2.5 No other read or write permissions are granted on the arrays other than
those specified above. 

3 Request / Response matching

3.1 The slave must copy the pointers to the arrays from request to response.

3.2 The arrival of the response back at the master is used to determine when
to deallocate or reuse the arrays.

4 Analysis

4.1 If you're doing analysis you must use the analysis interface ( eg
monitoring, functional coverage, performance analysis, scoreboarding )

4.2 If you're designing a life extending analysis component ( eg a scoreboard )
then you must use analysis_fifo or analysis_buffer with the by-val variants of
the request and response datastructures.

4.3 If you're connecting a by-pointer TLM to a life extending analysis
component, you must convert from by-pointer to by-val.

