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
##  examples/sysc/2.1/specialized_signals/Makefile.am --
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

CXX_FILES = \
	main.cpp \
	scx_signal_int.cpp \
	scx_signal_uint.cpp \
	scx_signal_signed.cpp \
	scx_signal_unsigned.cpp 

H_FILES = \
	scx_signal_int.h \
	scx_signal_uint.h \
	scx_signal_signed.h \
	scx_signal_unsigned.h 

check_PROGRAMS = specialized_signals
TESTS = $(check_PROGRAMS) 

INCLUDES = -I$(prefix)/include

specialized_signals_SOURCES = $(CXX_FILES) $(H_FILES)

specialized_signals_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

specialized_signals_INSTALLS = $(specialized_signals_SOURCES) 


all:
	@echo "To compile and run this example type"
	@echo "   make check "

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(specialized_signals_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(specialized_signals_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
