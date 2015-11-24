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
##  Original Author: Philipp A. Hartmann, OFFIS, 2013-05-10
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
##  changes you are making here.
##
##      Name, Affiliation, Date:
##  Description of Modification:
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

all:
	@echo "To compile the TLM examples library type"
	@echo "   make check "

check_LTLIBRARIES = libtlm20examples.la
libtlm20examples_la_SOURCES  = $(CXX_FILES)
libtlm20examples_la_CPPFLAGS = -I$(srcdir)/include $(AM_CPPFLAGS)
libtlm20examples_la_LIBADD   = $(top_builddir)/src/libsystemc.la


H_FILES = \
	include/at_initiator_annotated.h \
	include/at_initiator_explicit.h \
	include/at_target_1_phase_dmi.h \
	include/at_target_1_phase.h \
	include/at_target_2_phase.h \
	include/at_target_4_phase.h \
	include/dmi_memory.h \
	include/extension_initiator_id.h \
	include/lt_dmi_initiator.h \
	include/lt_dmi_target.h \
	include/lt_initiator.h \
	include/lt_synch_target.h \
	include/lt_target.h \
	include/lt_td_initiator.h \
	include/memory.h \
	include/models/README.txt \
	include/models/SimpleBusAT.h \
	include/models/SimpleBusLT.h \
	include/reporting.h \
	include/select_initiator.h \
	include/traffic_generator.h

# present, but currently broken
EXTRA_DIST += \
	include/at_initiator_temporal_decoupling.h

CXX_FILES = \
	src/at_initiator_annotated.cpp \
	src/at_initiator_explicit.cpp \
	src/at_target_1_phase.cpp \
	src/at_target_1_phase_dmi.cpp \
	src/at_target_2_phase.cpp \
	src/at_target_4_phase.cpp \
	src/dmi_memory.cpp \
	src/extension_initiator_id.cpp \
	src/lt_dmi_initiator.cpp \
	src/lt_dmi_target.cpp \
	src/lt_initiator.cpp \
	src/lt_synch_target.cpp \
	src/lt_target.cpp \
	src/lt_td_initiator.cpp \
	src/memory.cpp \
	src/report.cpp \
	src/select_initiator.cpp \
	src/traffic_generator.cpp

# present, but currently broken
EXTRA_DIST += \
	src/at_initiator_temporal_decoupling.cpp

# install examples?
if SEPARATE_INSTALL_TREE

tlm20examplesdir = $(examplesdir)/tlm/common
nobase_tlm20examples_HEADERS  = \
	$(H_FILES) \
	$(CXX_FILES) # source files should be installed as well

else # distribute only

EXTRA_DIST += \
	$(H_FILES) \
	$(CXX_FILES)

endif # SEPARATE_INSTALL_TREE

## Taf!
