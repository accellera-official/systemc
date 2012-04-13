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

DOXYGEN=doxygen

SUBDIRS = \
	src \
	examples \
	.

INST_FILES = \
	AUTHORS \
	COPYING \
	ChangeLog \
	INSTALL \
	LICENSE \
	NEWS \
	README \
	RELEASENOTES \
	docs

# support local addition of automake macros
ACLOCAL_AMFLAGS= -I config

install-data-local:
	@$(NORMAL_INSTALL)
	for file in $(INST_FILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    test -d $(prefix)/$$file \
	    || cp -pr $$d/$$file $(prefix) || :; \
	  else \
	    test -f $(prefix)/$$file \
	    || cp -p $$d/$$file $(prefix)/$$file || :; \
	  fi; \
	done
#@(cd ./examples; $(MAKE) install;)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	w=`pwd`; cd $(srcdir); s=`pwd`; cd $$w; \
	if test $(prefix) != $$s; then \
	  for file in $(INST_FILES); do \
	    d=$(srcdir); \
	    if test -d $$d/$$file; then \
	      rm -rf $(prefix)/$$file; \
	    else \
	      rm -f $(prefix)/$$file; \
	    fi; \
	  done; \
	fi
	@(cd ./examples; $(MAKE) uninstall;)

doxygen:
	@(cd docs/sysc/doxygen && $(DOXYGEN))
	@(cd docs/tlm/doxygen && $(DOXYGEN))

## Taf!
