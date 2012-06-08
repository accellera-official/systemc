dnl ***************************************************************************
dnl
dnl  The following code is derived, directly or indirectly, from the SystemC
dnl  source code Copyright (c) 1996-2002 by all Contributors.
dnl  All Rights reserved.
dnl
dnl  The contents of this file are subject to the restrictions and limitations
dnl  set forth in the SystemC Open Source License Version 2.3 (the "License");
dnl  You may not use this file except in compliance with such restrictions and
dnl  limitations. You may obtain instructions on how to receive a copy of the
dnl  License at http://www.systemc.org/. Software distributed by Contributors
dnl  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
dnl  ANY KIND, either express or implied. See the License for the specific
dnl  language governing rights and limitations under the License.
dnl
dnl ***************************************************************************
dnl
dnl  configure.in --
dnl  Process this file with autoconf to produce a configure script.
dnl
dnl  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21
dnl
dnl ***************************************************************************
dnl
dnl  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
dnl  changes you are making here.
dnl
dnl      Name, Affiliation, Date: Alan Fitch, Doulos, 2012-03-10
dnl  Description of Modification: TLM2 files added
dnl
dnl ***************************************************************************

dnl
dnl Initialization.
dnl
AC_INIT([SystemC],[2.3.0])
AC_CONFIG_SRCDIR([NEWS])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])

dnl
dnl Some simple checks.
dnl
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([1.10 no-define]) # for .s file support of LIBTOOLFLAGS

dnl
dnl reset default compiler flags
dnl
: ${CFLAGS=""}
: ${CXXFLAGS=""}

AM_PROG_AS
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

dnl
dnl Set the compiler flags and target architecture.
dnl

CXX_COMP=`set $CXX; basename $1`

EXTRA_CXXFLAGS="-Wall"
DEBUG_CXXFLAGS="-g"
OPT_CXXFLAGS="-O3"

case "$target" in
    *-apple-*)
        CPU_ARCH="${target_cpu}"
        case "$target_cpu" in
            x86_64)
                QT_ARCH="x86_64"
                TARGET_ARCH="macosx64"
                ;;
            x386 | i386)
                TARGET_ARCH="macosx"
                CPU_ARCH="i386"
                QT_ARCH="iX86"
                ;;
            powerpc)
                TARGET_ARCH="macosxppc"
                CPU_ARCH="ppc"
                QT_ARCH="powerpc-apple-macosx"
                ;;
            powerpc64)
                TARGET_ARCH="macosxppc64"
                CPU_ARCH="ppc64"
                QT_ARCH="pthreads"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
                ;;
        esac
        case "$CXX_COMP" in
            CC | c++ | g++)
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -arch ${CPU_ARCH}"
                EXTRA_ASFLAGS="-arch ${CPU_ARCH} -x assembler"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
                ;;
        esac
        ;;
    sparc-sun-solaris*) 
        case "$CXX_COMP" in
            CC)
                EXTRA_CXXFLAGS=""
                TARGET_ARCH="sparcOS5"
		CCAS=as
                ;;
            c++ | g++)
                TARGET_ARCH="gccsparcOS5"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
		;;
        esac
        QT_ARCH="sparc-os2"
        ;;
    *linux*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="linux64"
                CPU_ARCH=64
                QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="linux"
                CPU_ARCH=32
                QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        case "$CXX_COMP" in
            c++ | g++)
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
            ;;
        esac
        ;;
    *freebsd*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="freebsd64"
                CPU_ARCH=64
                QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="freebsd"
                CPU_ARCH=32
                QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        case "$CXX_COMP" in
            c++ | g++)
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
            ;;
        esac
        ;;
    *cygwin*)
        case "$CXX_COMP" in
            c++ | g++)
                TARGET_ARCH="cygwin"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
            ;;
        esac
        QT_ARCH="iX86"
        ;;
    *mingw*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="mingw64"
                CPU_ARCH=64
                QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="mingw"
                CPU_ARCH=32
                QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        case "$CXX_COMP" in
            c++ | g++ | *mingw*-g++)
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
            ;;
        esac
        ;;
    *hpux11*)
        case "$CXX_COMP" in
            aCC)
                EXTRA_CXXFLAGS="-AA -ext +DA2.0 +DS2.0"
                OPT_CXXFLAGS="+O1"
                TARGET_ARCH="hpux11"
        	EXTRA_CFLAGS="-Wa,-w36"
		CCAS=/usr/ccs/bin/as
                ;;
            c++ | g++)
                TARGET_ARCH="gcchpux11"
                ;;
            *)
                AC_MSG_ERROR("sorry...compiler not supported")
		;;
        esac
        QT_ARCH="hppa"
        ;;
    *)
        AC_MSG_ERROR("sorry...architecture not supported")
        ;;
