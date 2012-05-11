## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2005 by all Contributors.
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
##  src/sysc/tracing/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21
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

include $(top_srcdir)/config/Make-rules.sysc

noinst_LTLIBRARIES = libtracing.la

H_FILES = \
	sc_trace.h \
	sc_vcd_trace.h \
	sc_wif_trace.h

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	sc_trace.cpp \
	sc_vcd_trace.cpp \
	sc_wif_trace.cpp

NO_CXX_FILES =

libtracing_la_SOURCES = $(H_FILES) $(CXX_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/tracing

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)

## Taf!
