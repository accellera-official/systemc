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
##  src/sysc/packages/boost/detail/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, Inc., 2003-05-21
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

noinst_LIBRARIES = 

H_FILES = \
	algorithm.hpp \
	allocator.hpp \
	atomic_count.hpp \
	atomic_count_gcc.hpp \
	atomic_count_linux.hpp \
	atomic_count_pthreads.hpp \
	atomic_count_win32.hpp \
	binary_search.hpp \
	call_traits.hpp \
	catch_exceptions.hpp \
	compressed_pair.hpp \
	dynamic_bitset.hpp \
	iterator.hpp \
	lightweight_mutex.hpp \
	lightweight_test.hpp \
	limits.hpp \
	lwm_gcc.hpp \
	lwm_irix.hpp \
	lwm_linux.hpp \
	lwm_nop.hpp \
	lwm_pthreads.hpp \
	lwm_win32.hpp \
	lwm_win32_cs.hpp \
	named_template_params.hpp \
	numeric_traits.hpp \
	ob_call_traits.hpp \
	ob_compressed_pair.hpp \
	select_type.hpp \
	shared_array_nmt.hpp \
	shared_count.hpp \
	shared_ptr_nmt.hpp \
	winapi.hpp 

noinst_HEADERS = $(H_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/packages/boost/detail

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

## Taf!
