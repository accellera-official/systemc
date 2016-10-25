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

include $(top_srcdir)/config/Make-rules.sysc

noinst_LTLIBRARIES = libint.la

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

libint_la_SOURCES = $(H_FILES) $(CXX_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/datatypes/int

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)

## Taf!
