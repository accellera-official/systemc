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

SUBDIRS = \
	kernel \
	qt \
	communication \
	tracing \
	utils \
	packages/boost \
	datatypes \
	.

H_FILES = 

CXX_FILES =

localincludedir = $(includedir)/sysc
nobase_localinclude_HEADERS = $(H_FILES)

libarchdir = $(libdir)$(LIB_ARCH_SUFFIX)
libarch_LTLIBRARIES = libsystemc.la

libsystemc_la_SOURCES =

# dummy C++ source to cause C++ linking
nodist_EXTRA_libsystemc_la_SOURCES = sc_nonexistent.cpp

if !WANT_PTHREADS_THREADS
QT_LIB=qt/libqt.la
endif

libsystemc_la_LIBADD = \
       kernel/libkernel.la \
       $(QT_LIB) \
       communication/libcommunication.la \
       tracing/libtracing.la \
       utils/libutils.la \
       datatypes/libdatatypes.la

libsystemc_la_LDFLAGS = -release $(VERSION) -L$(libarchdir)

## Taf!
