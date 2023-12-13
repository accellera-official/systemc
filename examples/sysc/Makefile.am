## ****************************************************************************
##
##  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
##  more contributor license agreements.  See the NOTICE file distributed
##  with this work for additional information regarding copyright ownership.
##  Accellera licenses this file to you under the Apache License, Version 2.0
##  (the "License"); you may not use this file except in compliance with the
##  License.  You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
##  implied.  See the License for the specific language governing
##  permissions and limitations under the License.
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
examples_DIRS  =

##
## list of SystemC examples
##

## main examples

include fft/fft_flpt/test.am
include fft/fft_fxpt/test.am
examples_DIRS += fft

include fir/test.am
include pipe/test.am
include pkt_switch/test.am
include risc_cpu/test.am
include rsa/test.am
include simple_bus/test.am
include simple_fifo/test.am
include simple_perf/test.am
include async_suspend/test.am

## 2.1 examples

include 2.1/dpipe/test.am
include 2.1/forkjoin/test.am
include 2.1/reset_signal_is/test.am
include 2.1/sc_export/test.am
include 2.1/sc_report/test.am
include 2.1/scx_barrier/test.am
include 2.1/scx_mutex_w_policy/test.am
include 2.1/specialized_signals/test.am
examples_DIRS += 2.1

## 2.3 examples

include 2.3/sc_rvd/test.am
include 2.3/sc_ttd/test.am
include 2.3/simple_async/test.am
examples_DIRS += 2.3/include
examples_DIRS += 2.3

## ************************************************************************

# install examples?
if SEPARATE_INSTALL_TREE
nobase_syscexamples_DATA = \
	$(examples_FILES)

uninstall-hook:
	test ! -d "$(syscexamplesdir)" || ( set -e ; cd "$(syscexamplesdir)" ; \
	  for dir in $(examples_DIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir "$(syscexamplesdir)" )

endif # SEPARATE_INSTALL_TREE

EXTRA_DIST += \
	$(examples_FILES) \
	CMakeLists.txt

CLEANFILES += \
	$(examples_CLEAN) \
	$(check_SCRIPTS)

## Taf!
