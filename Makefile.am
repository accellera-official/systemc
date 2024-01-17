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

# support local addition of automake macros
ACLOCAL_AMFLAGS= -I config

SUBDIRS = \
	src \
	examples

DOC_FILES = \
	AUTHORS.md \
	NOTICE \
	CONTRIBUTING.md \
	INSTALL.md \
	LICENSE \
	README.md \
	RELEASENOTES.md \
	cmake/INSTALL_USING_CMAKE.md

EXTRA_DIST = \
	$(DOC_FILES) \
	\
	config/bootstrap \
	config/test.sh.in \
	\
	CMakeLists.txt \
	cmake/SystemCLanguageConfig.cmake.in \
	cmake/SystemCTLMConfig.cmake.in \
	cmake/run_test.cmake \
	\
	msvc16/SystemC/SystemC.sln \
	msvc16/SystemC/SystemC.props \
	msvc16/SystemC/SystemC.vcxproj \
	msvc16/SystemC/SystemC.vcxproj.filters \
	msvc16/SystemC/SystemC-core.vcxproj \
	msvc16/SystemC/SystemC-core.vcxproj.filters

## ****************************************************************************

## install documentation?
if SEPARATE_INSTALL_TREE

rootdoc_DATA = \
	$(DOC_FILES)

uninstall-hook:
	test ! -d "$(docdir)" || rmdir --ignore-fail-on-non-empty "$(docdir)"
	test ! -d "$(prefix)" || rmdir --ignore-fail-on-non-empty "$(prefix)"

endif # SEPARATE_INSTALL_TREE

## ****************************************************************************

## Taf!
