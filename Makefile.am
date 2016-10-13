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
	NOTICE \
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
