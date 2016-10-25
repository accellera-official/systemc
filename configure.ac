dnl ***************************************************************************
dnl
dnl  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
dnl  more contributor license agreements.  See the NOTICE file distributed
dnl  with this work for additional information regarding copyright ownership.
dnl  Accellera licenses this file to you under the Apache License, Version 2.0
dnl  (the "License"); you may not use this file except in compliance with the
dnl  License.  You may obtain a copy of the License at
dnl
dnl   http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl  Unless required by applicable law or agreed to in writing, software
dnl  distributed under the License is distributed on an "AS IS" BASIS,
dnl  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
dnl  implied.  See the License for the specific language governing
dnl  permissions and limitations under the License.
dnl
dnl ***************************************************************************
dnl
dnl  configure.ac --
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
dnl      Name, Affiliation, Date: Philipp A. Hartmann, OFFIS, 2013-08-20
dnl  Description of Modification: Installation/configuration reimplemented
dnl
dnl ***************************************************************************

dnl
dnl Initialization.
dnl
AC_INIT([SystemC],
        [2.3.1],
        [http://forums.accellera.org/forum/9-systemc/],
        [systemc],
        [http://accellera.org/],
)
dnl version of the included TLM library
TLM_PACKAGE_VERSION=2.0.3
AC_SUBST(TLM_PACKAGE_VERSION)

AC_CONFIG_SRCDIR([NEWS])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])

dnl
dnl Some simple checks.
dnl
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([1.10 tar-pax -Werror -Wno-portability no-define subdir-objects foreign])
dnl 1.10 needed for .s file support of LIBTOOLFLAGS

dnl enable silent rules, if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl
dnl reset default compiler flags
dnl
: ${CFLAGS=""}
: ${CXXFLAGS=""}

AC_PROG_CXX
: ${CC="${CXX}"}
AC_PROG_CC
AM_PROG_AS
AC_PROG_INSTALL
AC_PROG_LN_S

dnl
dnl Set the compiler flags and target architecture.
dnl

CXX_COMP=`set $CXX; basename $1`

EXTRA_CXXFLAGS="-Wall"
DEBUG_CXXFLAGS="-g"
OPT_CXXFLAGS="-O3"
EXPLICIT_LPTHREAD="-lpthread"
LDFLAG_RPATH="-Wl,-rpath="

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
        if test x$GXX = xyes ; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -arch ${CPU_ARCH}"
                EXTRA_CFLAGS="-xc"
                EXTRA_ASFLAGS="-arch ${CPU_ARCH} -x assembler"
                LDFLAG_RPATH="-Wl,-rpath -Wl,"
        else
                AC_MSG_ERROR("sorry...compiler not supported")
        fi
        ;;
    sparc-sun-solaris*) 
        QT_ARCH="sparc-os2"
        if test x$GXX = xyes ; then
                TARGET_ARCH="gccsparcOS5"
                EXTRA_CFLAGS="-xc"
                LDFLAG_RPATH="-Wl,-R"
        else
            if test x$CXX_COMP = xCC ; then
                EXTRA_CXXFLAGS=""
                TARGET_ARCH="sparcOS5"
                LDFLAG_RPATH="-Wl,-R"
            else
                AC_MSG_ERROR("sorry...compiler not supported")
            fi
        fi
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
        if test x$GXX = xyes ; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_CFLAGS="-xc"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry...compiler not supported")
        fi
        ;;
    *freebsd* | dragonfly* | *netbsd* | openbsd*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="bsd64"
                CPU_ARCH=64
                QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="bsd"
                CPU_ARCH=32
                QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        if test x$GXX = xyes ; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_CFLAGS="-xc"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry...compiler not supported")
        fi
        ;;
    *cygwin*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="cygwin64"
                CPU_ARCH=64
                QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="cygwin"
                CPU_ARCH=32
                QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        if test x$GXX = xyes ; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_CFLAGS="-xc"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry...compiler not supported")
        fi
        ;;
    *mingw*)
        QT_ARCH="none" # use WinFiber on MinGW platforms
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="mingw64"
                CPU_ARCH=64
                # QT_ARCH="x86_64"
                ;;
            i*86)
                TARGET_ARCH="mingw"
                CPU_ARCH=32
                # QT_ARCH="iX86"
                ;;
            *)
                AC_MSG_ERROR("sorry...architecture not supported")
            ;;
        esac
        if test x$GXX = xyes ; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
                EXTRA_CFLAGS="-xc"
                EXTRA_ASFLAGS="-m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry...compiler not supported")
        fi
        ;;
    *hpux11*)
        QT_ARCH="hppa"
        if test x$GXX = xyes ; then
                TARGET_ARCH="gcchpux11"
        else
            if test "$CXX_COMP" = "aCC" ; then
                EXTRA_CXXFLAGS="-AA -ext +DA2.0 +DS2.0"
                OPT_CXXFLAGS="+O1"
                TARGET_ARCH="hpux11"
                EXTRA_CFLAGS="-Wa,-w36"
                CCAS=/usr/ccs/bin/as
            else
                AC_MSG_ERROR("sorry...compiler not supported")
            fi
        fi
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
dnl Libtool setup
dnl
dnl - libtools should be put *after* all CC options have been set
dnl  - explicitly use C++ for libtool initialization
AC_LANG([C++])
dnl  - need to pass EXTRA_CXXFLAGS to Libtool via CXXFLAGS to correctly
dnl    detect C++ library dependencies, esp. for cross-compilation
CXXFLAGS_USER="${CXXFLAGS}"
CXXFLAGS="${EXTRA_CXXFLAGS} ${CXXFLAGS}"
dnl
AC_PROG_LIBTOOL
dnl  - restore user-given CXXFLAGS
CXXFLAGS="${CXXFLAGS_USER}"


