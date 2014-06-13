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
	SystemC_Open_Source_License.pdf

##
## SystemC documentation
##   find sysc -type f -a -not -name "*.in" -printf "\t%p \\\\\n"
DOC_FILES += \
	sysc/README \
	sysc/archived/README \
	sysc/archived/FuncSpec20.pdf \
	sysc/archived/WhitePaper20.pdf \
	sysc/archived/UserGuide20.pdf \
	sysc/archived/SystemC_2_1_overview.pdf \
	sysc/archived/SystemC_2_1_features.pdf \
	sysc/SystemC_2011_New_Features.pdf

##
## TLM-2.0 documentation
##   find tlm -type f -a -not -name "*.in" -printf "\t%p \\\\\n"
##
DOC_FILES += \
	tlm/ChangeLog \
	tlm/release/TLM_2_0_requirements.pdf \
	tlm/release/TLM_2_0_white_paper.pdf \
	tlm/release/README.txt \
	tlm/release/TLM_2_0_presentation.ppt \
	tlm/release/TLM_2_0_presentation.pdf \
	tlm/tlm_README

## ****************************************************************************

## install documentation?
if SEPARATE_INSTALL_TREE

nobase_doc_DATA = \
	$(DOC_FILES)

## Install, but do not distribute Doxygen configuration
nobase_doc_DATA += \
	sysc/doxygen/Doxyfile \
	tlm/doxygen/Doxyfile

endif # SEPARATE_INSTALL_TREE

## add documentation to distribution
EXTRA_DIST += \
	$(DOC_FILES)

doxygen: sysc/doxygen/Doxyfile tlm/doxygen/Doxyfile
	@(cd sysc/doxygen && $(DOXYGEN))
	@(cd tlm/doxygen  && $(DOXYGEN))

%/Doxyfile: %/Doxyfile.in
	(cd $(top_builddir) && $(SHELL) config.status docs/$@)

## Taf!
