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
##  src/sysc/datatypes/int/Makefile.am --
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

noinst_LIBRARIES = libint.a

H_FILES = \
	sc_bigint.h \
	sc_biguint.h \
	sc_int.h \
	sc_int_base.h \
	sc_int_ids.h \
	sc_length_param.h \
	sc_nbdefs.h \
	sc_nbexterns.h \
	sc_nbutils.h \
	sc_signed.h \
	sc_uint.h \
	sc_uint_base.h \
	sc_unsigned.h

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	sc_int_base.cpp \
	sc_int32_mask.cpp \
        sc_int64_io.cpp \
	sc_int64_mask.cpp \
	sc_length_param.cpp \
	sc_nbdefs.cpp \
	sc_nbexterns.cpp \
	sc_nbutils.cpp \
	sc_signed.cpp \
	sc_uint_base.cpp \
	sc_unsigned.cpp

libint_a_SOURCES = $(H_FILES) $(CXX_FILES) 

INCLUDE_DIR = $(prefix)/include/sysc/datatypes/int

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done
	$(AR_TOOL) $(top_builddir)/src/$(MAIN_LIBRARY) \
	  $(libint_a_OBJECTS)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)

## Taf!
