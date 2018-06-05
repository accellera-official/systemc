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
	README.txt

## ************************************************************************

TLM_EXAMPLE_SUBDIRS = {build-msvc,build-unix,docs,include,src,results,}

## shared build setup for TLM examples
CPPFLAGS_TLMTESTS = -I$(srcdir)/common/include $(AM_CPPFLAGS)
LDADD            += common/libtlm20examples.la

examples_INPUT =results/input.txt
examples_GOLDEN=results/expected.log

## filled later
examples_BUILD =
examples_CLEAN =
examples_TESTS =
examples_DIRS  =

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

examples_DIRS += common/include/models
examples_DIRS += common/include
examples_DIRS += common/src
examples_DIRS += common

## ************************************************************************

# install examples?
if SEPARATE_INSTALL_TREE
nobase_tlmexamples_DATA = \
	$(examples_FILES)

uninstall-hook:
	test ! -d "$(tlmexamplesdir)" || ( set -e ; cd "$(tlmexamplesdir)" ; \
	  for dir in $(examples_DIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir "$(tlmexamplesdir)" )

endif # SEPARATE_INSTALL_TREE

EXTRA_DIST += \
	$(examples_FILES) \
	CMakeLists.txt

CLEANFILES += \
	$(examples_CLEAN) \
	$(check_SCRIPTS)

## Taf!
