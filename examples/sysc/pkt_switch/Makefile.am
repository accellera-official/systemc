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
##  examples/sysc/pkt_switch/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, 17 June 2003
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation and
##  changes you are making here:
##
##  Modifier Name & Affiliation:
##  Description of Modification:
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

H_FILES = \
	fifo.h \
	pkt.h \
	receiver.h \
	sender.h \
	switch.h \
	switch_clk.h \
	switch_reg.h

CXX_FILES = \
	fifo.cpp \
	main.cpp \
	receiver.cpp \
	sender.cpp \
	switch.cpp \
	switch_clk.cpp

pkt_switch_SOURCES = $(CXX_FILES) $(H_FILES)

pkt_switch_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

check_PROGRAMS = pkt_switch 
TESTS = $(check_PROGRAMS) 

pkt_switch_INSTALLS = $(pkt_switch_SOURCES) README

all:
	@echo "To compile and run this example type"
	@echo "   make check "

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(pkt_switch_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(pkt_switch_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
