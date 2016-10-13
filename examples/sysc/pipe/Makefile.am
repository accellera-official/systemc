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
##  examples/sysc/pipe/Makefile.am --
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

CXX_FILES = \
	display.cpp \
	main.cpp \
	numgen.cpp \
	stage1.cpp \
	stage2.cpp \
	stage3.cpp

H_FILES = \
	display.h \
	numgen.h \
	stage1.h \
	stage2.h \
	stage3.h

check_PROGRAMS = pipe
TESTS = $(check_PROGRAMS) 

pipe_SOURCES = $(CXX_FILES) $(H_FILES)

pipe_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

pipe_INSTALLS = $(pipe_SOURCES) golden.log README

all:
	@echo "To compile an run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(pipe_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(pipe_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