dnl
dnl Setup installation directories
dnl
AC_MSG_CHECKING([for SystemC install layout])

AC_ARG_WITH([unix-layout],
  [AS_HELP_STRING([--with-unix-layout],
                  [use Unix directory layout for installation
                   @<:@default=no@:>@])],
  [case "x$withval" in
     xyes)  sysc_install_layout=Unix ;;
     xno|x) sysc_install_layout=classic ; ;;
     *)     AC_MSG_ERROR([Unknown install layout requested.]) ;;
   esac],
  [sysc_install_layout=classic]
) # AC_ARG_WITH gnu-layout

abs_srcdir=`cd "$srcdir" && pwd`
abs_builddir=`pwd`
if test "x$prefix" = "xNONE" ; then
  dnl Set the default prefix and make sure the prefix is absolute.
  prefix="$abs_srcdir"
  dnl Ignore install layout, when installing to $srcdir
  sysc_install_layout=ignored
fi

AC_MSG_RESULT($sysc_install_layout)

dnl
dnl Setup "classic" installation layout, if not overridden
dnl
rootdocdir='${docdir}'
if test $sysc_install_layout = Unix ; then

  dnl Automake/GNU/Unix defaults are kept,
  dnl but default library architecture suffix cleared
  LIB_ARCH_SUFFIX=''

else dnl classic/ignored (if not explicitly given)

  sysc_install_layout=classic
  if test x$datarootdir = x'${prefix}/share' ; then
    datarootdir='${prefix}'
  fi
  if test x$datadir = x'${datarootdir}' ; then
    datadir='${prefix}'
  fi
  if test x$docdir = x'${datarootdir}/doc/${PACKAGE_TARNAME}' ; then
    dnl main documentation
    docdir='${prefix}/docs'
    dnl "core" documentation (from root directory)
    rootdocdir='${prefix}'
  fi

  dnl default library arch suffix is target arch
  LIB_ARCH_SUFFIX='-${TARGET_ARCH}'

fi

AC_SUBST([rootdocdir])
AC_SUBST([examplesdir], ['${rootdocdir}/examples'] )
AC_SUBST([libarchdir],  ['${libdir}${LIB_ARCH_SUFFIX}'] )

AM_CONDITIONAL([SEPARATE_INSTALL_TREE],
  [ test "x${prefix}" != "x${abs_srcdir}" ])
dnl Check for out-of-tree build
AM_CONDITIONAL([SEPARATE_BUILD_TREE],
  [ test "x${abs_srcdir}" != "x${abs_builddir}" ])

#AM_COND_IF([SEPARATE_BUILD_TREE],[
if test -z "${SEPARATE_BUILD_TREE_TRUE}" ; then
  if test "x${prefix}" = "x${abs_builddir}" ; then
    AC_MSG_ERROR([Installation to build directory not supported.])
  fi
fi
#])

