models
------

- Master/Slave and bus models used in the examples

SimpleSocket
------------

- Convenience socket classes

p2p
---

- Point-2-point examples (LT, AT, Core Decoupling,...)

bus
---

- Example system using a TLM2 bus model

bus_dmi
-------

- Example system using a TLM2 bus model with DMI

gp_extensions
----------

- Examples for generic payload, including ignorable extensions

transactor
----------

- Example transactors: transport <-> nb_transport 

static_extensions
-----------------

- Example code for static mandatory extension handling.


To run the examples:
- set SYSTEMC_HOME and TLM_HOME in your environment
- cd <example>/build-unix
- run 'make' to build the examples
- run 'cd <example_dir>; ./test' to run an example
