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
##  Original Author: Philipp A. Hartmann, OFFIS, 2013-05-20
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

include $(top_srcdir)/config/Make-rules.common

DOXYGEN=doxygen

## Core documentation (see also root directory)

DOC_FILES = \
	DEVELOPMENT.md \
	git_branches.svg

##
## SystemC documentation
##   find sysc -type f -a -not -name "*.in" -printf "\t%p \\\\\n"
DOC_FILES += \
	sysc/README
DOC_FILES += \
	sysc/SIGN_EXTENSION.md

##
## TLM-2.0 documentation
##   find tlm -type f -a -not -name "*.in" -printf "\t%p \\\\\n"
##
DOC_FILES += \
	tlm/tlm_README.md

## ****************************************************************************

## install documentation?
if SEPARATE_INSTALL_TREE

nobase_doc_DATA = \
	$(DOC_FILES)

## Install, but do not distribute Doxygen configuration
nobase_doc_DATA += \
	sysc/doxygen/Doxyfile \
	tlm/doxygen/Doxyfile

DOCDIRS = \
	sysc/doxygen \
	sysc \
	tlm/doxygen \
	tlm

uninstall-hook:
	test ! -d "$(docdir)" || ( set -e ; cd "$(docdir)" ; \
	  for dir in $(DOCDIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir --ignore-fail-on-non-empty "$(docdir)" )

endif # SEPARATE_INSTALL_TREE

## add documentation and CMake scripts to distribution
EXTRA_DIST += \
	$(DOC_FILES) \
	CMakeLists.txt

doxygen: sysc/doxygen/Doxyfile tlm/doxygen/Doxyfile
	@(cd sysc/doxygen && $(DOXYGEN))
	@(cd tlm/doxygen  && $(DOXYGEN))

%/Doxyfile: %/Doxyfile.in
	(cd $(top_builddir) && $(SHELL) config.status docs/$@)

## Taf!
