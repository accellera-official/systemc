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
##  examples/sysc/pipe/Makefile.am --
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

CXX_FILES = \
	display.cpp \
	main.cpp \
	numgen.cpp \
	stage1.cpp \
	stage2.cpp \
	stage3.cpp

H_FILES = \
	display.h \
	numgen.h \
	stage1.h \
	stage2.h \
	stage3.h

check_PROGRAMS = pipe
TESTS = $(check_PROGRAMS) 

pipe_SOURCES = $(CXX_FILES) $(H_FILES)

pipe_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

pipe_INSTALLS = $(pipe_SOURCES) golden.log README

all:
	@echo "To compile an run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(pipe_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(pipe_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
