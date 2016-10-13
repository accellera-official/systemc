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
##  examples/sysc/risc_cpu/Makefile.am --
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
	bios.h \
	dcache.h \
	decode.h \
	directive.h \
	exec.h \
	fetch.h \
	floating.h \
	icache.h \
	mmxu.h \
	paging.h \
	pic.h

CXX_FILES = \
	bios.cpp \
	dcache.cpp \
	decode.cpp \
	exec.cpp \
	fetch.cpp \
	floating.cpp \
	icache.cpp \
	main.cpp \
	mmxu.cpp \
	paging.cpp \
	pic.cpp

risc_cpu_EXTRAS = \
	README \
	README_TYPESCRIPT \
	abc.asm \
	assembler.pl \
	bios \
	dcache \
	icache \
	printout \
	program \
	register \
	test.asm \
	test1.asm

risc_cpu_SOURCES = $(CXX_FILES) $(H_FILES)

risc_cpu_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

check_PROGRAMS = risc_cpu
TESTS = $(check_PROGRAMS) 

risc_cpu_INSTALLS = $(risc_cpu_SOURCES) $(risc_cpu_EXTRAS)


all:
	@echo "To compile and run this example type"
	@echo "   make check "

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(risc_cpu_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(risc_cpu_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
