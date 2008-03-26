## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2005 by all Contributors.
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
##  src/sysc/packages/boost/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, 05 May 2003
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

include $(top_srcdir)/config/Make-rules

SUBDIRS = \
	bind \
	config \
	detail \
	mpl \
	utility \
	.

H_FILES = \
	assert.hpp \
	bind.hpp \
	checked_delete.hpp \
	config.hpp \
	current_function.hpp \
	get_pointer.hpp \
	mem_fn.hpp \
	non_type.hpp \
	ref.hpp \
	shared_ptr.hpp \
	throw_exception.hpp \
	type.hpp \
	weak_ptr.hpp

noinst_HEADERS = $(H_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/packages/boost

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)
