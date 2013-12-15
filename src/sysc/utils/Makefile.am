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
##  src/sysc/utils/Makefile.am --
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

noinst_LTLIBRARIES = libutils.la

H_FILES = \
	sc_hash.h \
	sc_iostream.h \
	sc_list.h \
	sc_machine.h \
	sc_mempool.h \
	sc_pq.h \
	sc_pvector.h \
	sc_report.h \
	sc_report_handler.h \
	sc_string.h \
	sc_temporary.h \
	sc_utils_ids.h \
	sc_vector.h 

NO_H_FILES = \
	sc_stop_here.h

CXX_FILES = \
	sc_hash.cpp \
	sc_list.cpp \
	sc_mempool.cpp \
	sc_pq.cpp \
	sc_report.cpp \
	sc_report_handler.cpp \
	sc_stop_here.cpp \
	sc_string.cpp \
	sc_utils_ids.cpp \
	sc_vector.cpp

localincludedir = $(includedir)/sysc/utils
nobase_localinclude_HEADERS = $(H_FILES)

libutils_la_SOURCES = $(NO_H_FILES) $(CXX_FILES)

## Taf!
