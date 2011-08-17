#!/bin/sh 
########################################################################
# Shell script to run the risc_cpu example with disabled check for
# "Error: (E115) sc_signal<T> cannot have more than one driver."
########################################################################
export SC_SIGNAL_WRITE_CHECK=DISABLE
./risc_cpu
unset SC_SIGNAL_WRITE_CHECK

