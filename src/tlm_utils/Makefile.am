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
##  src/tlm_utils/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Alan Fitch, Doulos, 2012-03-10
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

include $(top_srcdir)/config/Make-rules.sysc

H_FILES = \
	instance_specific_extensions.h \
	multi_passthrough_initiator_socket.h \
	multi_passthrough_target_socket.h \
	multi_socket_bases.h \
	passthrough_target_socket.h \
	peq_with_cb_and_phase.h \
	peq_with_get.h \
	simple_initiator_socket.h \
	simple_target_socket.h \
	tlm_quantumkeeper.h

localincludedir = $(includedir)/tlm_utils
nobase_localinclude_HEADERS = $(H_FILES)

EXTRA_DIST += \
	README.txt
