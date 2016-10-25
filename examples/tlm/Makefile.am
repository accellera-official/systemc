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
