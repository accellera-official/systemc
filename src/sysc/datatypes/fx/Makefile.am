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
##  src/sysc/datatypes/fx/Makefile.am --
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

noinst_LTLIBRARIES = libfx.la

H_FILES = \
	fx.h \
	sc_context.h \
	sc_fix.h \
	sc_fixed.h \
	sc_fx_ids.h \
	sc_fxcast_switch.h \
	sc_fxdefs.h \
	sc_fxnum.h \
	sc_fxnum_observer.h \
	sc_fxtype_params.h \
	sc_fxval.h \
	sc_fxval_observer.h \
	sc_ufix.h \
	sc_ufixed.h \
	scfx_ieee.h \
	scfx_mant.h \
	scfx_other_defs.h \
	scfx_params.h \
	scfx_rep.h \
	scfx_string.h \
	scfx_utils.h

NO_H_FILES = \
	scfx_pow10.h

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	sc_fxcast_switch.cpp \
	sc_fxdefs.cpp \
	sc_fxnum.cpp \
	sc_fxnum_observer.cpp \
	sc_fxtype_params.cpp \
	sc_fxval.cpp \
	sc_fxval_observer.cpp \
	scfx_mant.cpp \
	scfx_pow10.cpp \
	scfx_rep.cpp \
	scfx_utils.cpp

NO_CXX_FILES =

libfx_la_SOURCES = $(H_FILES) $(CXX_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/datatypes/fx

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
