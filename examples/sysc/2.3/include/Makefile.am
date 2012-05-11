## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2010 by all Contributors.
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
##  examples/sysc/2.3/include/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, 20 Aug 2010
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

CXX_FILES = 

H_FILES = \
	sc_rvd.h \
	sc_ttd.h

noinst_HEADERS = $(H_FILES) 

INCLUDE_DIR = $(prefix)/examples/sysc/2.3/include

all:

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(noinst_HEADERS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
		rm -f ./$$file; \
	done

## Taf!
