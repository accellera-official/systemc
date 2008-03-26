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

include $(top_srcdir)/config/Make-rules

noinst_LIBRARIES = libqt.a

H_FILES = \
	qt.h \
	qtmd.h

noinst_HEADERS = $(H_FILES)

C_FILES = \
	qt.c \
	qtmdc.c

libqt_a_SOURCES = $(H_FILES) $(C_FILES)

libqt_a_LIBADD = qtmds.o

INCLUDE_DIR = $(prefix)/include/sysc/qt

qt.o qtmdc.c qtmds.s: configuration
qtmds.o: qtmds.s
	$(AS) -o qtmds.o qtmds.s $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES)

configuration:
	@test -d md || ln -s $(srcdir)/md .
	@$(srcdir)/config $(QT_ARCH)

clean:
	-rm -f configuration qtmdc.c qtmds.s
	-test -h md && rm -f md || true

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done
	$(mkinstalldirs) $(INCLUDE_DIR)/md
	list=`echo md/*`; \
	  for file in $$list; do \
	    $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	  done
	$(AR_TOOL) $(top_builddir)/src/$(MAIN_LIBRARY) \
	  $(libqt_a_OBJECTS) $(libqt_a_LIBADD)


uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	list=`echo md/*`; \
	  for file in $$list; do \
	    rm -f $(INCLUDE_DIR)/$$file; \
	  done
	-rmdir $(INCLUDE_DIR)/md
	-rmdir $(INCLUDE_DIR)

## Taf!
