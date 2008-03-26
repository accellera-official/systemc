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
##  src/sysc/communication/Makefile.am --
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

include $(top_srcdir)/config/Make-rules

noinst_LIBRARIES = libcommunication.a

H_FILES = \
	sc_buffer.h \
	sc_clock.h \
	sc_clock_ports.h \
	sc_communication_ids.h \
	sc_event_finder.h \
	sc_event_queue.h \
	sc_export.h \
	sc_fifo.h \
	sc_fifo_ifs.h \
	sc_fifo_ports.h \
	sc_interface.h \
	sc_mutex.h \
	sc_mutex_if.h \
	sc_port.h \
	sc_prim_channel.h \
	sc_semaphore.h \
	sc_semaphore_if.h \
	sc_signal.h \
	sc_signal_ifs.h \
	sc_signal_ports.h \
	sc_signal_resolved.h \
	sc_signal_resolved_ports.h \
	sc_signal_rv.h \
	sc_signal_rv_ports.h 

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	sc_clock.cpp \
	sc_event_finder.cpp \
	sc_event_queue.cpp \
	sc_export.cpp \
	sc_interface.cpp \
	sc_mutex.cpp \
	sc_port.cpp \
	sc_prim_channel.cpp \
	sc_semaphore.cpp \
	sc_signal.cpp \
	sc_signal_ports.cpp \
	sc_signal_resolved.cpp \
	sc_signal_resolved_ports.cpp 

NO_CXX_FILES =

libcommunication_a_SOURCES = $(H_FILES) $(CXX_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/communication

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done
	$(AR_TOOL) $(top_builddir)/src/$(MAIN_LIBRARY) \
	  $(libcommunication_a_OBJECTS)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)

## Taf!
