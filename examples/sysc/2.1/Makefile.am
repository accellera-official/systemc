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
##  examples/sysc/Makefile.am --
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

SUBDIRS = \
	dpipe \
	forkjoin \
	reset_signal_is \
	sc_export \
	sc_report \
	scx_barrier \
	scx_mutex_w_policy \
	specialized_signals


EXAMPLES_DIR = $(prefix)/examples/sysc/2.1

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(EXAMPLES_DIR)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	-rmdir $(EXAMPLES_DIR)

## Taf!
