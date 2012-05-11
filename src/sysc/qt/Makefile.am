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
