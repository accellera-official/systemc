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