esac

dnl
dnl check for pthreads compiler flags.
dnl
AX_PTHREAD

dnl
dnl set C-related variables
dnl
CC="$CXX"

dnl libtools should be put *after* all CC options have been set
AC_PROG_LIBTOOL

dnl
dnl Set the default prefix and make sure the prefix is absolute.
dnl

test "x$prefix" = xNONE && prefix=$srcdir
current_dir=`pwd`; cd $prefix; prefix=`pwd`; cd $current_dir

dnl
dnl add debugging symbols to library
dnl
AC_MSG_CHECKING([whether to add debug symbols to the SystemC library])
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],[include debugging symbols]),
  [case "${enableval}" in
    yes|no) : ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
  esac],[enable_debug=no])
AM_CONDITIONAL([WANT_DEBUG], [test x$enable_debug = xyes])
AC_MSG_RESULT($enable_debug)

dnl
dnl switch off compiler optimization
dnl
AC_MSG_CHECKING([whether to enable compiler optimization])
AC_ARG_ENABLE([optimize],
  AS_HELP_STRING([--disable-optimize],[disable compiler optimization]),
  [case "${enableval}" in
    yes|no) : ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-optimize]) ;;
  esac],[enable_optimize=yes])
AM_CONDITIONAL([WANT_OPTIMIZE], [test x$enable_optimize = xyes])
AC_MSG_RESULT($enable_optimize)

dnl
dnl async_request_update support
dnl
AC_MSG_CHECKING([whether to add support for asynchronous update requests])
support_async_updates=yes
if test x$ax_pthread_ok = xno ; then
case "${TARGET_ARCH}" in
 cygwin|mingw*) support_async_updates=yes ;; dnl no pthreads needed on Windows
 *)             support_async_updates=no  ;;
esac
fi
AC_ARG_ENABLE([async-updates],
  AS_HELP_STRING([--disable-async-updates],
                 [disable request_async_update support]),
  [case "${enableval}" in
    yes) if test x"$support_async_updates" = xno ; then
            AC_MSG_WARN([no pthread support found])
         fi
         support_async_updates=yes ;;
    no)  support_async_updates=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-async-updates]) ;;
  esac],[enable_async_updates=${support_async_updates}])
AM_CONDITIONAL([DISABLE_ASYNC_UPDATES],
               [test x"$support_async_updates" = xno])
AC_MSG_RESULT($support_async_updates)

dnl
dnl use pthreads for SystemC processes
dnl
AC_MSG_CHECKING([whether to use POSIX threads for SystemC processes])
AC_ARG_ENABLE([pthreads],
  AS_HELP_STRING([--enable-pthreads],
                 [use POSIX threads for SystemC processes]),
  [case "${enableval}" in
    yes) if test x"$ax_pthread_ok" = xyes ; then
           QT_ARCH=pthreads
         else
           enable_pthreads=no
           AC_MSG_WARN([no pthread support found, ignore --enable-pthreads])
         fi ;;
    no)  : ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-pthreads]) ;;
  esac],[enable_pthreads=no])

if test x"$QT_ARCH" = xpthreads -a x"$enable_pthreads" = xno ; then
  if test x"$ax_pthread_ok" = xyes ; then
    enable_pthreads=yes
  else
    AC_MSG_ERROR([pthread processes required on target architecture ${target}])
  fi
fi

AM_CONDITIONAL([WANT_PTHREADS_THREADS], [test x"$enable_pthreads" = xyes])

AM_CONDITIONAL([USES_PTHREADS_LIB],
  [test x"$enable_pthreads" = xyes -o x"$support_async_updates" = xyes ])
AC_MSG_RESULT($enable_pthreads)

