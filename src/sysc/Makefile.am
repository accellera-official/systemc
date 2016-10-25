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

SUBDIRS = \
	kernel \
	qt \
	communication \
	tracing \
	utils \
	packages \
	datatypes \
	.

H_FILES = 

noinst_HEADERS = $(H_FILES)

CXX_FILES =

NO_CXX_FILES =

INCLUDE_DIR = $(prefix)/include

libdir = $(prefix)/lib-$(TARGET_ARCH)

lib_LTLIBRARIES = libsystemc.la

libsystemc_la_SOURCES =

if !WANT_PTHREADS_THREADS
QT_LIB=qt/libqt.la
endif

libsystemc_la_LIBADD = \
       kernel/libkernel.la \
       $(QT_LIB) \
       communication/libcommunication.la \
       tracing/libtracing.la \
       utils/libutils.la \
       datatypes/bit/libbit.la \
       datatypes/fx/libfx.la \
       datatypes/int/libint.la \
       datatypes/misc/libmisc.la 

libsystemc_la_LDFLAGS = -release $(VERSION) -L$(prefix)/lib-$(TARGET_ARCH)

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done
	$(mkinstalldirs) $(prefix)/lib-$(TARGET_ARCH)
	$(INSTALL_DATA) $(MAIN_LIBRARY) \
	    $(prefix)/lib-$(TARGET_ARCH)/$(MAIN_LIBRARY)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)
	rm -f $(prefix)/lib-$(TARGET_ARCH)/$(MAIN_LIBRARY)
	-rmdir $(prefix)/lib-$(TARGET_ARCH)

## Taf!
