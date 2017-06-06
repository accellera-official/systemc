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
##  src/sysc/qt/Makefile.am --
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

NO_H_FILES = \
	copyright.h \
	qt.h \
	qtmd.h \
	\
	md/aarch64.h \
	md/axp.h \
	md/hppa.h \
	md/i386.h \
	md/iX86_64.h \
	md/ksr1.h \
	md/m88k.h \
	md/mips.h \
	md/powerpc_mach.h \
	md/powerpc_sys5.h \
	md/sparc.h \
	md/vax.h

noinst_HEADERS = \
	$(NO_H_FILES)

C_FILES = \
	qt.c

NO_C_FILES = \
	md/axp.c \
	md/m88k.c \
	md/null.c \
	md/powerpc.c

NO_S_FILES = \
	md/aarch64.s \
	md/axp_b.s \
	md/axp.s \
	md/hppa_b.s \
	md/hppa.s \
	md/i386_b.s \
	md/i386.s \
	md/iX86_64.s \
	md/ksr1_b.s \
	md/ksr1.s \
	md/m88k_b.s \
	md/m88k.s \
	md/mips_b.s \
	md/mips-irix5.s \
	md/mips.s \
	md/null.s \
	md/powerpc_mach_b.s \
	md/powerpc_mach.s \
	md/powerpc_sys5_b.s \
	md/powerpc_sys5.s \
	md/sparc_b.s \
	md/sparc.s \
	md/vax_b.s \
	md/vax.s

EXTRA_DIST += \
	CHANGES \
	config \
	INSTALL \
	Makefile.base \
	README \
	README.MISC \
	README.PORT \
	\
	b.h \
	meas.c \
	stp.c \
	stp.h \
	\
	time/assim \
	time/cswap \
	time/go \
	time/init \
	time/prim \
	time/raw \
	time/README.time \
	\
	md/axp.1.Makefile \
	md/axp.2.Makefile \
	md/axp.Makefile \
	md/axp.README \
	md/default.Makefile \
	md/hppa-cnx.Makefile \
	md/hppa.Makefile \
	md/i386.README \
	md/ksr1.Makefile \
	md/m88k.Makefile \
	md/null.README \
	md/powerpc.README \
	md/pthreads.Makefile \
	md/solaris.README \
	\
	$(NO_C_FILES) \
	$(NO_H_FILES) \
	$(NO_S_FILES)

CLEANFILES += \
	$(QT_ARCH_FILES) \
	configuration

if WANT_QT_THREADS

BUILT_SOURCES = \
	configuration

noinst_LTLIBRARIES = libqt.la

nodist_libqt_la_SOURCES = \
	$(QT_ARCH_FILES)

libqt_la_SOURCES = \
	$(C_FILES)

libqt_la_LIBTOOLFLAGS = \
	--tag=CC

qtmds_o_CCASFLAGS = \
	$(AM_CCASFLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES)

# only add qtmdc.c to QT_ARCH_FILES, if needed for the platform (!=null.c)
QT_ARCH_FILES = \
	qtmds.s

if QT_ARCH_AARCH64
QT_ARCH_IMPL = md/aarch64.s
endif
if QT_ARCH_X86
QT_ARCH_IMPL = md/i386.s
endif
if QT_ARCH_X86_64
QT_ARCH_IMPL = md/iX86_64.s
endif
if QT_ARCH_POWERPC_MACOSX
QT_ARCH_FILES+=qtmdc.c
QT_ARCH_IMPL = md/powerpc_mach.s md/powerpc.c
endif
if QT_ARCH_SPARC
QT_ARCH_IMPL = md/sparc.s
endif
if QT_ARCH_HPPA
QT_ARCH_IMPL = md/hppa.s
endif

# ensure to use a working link command in 'config'
export LN_S

configuration:
if SEPARATE_BUILD_TREE # copy arch files to build tree
	$(AM_V_at)for file in $(QT_ARCH_IMPL); do  \
	  $(MKDIR_P) `dirname "$$file"` ;             \
	  $(INSTALL_DATA) $(srcdir)/$$file ./$$file ; \
	done
endif # SEPARATE_BUILD_TREE
	$(AM_V_GEN)$(SHELL) $(srcdir)/config $(QT_ARCH)

if SEPARATE_BUILD_TREE
CLEANFILES += \
	 $(QT_ARCH_IMPL)
endif # SEPARATE_BUILD_TREE

endif #  WANT_QT_THREADS

## Taf!
