## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2003 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License Version 2.4 (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.systemc.org/. Software distributed by Contributors
##  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
##  ANY KIND, either express or implied. See the License for the specific
##  language governing rights and limitations under the License.
##
## ****************************************************************************
##
##  examples/sysc/simple_bus/Makefile.am --
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

H_FILES = \
	simple_bus.h \
	simple_bus_arbiter.h \
	simple_bus_arbiter_if.h \
	simple_bus_blocking_if.h \
	simple_bus_direct_if.h \
	simple_bus_fast_mem.h \
	simple_bus_master_blocking.h \
	simple_bus_master_direct.h \
	simple_bus_master_non_blocking.h \
	simple_bus_non_blocking_if.h \
	simple_bus_request.h \
	simple_bus_slave_if.h \
	simple_bus_slow_mem.h \
	simple_bus_test.h \
	simple_bus_types.h

CXX_FILES = \
    simple_bus.cpp                     \
    simple_bus_arbiter.cpp             \
    simple_bus_main.cpp                \
    simple_bus_master_blocking.cpp     \
    simple_bus_master_direct.cpp       \
    simple_bus_master_non_blocking.cpp \
    simple_bus_types.cpp               \
    simple_bus_tools.cpp
	
simple_bus_SOURCES = $(CXX_FILES) $(H_FILES)

simple_bus_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

check_PROGRAMS = simple_bus 
TESTS = $(check_PROGRAMS) 

INCLUDES = -I$(prefix)/include

simple_bus_INSTALLS = $(simple_bus_SOURCES) README SLIDES.pdf LEGAL

all:
	@echo "To compile and run this example type"
	@echo "   make check "

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(simple_bus_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(simple_bus_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
