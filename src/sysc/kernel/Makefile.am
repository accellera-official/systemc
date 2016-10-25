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
	sc_reset.h \
	sc_runnable.h \
	sc_sensitive.h \
	sc_spawn.h \
	sc_spawn_options.h \
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
	sc_macros_int.h \
	sc_method_process.h \
	sc_module_registry.h \
	sc_name_gen.h \
	sc_object_manager.h \
	sc_runnable_int.h \
	sc_simcontext_int.h
	sc_thread_process.h 

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	sc_attribute.cpp \
	sc_cor_fiber.cpp \
	sc_cor_pthread.cpp \
	sc_cor_qt.cpp \
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
	sc_process.cpp \
	sc_reset.cpp \
	sc_sensitive.cpp \
	sc_simcontext.cpp \
	sc_thread_process.cpp \
	sc_time.cpp \
	sc_ver.cpp \
	sc_wait.cpp \
	sc_wait_cthread.cpp

NO_CXX_FILES =

libkernel_la_SOURCES = $(H_FILES) $(CXX_FILES)

INCLUDE_DIR = $(prefix)/include/sysc/kernel

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