dnl
dnl handle library arch suffix
dnl
AC_MSG_CHECKING([for SystemC library arch suffix])
AC_ARG_WITH([arch-suffix],
  [AS_HELP_STRING([--with-arch-suffix],
                  [add suffix to library installation directory
                   @<:@default=-<target-arch>@:>@])],
  [case "x$withval" in
     xyes)  with_arch_suffix=yes
            LIB_ARCH_SUFFIX='-${TARGET_ARCH}'
            ;;
     xno|x) with_arch_suffix=no
            LIB_ARCH_SUFFIX=""
            ;;
     *)     LIB_ARCH_SUFFIX="${withval}" ;;
   esac],
  [with_arch_suffix=default]
) # AC_ARG_WITH arch-suffix
AC_MSG_RESULT($with_arch_suffix (TARGET_ARCH=${TARGET_ARCH}))

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
support_async_updates=no
if test x$ax_pthread_ok = xyes ; then
support_async_updates=pthread
elif test x$ax_pthread_ok = xno ; then
case "${TARGET_ARCH}" in
 dnl no pthreads needed on MSVC/MinGW
 mingw*|msvc*) support_async_updates=native ;;
 *)            ;;
esac
fi
AC_ARG_ENABLE([async-updates],
  AS_HELP_STRING([--disable-async-updates],
                 [disable request_async_update support]),
  [case "${enableval}" in
    yes) if test x"$support_async_updates" = xno ; then
            AC_MSG_WARN([no native/pthread support found, disabled])
            enable_async_updates=no
            support_async_updates=disabled
         fi
         ;;
    no)  support_async_updates=disabled ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-async-updates]) ;;
  esac],
  [if test x${support_async_updates} != xno; then
     enable_async_updates=yes
   else
     enable_async_updates=no
   fi]
  )
AM_CONDITIONAL([DISABLE_ASYNC_UPDATES],
               [test x"$enable_async_updates" = xno])
AC_MSG_RESULT($enable_async_updates)

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

AM_CONDITIONAL([WANT_PTHREADS_THREADS],
     [test x"$enable_pthreads" = xyes])
AM_CONDITIONAL([WANT_QT_THREADS],
     [test x"$enable_pthreads" = xno -a x"$QT_ARCH" != xnone ])

AM_CONDITIONAL([USES_PTHREADS_LIB],
  [test x"$enable_pthreads" = xyes -o x"$support_async_updates" = xpthread ])
AC_MSG_RESULT($enable_pthreads)

dnl
dnl enable simulation phase callbacks (experimental)
dnl
AC_MSG_CHECKING([whether to enable simulation phase callbacks (experimental)])
AC_ARG_ENABLE([phase-callbacks],
  [AS_HELP_STRING([--enable-phase-callbacks],
                  [enable simulation phase callbacks (experimental)
                   @<:@yes|tracing|no(=default)@:>@])],
  [case x"$enableval" in
    xyes|x)        enable_phase_callbacks=yes ;;
    xtracing)      enable_phase_callbacks=tracing ;;
    xno|xdefault)  enable_phase_callbacks=no  ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-phase-callbacks]) ;;
  esac],[enable_phase_callbacks=no])
AM_CONDITIONAL([ENABLE_CALLBACKS],
               [test x"$enable_phase_callbacks" = xyes])
AM_CONDITIONAL([ENABLE_CALLBACKS_TRACING],
               [test x"$enable_phase_callbacks" = xtracing])
AC_MSG_RESULT($enable_phase_callbacks)

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
dnl pkg-config setup
dnl
LIBCONFIG_DEFINES=
PKGCONFIG_DEFINES=
PKGCONFIG_CFLAGS=
PKGCONFIG_LDPRIV=

dnl add pthread (private) dependency
#AM_COND_IF([USES_PTHREADS_LIB],[
if test -z "${USES_PTHREADS_LIB_TRUE}" ; then
  PKGCONFIG_CFLAGS="${PTHREAD_CFLAGS}"
  PKGCONFIG_LDPRIV="${PTHREAD_LIBS} ${EXPLICIT_LPTHREAD}"
fi
#])

dnl
dnl check for additional (header+lib) compiler flags
dnl
# AC_CHECK_DEFINE([DEBUG_SYSTEMC],
#   [PKGCONFIG_DEFINES="${PKGCONFIG_DEFINES} -DDEBUG_SYSTEMC"])
AC_CHECK_DEFINE([SC_DISABLE_VIRTUAL_BIND],
  [PKGCONFIG_DEFINES="${PKGCONFIG_DEFINES} -DSC_DISABLE_VIRTUAL_BIND"
   sysc_non_1666=yes])

dnl
dnl check for additional library compiler flags (documentation only)
dnl
AC_CHECK_DEFINE([SC_ENABLE_IMMEDIATE_SELF_NOTIFICATION],
  [LIBCONFIG_DEFINES="${LIBCONFIG_DEFINES} -DSC_ENABLE_IMMEDIATE_SELF_NOTIFICATION"
   sysc_non_1666=yes])
