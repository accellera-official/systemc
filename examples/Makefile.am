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
##  examples/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Martin Janssen, Synopsys, Inc.
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

SUBDIRS = \
	sysc \
	tlm

examples_FILES = \
	build-unix/Makefile \
	build-unix/Makefile.config \
	build-unix/Makefile.rules \
	\
	build-msvc/Makefile \
	build-msvc/Makefile.config \
	build-msvc/Makefile.rules \
	build-msvc/READMEcommandlinemsvc.txt \
	build-msvc/SystemC_examples.props \
	build-msvc/SystemC_examples.sln \
	build-msvc/openhere.bat \
	build-msvc/tlm_examples.props \
	build-msvc/tlm_examples.sln \
	build-msvc/vsvars.bat

# install examples?
if SEPARATE_INSTALL_TREE
buildexamplesdir = $(examplesdir)
nobase_buildexamples_DATA = \
	$(examples_FILES)

examples_DIRS = \
	build-unix \
	build-msvc

uninstall-hook:
	test ! -d "$(buildexamplesdir)" || ( set -e ;  cd "$(buildexamplesdir)" ; \
	  for dir in $(examples_DIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir --ignore-fail-on-non-empty "$(buildexamplesdir)" )

endif # SEPARATE_INSTALL_TREE

EXTRA_DIST += \
	CMakeLists.txt \
	\
	$(examples_FILES)

## Taf!
