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
##  src/sysc/Makefile.am --
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
AM_CFLAGS   += @DEFS@
AM_CXXFLAGS += @DEFS@

SUBDIRS =
H_FILES =
NO_H_FILES =
CXX_FILES =
INCDIRS =
LIBADD =

# include file lists for sub-directories
include kernel/files.am
include communication/files.am
include datatypes/files.am
include tracing/files.am
include utils/files.am

# include individual third-party packages
if WANT_QT_THREADS
include packages/qt.am
endif

localincludedir = $(includedir)/sysc
nobase_localinclude_HEADERS = $(H_FILES)

noinst_LTLIBRARIES = libsysc.la

libsysc_la_SOURCES = $(NO_H_FILES) $(CXX_FILES)

# add dependent (package) libraries
libsysc_la_LIBADD = $(LIBADD)

uninstall-hook:
	test ! -d "$(localincludedir)" || ( set -e ; cd "$(localincludedir)"; \
	  for dir in $(INCDIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir "$(localincludedir)" )

## Taf!
