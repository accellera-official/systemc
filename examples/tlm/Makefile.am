## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2004 by all Contributors.
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
##  Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Alan Fitch, Doulos, 2012-03-10
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

include $(top_srcdir)/config/Make-rules.examples

INST_FILES = \
	at_1_phase \
	at_1_phase \
	at_2_phase \
	at_4_phase \
	at_extension_optional \
	at_mixed_targets \
	at_ooo \
	build-msvc \
	build-unix \
	common \
	lt \
	lt_dmi \
	lt_extension_mandatory \
	lt_mixed_endian \
	lt_temporal_decouple

# support local addition of automake macros
ACLOCAL_AMFLAGS= -I config

EXAMPLES_DIR = $(prefix)/examples/tlm

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(EXAMPLES_DIR)
	for file in $(INST_FILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    test -d $(prefix)/$$file \
	    || cp -pr $$d/$$file $(EXAMPLES_DIR) || :; \
	  else \
	    test -f $(prefix)/$$file \
	    || cp -p $$d/$$file $(EXAMPLES_DIR)/$$file || :; \
	  fi; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	w=`pwd`; cd $(srcdir); s=`pwd`; cd $$w; \
	if test $(prefix) != $$s; then \
	  for file in $(INST_FILES); do \
	    d=$(srcdir); \
	    if test -d $$d/$$file; then \
	      rm -rf $(EXAMPLES_DIR)/$$file; \
	    else \
	      rm -f $(EXAMPLES_DIR)/$$file; \
	    fi; \
	  done; \
	fi
	-rmdir $(EXAMPLES_DIR)

## Taf!
