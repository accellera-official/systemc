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
##  Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Alan Fitch, Doulos, 2012-03-10
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
##  changes you are making here.
##
##      Name, Affiliation, Date: Philipp Hartmann, OFFIS, 2013-05-20
##  Description of Modification: Rewritten for (mostly) non-recursive build.
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

# override package string for TLM tests
PACKAGE_STRING = "TLM $(TLM_PACKAGE_VERSION)"

SUBDIRS = \
	common \
	.

all:
	@echo "To compile and run the examples type"
	@echo "   make check "

## use a wrapper script to check expected outputs
TESTS = $(check_SCRIPTS)
check_SCRIPTS  = $(examples_TESTS:=.sh)
check_PROGRAMS = $(examples_TESTS)

## ************************************************************************
## additional data for installation

tlmexamplesdir = $(examplesdir)/tlm

examples_FILES = \
	build-msvc/Makefile \
	build-msvc/Makefile.config \
	build-msvc/Makefile.rules \
	build-msvc/openhere.bat \
	build-msvc/READMEcommandlinemsvc.txt \
	build-msvc/systemc.vsprops \
	build-msvc/tlm.sln \
	build-msvc/vsvars.bat \
	\
	build-unix/Makefile \
	build-unix/Makefile.config \
	build-unix/Makefile.rules \
	\
	README.txt

## ************************************************************************

## shared build setup for TLM examples
CPPFLAGS_TLMTESTS = -I$(srcdir)/common/include $(AM_CPPFLAGS)
LDADD            += common/libtlm20examples.la

examples_INPUT =results/input.txt
examples_GOLDEN=results/expected.log

## filled later
examples_BUILD =
examples_CLEAN =
examples_TESTS =

##
## list of TLM examples
##

include at_1_phase/test.am
include at_2_phase/test.am
include at_4_phase/test.am
include at_extension_optional/test.am
include at_mixed_targets/test.am
include at_ooo/test.am
include lt/test.am
include lt_dmi/test.am
include lt_extension_mandatory/test.am
include lt_mixed_endian/test.am
include lt_temporal_decouple/test.am

## ************************************************************************

# install examples?
if SEPARATE_INSTALL_TREE
nobase_tlmexamples_DATA = \
	$(examples_FILES)
endif

EXTRA_DIST += \
	$(examples_FILES)

CLEANFILES += \
	$(examples_CLEAN) \
	$(check_SCRIPTS)

## Taf!
