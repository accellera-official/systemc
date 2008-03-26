## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2003 by all Contributors.
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
##  examples/sysc/fft/fft_flpt/Makefile.am --
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

H_FILES = \
	fft.h \
	sink.h \
	source.h

CXX_FILES = \
	fft.cpp \
	main.cpp \
	sink.cpp \
	source.cpp

IMAGE_FILES = \
	in_imag \
	in_imag.1 \
	in_imag.2 \
	in_imag.3 \
	in_imag.4 \
	in_real \
	in_real.1 \
	in_real.2 \
	in_real.3 \
	in_real.4 \
	out_imag.1.golden \
	out_imag.2.golden \
	out_imag.3.golden \
	out_imag.4.golden \
	out_real.1.golden \
	out_real.2.golden \
	out_real.3.golden \
	out_real.4.golden 

fft_SOURCES = $(CXX_FILES) $(H_FILES)

fft_LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

check_PROGRAMS = fft

INCLUDES = -I$(prefix)/include

fft_INSTALLS = $(IMAGE_FILES) $(fft_SOURCES)


all:
	@echo "To compile and run this example type"
	@echo "   make check "

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(fft_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(fft_INSTALLS); do \
		rm -f ./$$file; \
	done

## Taf!