dnl
dnl Set conditionals for various quick thread architectures:
dnl
AM_CONDITIONAL([QT_ARCH_X86_64], [ test "${QT_ARCH}" = "x86_64" ])
AM_CONDITIONAL([QT_ARCH_POWERPC_MACOSX], [ test "${QT_ARCH}" = "powerpc-apple-macosx" ])
AM_CONDITIONAL([QT_ARCH_SPARC], [ test "${QT_ARCH}" = "sparc-os2" ])
AM_CONDITIONAL([QT_ARCH_X86], [ test "${QT_ARCH}" = "iX86" ])
AM_CONDITIONAL([QT_ARCH_HPPA], [ test "${QT_ARCH}" = "hppa" ])
AM_CONDITIONAL([QT_ARCH_PTHREADS], [ test "${QT_ARCH}" = "pthreads" ])

dnl
dnl Substitution variables.
dnl
AC_SUBST(EXTRA_CXXFLAGS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(EXTRA_ASFLAGS)
AC_SUBST(DEBUG_CXXFLAGS)
AC_SUBST(OPT_CXXFLAGS)
AC_SUBST(TARGET_ARCH)
AC_SUBST(QT_ARCH)

dnl
dnl Create the Makefiles.
dnl
AC_OUTPUT(
 Makefile 
 src/Makefile 
 src/sysc/Makefile 
 src/sysc/communication/Makefile 
 src/sysc/datatypes/Makefile 
 src/sysc/datatypes/bit/Makefile 
 src/sysc/datatypes/fx/Makefile 
 src/sysc/datatypes/int/Makefile 
 src/sysc/datatypes/misc/Makefile 
 src/sysc/kernel/Makefile 
 src/sysc/tracing/Makefile 
 src/sysc/qt/Makefile 
 src/sysc/utils/Makefile 
 src/sysc/packages/Makefile 
 src/sysc/packages/boost/Makefile 
 src/sysc/packages/boost/bind/Makefile 
 src/sysc/packages/boost/config/Makefile 
 src/sysc/packages/boost/config/compiler/Makefile 
 src/sysc/packages/boost/config/platform/Makefile 
 src/sysc/packages/boost/config/stdlib/Makefile 
 src/sysc/packages/boost/mpl/Makefile 
 src/sysc/packages/boost/mpl/aux_/Makefile 
 src/sysc/packages/boost/mpl/aux_/config/Makefile 
 src/sysc/packages/boost/detail/Makefile 
 src/sysc/packages/boost/utility/Makefile 
 src/tlm_core/tlm_1/tlm_analysis/Makefile
 src/tlm_core/tlm_1/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_channels/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_req_rsp_channels/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_ports/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_adapters/Makefile
 src/tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/Makefile
 src/tlm_core/tlm_2/tlm_2_interfaces/Makefile
 src/tlm_core/tlm_2/Makefile
 src/tlm_core/tlm_2/tlm_generic_payload/Makefile
 src/tlm_core/tlm_2/tlm_sockets/Makefile
 src/tlm_core/tlm_2/tlm_quantum/Makefile
 src/tlm_utils/Makefile
 examples/Makefile 
 examples/sysc/Makefile 
 examples/sysc/fir/Makefile 
 examples/sysc/fft/Makefile 
 examples/sysc/fft/fft_flpt/Makefile 
 examples/sysc/fft/fft_fxpt/Makefile 
 examples/sysc/pipe/Makefile 
 examples/sysc/pkt_switch/Makefile 
 examples/sysc/rsa/Makefile 
 examples/sysc/simple_bus/Makefile 
 examples/sysc/simple_fifo/Makefile 
 examples/sysc/simple_perf/Makefile 
 examples/sysc/2.1/Makefile 
 examples/sysc/2.1/dpipe/Makefile 
 examples/sysc/2.1/forkjoin/Makefile 
 examples/sysc/2.1/reset_signal_is/Makefile 
 examples/sysc/2.1/sc_export/Makefile 
 examples/sysc/2.1/sc_report/Makefile 
 examples/sysc/2.1/scx_barrier/Makefile 
 examples/sysc/2.1/scx_mutex_w_policy/Makefile 
 examples/sysc/2.1/specialized_signals/Makefile 
 examples/sysc/2.3/Makefile
 examples/sysc/2.3/include/Makefile
 examples/sysc/2.3/sc_rvd/Makefile
 examples/sysc/2.3/sc_ttd/Makefile
 examples/tlm/Makefile
 docs/sysc/doxygen/Doxyfile
 docs/tlm/doxygen/Doxyfile
)


dnl Taf!
