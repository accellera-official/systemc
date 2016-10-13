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
##  src/sysc/qt/Makefile.am --
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


H_FILES = \
	qt.h \
	qtmd.h \
	md/axp.h \
	md/hppa.h \
	md/i386.h \
	md/iX86_64.h \
	md/ksr1.h \
	md/m88k.h \
	md/mips.h \
	md/powerpc_mach.h \
	md/powerpc_sys5.h \
	md/sparc.h \
	md/vax.h

noinst_HEADERS = $(H_FILES)

C_FILES = \
	qt.c \
	qtmdc.c

S_FILES = \
       qtmds.s

if !WANT_PTHREADS_THREADS

noinst_LTLIBRARIES = libqt.la

libqt_la_SOURCES = $(H_FILES) $(C_FILES) $(S_FILES)

libqt_la_LIBTOOLFLAGS = --tag=CC

qtmds_o_CCASFLAGS = $(AM_CCASFLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES)

INCLUDE_DIR = $(prefix)/include/sysc/qt

qt.o qtmdc.c qtmds.s: configuration

if QT_ARCH_X86
configuration: md/i386.s md/null.c
endif
if QT_ARCH_X86_64
configuration: md/iX86_64.s md/null.c
endif
if QT_ARCH_POWERPC_MACOSX
configuration: md/powerpc_mach.s md/powerpc.c
endif
if QT_ARCH_SPARC
configuration: md/sparc.s md/null.c
endif
if QT_ARCH_HPPA
configuration: md/hppa.s md/null.c
endif
if QT_ARCH_PTHREADS
configuration: md/null.s md/null.c
endif
	@test -d md || ln -s $(srcdir)/md .
	@$(srcdir)/config $(QT_ARCH)

endif !WANT_PTHREADS_THREADS

clean-local:
	-rm -f configuration qtmdc.c qtmds.s
	-test -h md && rm -f md || true

install-data-local:
	@$(NORMAL_INSTALL)


uninstall-local:
	@$(NORMAL_UNINSTALL)

## Taf!
