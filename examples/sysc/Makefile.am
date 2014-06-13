## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2014 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.accellera.org/. Software distributed by Contributors
##  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
##  ANY KIND, either express or implied. See the License for the specific
##  language governing rights and limitations under the License.
##
## ****************************************************************************
##
##  examples/sysc/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Martin Janssen, Synopsys, Inc.
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
##  changes you are making here.
##
##      Name, Affiliation, Date: Philipp Hartmann, OFFIS, 2013-05-20
##  Description of Modification: Rewritten for (mostly) non-recursive build,
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

all:
	@echo "To compile and run the examples type"
	@echo "   make check "

## use a wrapper script to check expected outputs
TESTS = $(check_SCRIPTS)
check_SCRIPTS  = $(examples_TESTS:=.sh)
check_PROGRAMS = $(examples_TESTS)

## ************************************************************************
## additional data for installation

syscexamplesdir = $(examplesdir)/sysc

examples_FILES = \
	Makefile.all \
	Makefile.config \
	Makefile.rules \
	README.txt \
	\
	fft/README

## ************************************************************************

examples_INPUT =input.txt
examples_GOLDEN=golden.log

## filled later
examples_BUILD =
examples_CLEAN =
examples_TESTS =

##
## list of SystemC examples
##

## main examples

include fft/fft_flpt/test.am
include fft/fft_fxpt/test.am
include fir/test.am
include pipe/test.am
include pkt_switch/test.am
include risc_cpu/test.am
include rsa/test.am
include simple_bus/test.am
include simple_fifo/test.am
include simple_perf/test.am

## 2.1 examples

include 2.1/dpipe/test.am
include 2.1/forkjoin/test.am
include 2.1/reset_signal_is/test.am
include 2.1/sc_export/test.am
include 2.1/sc_report/test.am
include 2.1/scx_barrier/test.am
include 2.1/scx_mutex_w_policy/test.am
include 2.1/specialized_signals/test.am

## 2.3 examples

include 2.3/sc_rvd/test.am
include 2.3/sc_ttd/test.am

## ************************************************************************

# install examples?
if SEPARATE_INSTALL_TREE
nobase_syscexamples_DATA = \
	$(examples_FILES)
endif

EXTRA_DIST += \
	$(examples_FILES)

CLEANFILES += \
	$(examples_CLEAN) \
	$(check_SCRIPTS)

## Taf!
