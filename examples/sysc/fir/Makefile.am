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
##  examples/sysc/fir/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, 17 June 2003
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation and
##  changes you are making here:
##
##  Modifier Name & Affiliation:
##  Description of Modification:
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

H_FILES = \
	display.h    \
	fir.h        \
	fir_const.h  \
	fir_data.h   \
	fir_fsm.h    \
	fir_top.h    \
	stimulus.h

COMMON_CXX_FILES = \
	stimulus.cpp \
	display.cpp 

FIR_CXX_FILES = \
	fir.cpp \
	main.cpp 

fir_SOURCES = $(COMMON_CXX_FILES) $(FIR_CXX_FILES) $(H_FILES)

fir_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

FIR_RTL_CXX_FILES = \
	fir_fsm.cpp \
	fir_data.cpp \
	main_rtl.cpp 

fir_rtl_SOURCES = $(COMMON_CXX_FILES) $(FIR_RTL_CXX_FILES) $(H_FILES)

fir_rtl_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

fir_INSTALLS = \
	$(FIR_CXX_FILES) $(COMMON_CXX_FILES) $(FIR_RTL_CXX_FILES) $(H_FILES)

check_PROGRAMS = fir fir_rtl

TESTS = fir fir_rtl

all:
	@echo "Use make check to compile this example"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(fir_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(fir_INSTALLS); do \
		rm -f ./$$file; \
	done


## Taf!