AC_CHECK_DEFINE([SC_ENABLE_EARLY_MAXTIME_CREATION],
  [LIBCONFIG_DEFINES="${LIBCONFIG_DEFINES} -DSC_ENABLE_EARLY_MAXTIME_CREATION"
   sysc_non_1666=yes])

dnl
dnl Substitution variables.
dnl
AC_SUBST(LIBCONFIG_DEFINES)
AC_SUBST(PKGCONFIG_DEFINES)
AC_SUBST(PKGCONFIG_CFLAGS)
AC_SUBST(PKGCONFIG_LDPRIV)
AC_SUBST(EXTRA_CXXFLAGS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(EXTRA_ASFLAGS)
AC_SUBST(EXPLICIT_LPTHREAD)
AC_SUBST(LDFLAG_RPATH)
AC_SUBST(DEBUG_CXXFLAGS)
AC_SUBST(OPT_CXXFLAGS)
AC_SUBST(TARGET_ARCH)
AC_SUBST(LIB_ARCH_SUFFIX)
AC_SUBST(QT_ARCH)

dnl
dnl Create the Makefiles.
dnl
AC_OUTPUT(
 Makefile 
 src/Makefile 
 src/sysc/Makefile 
 src/sysc/systemc.pc
 src/sysc/communication/Makefile 
 src/sysc/datatypes/Makefile 
 src/sysc/kernel/Makefile 
 src/sysc/tracing/Makefile 
 src/sysc/qt/Makefile 
 src/sysc/utils/Makefile 
 src/sysc/packages/boost/Makefile 
 src/tlm_core/Makefile
 src/tlm_core/tlm.pc
 src/tlm_utils/Makefile
 examples/Makefile 
 examples/sysc/Makefile 
 examples/tlm/Makefile
 examples/tlm/common/Makefile
 docs/Makefile
 docs/sysc/doxygen/Doxyfile
 docs/tlm/doxygen/Doxyfile
)

dnl
dnl print configuration summary
dnl

dnl invert "async_updates" setting for output
AS_VAR_IF([enable_async_updates],[yes],
          [disable_async_updates=no],
          [disable_async_updates=yes
           sysc_non_1666=yes])

AS_IF([test -z "${WANT_QT_THREADS_TRUE}"],[sysc_coroutine="QuickThreads"],
      [test -z "${WANT_PTHREADS_THREADS_TRUE}"],[sysc_coroutine="Pthreads"],
      [sysc_coroutine="WinFiber"])

SYSTEMC_HOME=$prefix
prefix='<SYSTEMC_HOME>' dnl replace for printing
dnl ... resolve aliases
eval "sysc_includedir=\"${includedir}\""
eval "sysc_includedir=\"${sysc_includedir}\""
eval "sysc_libdir=\"${libdir}${LIB_ARCH_SUFFIX}\""
eval "sysc_libdir=\"${sysc_libdir}\""
eval "sysc_docdir=\"${docdir}\""
eval "sysc_docdir=\"${sysc_docdir}\""
eval "sysc_examplesdir=\"${examplesdir}\""
eval "sysc_examplesdir=\"${sysc_examplesdir}\""
eval "sysc_examplesdir=\"${sysc_examplesdir}\""
eval "sysc_examplesdir=\"${sysc_examplesdir}\""
eval "sysc_prefix=\"${SYSTEMC_HOME}\""
dnl ... restore prefix
prefix=$SYSTEMC_HOME

cat <<EOF | tee -a config.log
---------------------------------------------------------------------
Configuration summary of $PACKAGE_STRING for $target
---------------------------------------------------------------------

 Directory setup (based on $sysc_install_layout layout):
   Installation prefix (aka SYSTEMC_HOME):
      ${sysc_prefix}
   Header files  : ${sysc_includedir}
   Libraries     : ${sysc_libdir}
   Documentation : ${sysc_docdir}
   Examples      : ${sysc_examplesdir}

 Architecture    : ${TARGET_ARCH}
 Compiler (flags): ${CXX}
     ${CXXFLAGS}

 Build settings:
   Enable compiler optimizations  : $enable_optimize
   Include debugging symbols      : $enable_debug
   Coroutine package for processes: $sysc_coroutine
   Disable async_request_update   : $disable_async_updates
   Phase callbacks (experimental) : $enable_phase_callbacks
   Additional settings            :
     ${PKGCONFIG_DEFINES} ${LIBCONFIG_DEFINES}
---------------------------------------------------------------------
EOF
if test x${sysc_non_1666+set} = xset ; then
cat <<EOF | tee -a config.log
WARNING: The selected SystemC library configuration is non-conforming
         to IEEE Std. 1666-2011. See INSTALL.
---------------------------------------------------------------------
EOF
fi

dnl Taf!
