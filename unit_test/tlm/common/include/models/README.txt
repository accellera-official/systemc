SimpleLTMaster1/SimpleLTSlave1
------------------------------

- LT Master/Slave model using the base (standard) tlm socket
- Added support for DMI in SimpleLTSlave1

SimpleLTMaster1_DMI
-------------------

- uses DMI transactions, the DMI structure is using the DMI-hint
  to check if a DMI request would make sense.
- uses a single transport_dbg transaction at end_of_simulation()

SimpleLTMaster2/SimpleLTSlave2
------------------------------

- LT Master/Slave model using the convenience tlm socket
- Slave and Master model use the REGISTER_SOCKETPROCESS macro to register a
  transport callback to the socket
- Added support for DMI handling, callback registration with
  REGISTER_SOCKETPROCESS
- SimpleLTSlave2 does not register the transport_dbg callback, so that
  we are able to test this case in bus_dmi.

SimpleLTMaster2_DMI
-------------------

- uses DMI transactions, but ignoring the DMI hint
- uses a single transport_dbg transaction at end_of_simulation()

SimpleLTMaster3
---------------

- LT Master model using the convenience tlm socket
- Master model uses the endEvent of the socket to wait until the transaction is finished

SimpleLTMaster3_DMI
-------------------

- based on SimpleMaster3, uses DMI (without DMI hint)

SimpleATMaster1/SimpleATSlave1
------------------------------

- AT Master/Slave model implementing the AT protocol
- one call of nb_transport for each timing point in the protocol (BEGIN_REQ, END_REQ, BEGIN_RESP and END_RESP)

SimpleATMaster2/SimpleATSlave2
------------------------------

- AT Master/Slave model implementing the AT protocol with timing annotation
- only a call of nb_transport for the start of a phase (BEGIN_REQ and BEGIN_RESP)
- end of a phase is notified via timing annotation (t argument)

CoreDecouplingLTMaster
----------------------

- LT Master using 'Core Decoupling'

ExplicitLTSlave
---------------

- LT Slave that uses explicit timing: seperate call for the start of the response phase
- added support for debug transactions

SimpleBus
---------

- Simple bus model
- Runtime switcheable between LT and AT (can only switch if no transactions are pending)
- No limitation on number of pending transactions (all slaves that can return false must support multiple transactions)
- added support for DMI and debug transactions
- LT mode:
-- Forward nb_transport calls to master/slaves
-- Only one active request/response phase
- AT mode:
-- Incoming transactions are queued
-- AT protocol is executed from a different SC_THREAD
-- A slave is notified immediately of the end of a transaction (using timing annotation). This is needed because the master can re-use the transaction (and the slave may use the transaction pointer to identify the transaction)
