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
##  src/sysc/kernel/Makefile.am --
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

include $(top_srcdir)/config/Make-rules.sysc

noinst_LTLIBRARIES = libkernel.la

H_FILES = \
	sc_attribute.h \
	sc_boost.h \
	sc_cmnhdr.h \
	sc_constants.h \
	sc_cor.h \
	sc_dynamic_processes.h \
	sc_event.h \
	sc_except.h \
	sc_externs.h \
	sc_join.h \
	sc_kernel_ids.h \
	sc_macros.h \
	sc_module.h \
	sc_module_name.h \
	sc_object.h \
	sc_process.h \
	sc_process_handle.h \
	sc_runnable.h \
	sc_sensitive.h \
	sc_spawn.h \
	sc_spawn_options.h \
	sc_status.h \
	sc_simcontext.h \
	sc_time.h \
	sc_ver.h \
	sc_wait.h \
	sc_wait_cthread.h 

NO_H_FILES = \
	sc_cor_fiber.h \
	sc_cor_pthread.h \
	sc_cor_qt.h \
	sc_cthread_process.h \
	sc_method_process.h \
	sc_module_registry.h \
	sc_name_gen.h \
	sc_object_int.h \
	sc_object_manager.h \
	sc_phase_callback_registry.h \
	sc_reset.h \
	sc_runnable_int.h \
	sc_simcontext_int.h \
	sc_thread_process.h 

CXX_FILES = \
	sc_attribute.cpp \
	$(CXX_COR_FILES) \
	sc_cthread_process.cpp \
	sc_event.cpp \
	sc_except.cpp \
	sc_join.cpp \
	sc_main.cpp \
	sc_main_main.cpp \
	sc_method_process.cpp \
	sc_module.cpp \
	sc_module_name.cpp \
	sc_module_registry.cpp \
	sc_name_gen.cpp \
	sc_object.cpp \
	sc_object_manager.cpp \
	sc_phase_callback_registry.cpp \
	sc_process.cpp \
	sc_reset.cpp \
	sc_sensitive.cpp \
	sc_simcontext.cpp \
	sc_spawn_options.cpp \
	sc_thread_process.cpp \
	sc_time.cpp \
	sc_ver.cpp \
	sc_wait.cpp \
	sc_wait_cthread.cpp

# co-routine implementation
if WANT_QT_THREADS
CXX_COR_FILES = sc_cor_qt.cpp
else
if WANT_PTHREADS_THREADS
CXX_COR_FILES = sc_cor_pthread.cpp
else
CXX_COR_FILES = sc_cor_fiber.cpp
endif
endif # co-routine implementation

localincludedir = $(includedir)/sysc/kernel
nobase_localinclude_HEADERS = $(H_FILES)

libkernel_la_SOURCES = $(NO_H_FILES) $(CXX_FILES)

## Taf!
