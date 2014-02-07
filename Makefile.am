## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2014 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.accellera.org/. Software distributed by Contributors
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

# support local addition of automake macros
ACLOCAL_AMFLAGS= -I config

SUBDIRS = \
	docs \
	src \
	examples

DOC_FILES = \
	AUTHORS \
	COPYING \
	ChangeLog \
	INSTALL \
	LICENSE \
	NEWS \
	README \
	RELEASENOTES

EXTRA_DIST = \
	$(DOC_FILES) \
	\
	config/bootstrap \
	config/test.sh.in \
	\
	msvc80/SystemC/SystemC.sln \
	msvc80/SystemC/SystemC.vcproj \
	msvc80/SystemC/SystemC-examples.sln

## ****************************************************************************

## install documentation?
if SEPARATE_INSTALL_TREE

rootdoc_DATA = \
	$(DOC_FILES)

endif # SEPARATE_INSTALL_TREE

## ****************************************************************************

doxygen:
	@(cd docs && $(MAKE) $@)

## Taf!
