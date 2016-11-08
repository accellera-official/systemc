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
	sc_tracing_ids.h

NO_H_FILES = \
	sc_trace_file_base.h \
	sc_vcd_trace.h \
	sc_wif_trace.h

CXX_FILES = \
	sc_trace.cpp \
	sc_trace_file_base.cpp \
	sc_vcd_trace.cpp \
	sc_wif_trace.cpp

libtracing_la_SOURCES = $(H_FILES) $(NO_H_FILES) $(CXX_FILES)

localincludedir = $(includedir)/sysc/tracing
nobase_localinclude_HEADERS = $(H_FILES)

## Taf!
