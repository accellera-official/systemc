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

# order of SUBDIRS is important, sysc needs to go first:
SUBDIRS = \
	sysc \
	tlm_core  \
	tlm_utils \
	.

H_FILES = \
	systemc \
	systemc.h \
	tlm \
	tlm.h

nobase_include_HEADERS = $(H_FILES)

libarchdir = $(libdir)$(LIB_ARCH_SUFFIX)
libarch_LTLIBRARIES = libsystemc.la

pkgconfigdir = $(libarchdir)/pkgconfig
pkgconfig_DATA = \
	systemc.pc \
	tlm.pc

EXTRA_DIST += \
	README_TLM.txt \
	CMakeLists.txt

# no own sources here
libsystemc_la_SOURCES =

# dummy C++ source to cause C++ linking
nodist_EXTRA_libsystemc_la_SOURCES = sc_nonexistent.cpp

# add main libraries
libsystemc_la_LIBADD = \
	tlm_utils/libtlm_utils.la \
	tlm_core/libtlm_core.la \
	sysc/libsysc.la

# either for async_update locking or pthread processes
if USES_PTHREADS_LIB
# Libtool/GCC do not play well together on some platforms
# with C++ libraries and libpthread dependency, see e.g.
# http://lists.gnu.org/archive/html/libtool/2012-02/msg00003.html
libsystemc_la_LIBADD+=$(EXPLICIT_LPTHREAD)
endif

libsystemc_la_LDFLAGS = $(EXTRA_LDFLAGS) -release $(VERSION)

uninstall-hook:
	test ! -d "$(includedir)"   || rmdir --ignore-fail-on-non-empty "$(includedir)"
	test ! -d "$(pkgconfigdir)" || rmdir --ignore-fail-on-non-empty "$(pkgconfigdir)"
	test ! -d "$(libarchdir)"   || rmdir --ignore-fail-on-non-empty "$(libarchdir)"

## Taf!
