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
        [3.0.0],
        [http://forums.accellera.org/forum/9-systemc/],
        [systemc],
        [http://accellera.org/],
)
dnl version of the included TLM library
TLM_PACKAGE_VERSION=2.0.6
AC_SUBST(TLM_PACKAGE_VERSION)

AC_CONFIG_SRCDIR([README.md])
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

dnl define AS_VAR_APPEND, if not present (on older Autoconf versions)
m4_ifndef([AS_VAR_APPEND],[as_fn_append () { eval $[1]=\$$[1]\$[2]; }
AC_DEFUN([AS_VAR_APPEND],[as_fn_append $1 $2])])dnl

dnl
dnl reset default compiler flags
dnl
: ${CFLAGS=""}
: ${CXXFLAGS=""}

dnl check for C++ compiler
AC_PROG_CXX

dnl extract C++ compiler name (used for matching CC and aCC below)
CXX_COMP=`set $CXX; basename $1`

dnl check for clang++
AC_CACHE_CHECK([whether we are using a Clang/LLVM C++ compiler],
  [sc_cv_clangxx],
  [AC_LANG_PUSH([C++])
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[[
#ifndef __clang__
not clang
#endif
]])],dnl
   [sc_cv_clangxx=yes],[sc_cv_clangxx=no])
   AC_LANG_POP([C++])])

dnl check for C compiler (default to C++ compiler)
: ${CC="${CXX}"}
AC_PROG_CC

dnl check for clang
AC_CACHE_CHECK([whether we are using a Clang/LLVM C compiler],
  [sc_cv_clang],
  [AC_LANG_PUSH([C])
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[[
#ifndef __clang__
not clang
#endif
]])],dnl
  [sc_cv_clang=yes],[sc_cv_clang=no])
  AC_LANG_POP([C])])

dnl AM_PROG_AR available since Automake 1.11.2, generic check otherwise
m4_ifdef([AM_PROG_AR],[AM_PROG_AR],[AC_CHECK_TOOL([AR],[ar],[ar])])

dnl Update default archiver flags to avoid warnings on some platforms,
dnl see https://www.mail-archive.com/bug-automake@gnu.org/msg04226.html
: ${ARFLAGS=cr}
: ${AR_FLAGS=${ARFLAGS}}dnl used by many libtool versions

AM_PROG_AS
AC_PROG_INSTALL
AC_PROG_LN_S

dnl
dnl Set the compiler flags and target architecture.
dnl

EXTRA_CXXFLAGS="-Wall"
EXTRA_CFLAGS="-xc"
EXTRA_ASFLAGS=""
EXTRA_LDFLAGS=""
DEBUG_CXXFLAGS="-g"
OPT_CXXFLAGS="-O3"
EXPLICIT_LPTHREAD="-lpthread"
LDFLAG_RPATH="-Wl,-rpath="

dnl Run assembler with pre-processor on GCC-compatible compilers
AS_VAR_IF([GXX],[yes],[EXTRA_ASFLAGS="-xassembler-with-cpp"])

dnl
dnl Prepare architecture alias (to be refined later)
dnl
AS_CASE([$target],
  [*-apple-*],        [TARGET_ARCH=macos],
  [*linux*],          [TARGET_ARCH=linux],
  [*bsd*|*dragonfly*],[TARGET_ARCH=bsd],
  [*mingw*],          [TARGET_ARCH=mingw],
  [*cygwin*],         [TARGET_ARCH=cygwin],
  [sparc-*-solaris*], [TARGET_ARCH=sparcOS5],
  [*hpux11*],         [TARGET_ARCH=hpux11],
  dnl default
  [AC_MSG_ERROR("sorry...architecture not supported")])

dnl Adjust for GCC on Solaris and HP-UX
AS_CASE([${TARGET_ARCH}],
  [hpux11|sparcOS5],
    [AS_VAR_IF([GXX],[yes],[TARGET_ARCH="gcc${TARGET_ARCH}"])])

dnl
dnl Refine architecture settings
dnl
AS_CASE([${TARGET_ARCH}],
  [linux|bsd|mingw|cygwin],dnl
    [# check CPU architecture
     AS_CASE([${target_cpu}], dnl
       [x86_64|amd64], dnl
         [TARGET_ARCH="${TARGET_ARCH}64"
          QT_ARCH="x86_64"
          CPU_ARCH="-m64"],
       [x*86|i*86], dnl
         [QT_ARCH="iX86"
          CPU_ARCH="-m32"],
       [aarch64], dnl
         [TARGET_ARCH="linuxaarch64"
          QT_ARCH="aarch64"
          CPU_ARCH=""],
       [AC_MSG_ERROR("sorry...architecture not supported")])

     # check compiler
     AS_VAR_IF([GXX],[yes],dnl
       [EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} ${CPU_ARCH}"
        EXTRA_ASFLAGS="${EXTRA_ASFLAGS} ${CPU_ARCH}"],
       [AC_MSG_ERROR("sorry...compiler not supported")])

     # use WinFiber on MinGW platforms
     AS_CASE([${TARGET_ARCH}],
       [mingw*],[QT_ARCH="none"])
    ],

  [macos],dnl
    [LDFLAG_RPATH="-Wl,-rpath -Wl,"
     # check CPU architecture
     AS_CASE([${target_cpu}], dnl
       [x86_64|amd64], dnl
         [TARGET_ARCH="${TARGET_ARCH}x64"
          CPU_ARCH="x86_64"
          QT_ARCH="x86_64"],
       [aarch64|arm64], dnl
         [TARGET_ARCH="${TARGET_ARCH}arm64"
          CPU_ARCH="arm64"
          QT_ARCH="aarch64"],
       [x*86|i*86], dnl
         [CPU_ARCH="i386"
          QT_ARCH="iX86"],
       [powerpc64], dnl
         [TARGET_ARCH="${TARGET_ARCH}ppc64"
          CPU_ARCH="ppc64"
          QT_ARCH="pthreads"],
       [powerpc], dnl
         [TARGET_ARCH="${TARGET_ARCH}ppc"
          CPU_ARCH="ppc"
          QT_ARCH="pthreads"],
       [AC_MSG_ERROR("sorry...architecture not supported")])

     # check compiler
     AS_VAR_IF([GXX],[yes],dnl
       [EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -arch ${CPU_ARCH}"
        EXTRA_ASFLAGS="${EXTRA_ASFLAGS} -arch ${CPU_ARCH}"],
       [AC_MSG_ERROR("sorry...compiler not supported")])
    ],

  [gcchpux11],dnl
    [QT_ARCH="hppa"],
  [hpux11],dnl
    [QT_ARCH="hppa"
     AS_VAR_IF([CXX_COMP],[aCC], dnl
       [EXTRA_CXXFLAGS="-AA -ext +DA2.0 +DS2.0"
        EXTRA_CFLAGS="-Wa,-w36"
        OPT_CXXFLAGS="+O1"
        : ${CCAS=/usr/ccs/bin/as}],
       [AC_MSG_ERROR("sorry...compiler not supported")])],

  [gccsparcOS5],dnl
    [QT_ARCH="sparc-os2"
     LDFLAG_RPATH="-Wl,-R"],
  [sparcOS5],dnl
    [QT_ARCH="sparc-os2"
     LDFLAG_RPATH="-Wl,-R"
     AS_VAR_IF([CXX_COMP],[CC], dnl
       [EXTRA_CXXFLAGS=""
        EXTRA_CFLAGS=""],
       [AC_MSG_ERROR("sorry...compiler not supported")])]
) dnl AS_CASE([$TARGET_ARCH])

dnl include EXTRA_CXXFLAGS in EXTRA_(C|AS)FLAGS by default
EXTRA_CFLAGS="${EXTRA_CXXFLAGS} ${EXTRA_CFLAGS}"

dnl suppress clang's warnings about unused command-line arguments
dnl (currently enforced for assembler and linker only)
AS_IF([test "$sc_cv_clangxx" = yes],dnl CXX is used for linking
  [EXTRA_LDFLAGS="${EXTRA_LDFLAGS} -Wc,-Qunused-arguments"])
AS_IF([test "$sc_cv_clang" = yes -a "$CCAS" = "$CC"], dnl CC used as assembler
  [EXTRA_ASFLAGS="${EXTRA_ASFLAGS} -Qunused-arguments"])


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
  [AS_CASE(["x$withval"],
     [xyes], [sysc_install_layout=Unix],
     [xno|x],[sysc_install_layout=classic],
     [AC_MSG_ERROR([Unknown install layout requested.])])],
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

AM_CONDITIONAL([SEPARATE_INSTALL_TREE],dnl
  [ test "x${prefix}" != "x${abs_srcdir}" ])
dnl Check for out-of-tree build
AM_CONDITIONAL([SEPARATE_BUILD_TREE],dnl
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
  [AS_CASE(["x$withval"],dnl
    [xyes],[with_arch_suffix=yes
            LIB_ARCH_SUFFIX='-${TARGET_ARCH}'],
    [xno|x],[with_arch_suffix=no
             LIB_ARCH_SUFFIX=""],
    [LIB_ARCH_SUFFIX="${withval}"])],
  [with_arch_suffix=default]
) # AC_ARG_WITH arch-suffix
AC_MSG_RESULT($with_arch_suffix (TARGET_ARCH=${TARGET_ARCH}))

dnl
dnl add debugging symbols to library
dnl
AC_MSG_CHECKING([whether to add debug symbols to the SystemC library])
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],[include debugging symbols]),
  [AS_CASE(["${enableval}"],dnl
    [yes|no],[],
    [AC_MSG_ERROR([bad value ${enableval} for --enable-debug])])],
  [enable_debug=no])
AM_CONDITIONAL([WANT_DEBUG], [test x$enable_debug = xyes])
AC_MSG_RESULT($enable_debug)

dnl
dnl switch off compiler optimization
dnl
AC_MSG_CHECKING([whether to enable compiler optimization])
AC_ARG_ENABLE([optimize],
  AS_HELP_STRING([--disable-optimize],[disable compiler optimization]),
  [AS_CASE(["${enableval}"],dnl
    [yes|no],[],
    [AC_MSG_ERROR([bad value ${enableval} for --enable-optimize])])],
  [enable_optimize=yes])
AM_CONDITIONAL([WANT_OPTIMIZE], [test x$enable_optimize = xyes])
AC_MSG_RESULT($enable_optimize)

dnl
dnl use pthreads for SystemC processes
dnl
AC_MSG_CHECKING([whether to use POSIX threads for SystemC processes])
AC_ARG_ENABLE([pthreads],
  AS_HELP_STRING([--enable-pthreads],
                 [use POSIX threads for SystemC processes]),
  [AS_CASE(["${enableval}"],dnl
    [yes],dnl
      [AS_IF([test x"$ax_pthread_ok" = xyes],[QT_ARCH=pthreads],
        [enable_pthreads=no
         AC_MSG_WARN([no pthread support found, ignore --enable-pthreads])]
      )],
    [no],[],
    [AC_MSG_ERROR([bad value ${enableval} for --enable-pthreads])])],
  [enable_pthreads=no])

AS_IF([test x"$enable_pthreads" = xyes],[dnl
  AS_CASE(["${TARGET_ARCH}"],dnl
    dnl pthread not supported in win32/win64
    [mingw*|msvc*],dnl
      [AC_MSG_ERROR([pthread processes not supported on target architecture ${target}])dnl
  ])dnl
])

AS_IF([test x"$QT_ARCH" = xpthreads -a x"$enable_pthreads" = xno],[dnl
  AS_IF([test x"$ax_pthread_ok" = xyes],dnl
    [enable_pthreads=yes],
    [AC_MSG_ERROR([pthread processes required on target architecture ${target}])])dnl
])

AM_CONDITIONAL([WANT_PTHREADS_THREADS],dnl
     [test x"$enable_pthreads" = xyes])
AM_CONDITIONAL([WANT_QT_THREADS],dnl
     [test x"$enable_pthreads" = xno -a x"$QT_ARCH" != xnone ])

AM_CONDITIONAL([USES_PTHREADS_LIB],dnl
  [test x"$enable_pthreads" = xyes])
AC_MSG_RESULT($enable_pthreads)

dnl
dnl enable VCD scopes
dnl
AC_MSG_CHECKING([whether to enable VCD scopes by default])
AC_ARG_ENABLE([vcd-scopes],
  [AS_HELP_STRING([--enable-vcd-scopes],
                  [enable vcd scopes
                   @<:@yes(=default)|no@:>@])],
  [AS_CASE(["${enableval}"],dnl
    [yes|default],       [enable_vcd_scopes=yes],
    [no],[enable_vcd_scopes=no],
    [AC_MSG_ERROR([bad value ${enableval} for --enable-vcd-scopes])])],
  [enable_vcd_scopes=yes])
AM_CONDITIONAL([DISABLE_VCD_SCOPES],dnl
               [test x"$enable_vcd_scopes" = xno])
AC_MSG_RESULT($enable_vcd_scopes)

dnl
dnl Set conditionals for various quick thread architectures:
dnl
AM_CONDITIONAL([QT_ARCH_X86], [ test "${QT_ARCH}" = "iX86" ])
AM_CONDITIONAL([QT_ARCH_AARCH64], [ test "${QT_ARCH}" = "aarch64" ])
AM_CONDITIONAL([QT_ARCH_X86_64], [ test "${QT_ARCH}" = "x86_64" ])
AM_CONDITIONAL([QT_ARCH_POWERPC_MACOSX], [ test "${QT_ARCH}" = "powerpc-apple-macosx" ])
AM_CONDITIONAL([QT_ARCH_SPARC], [ test "${QT_ARCH}" = "sparc-os2" ])
AM_CONDITIONAL([QT_ARCH_HPPA], [ test "${QT_ARCH}" = "hppa" ])

dnl
dnl pkg-config setup
dnl
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

dnl
dnl Substitution variables.
dnl
AC_SUBST(PKGCONFIG_DEFINES)
AC_SUBST(PKGCONFIG_CFLAGS)
AC_SUBST(PKGCONFIG_LDPRIV)
AC_SUBST(EXTRA_CXXFLAGS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(EXTRA_ASFLAGS)
AC_SUBST(EXTRA_LDFLAGS)
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
 src/systemc.pc
 src/tlm.pc
 src/sysc/Makefile 
 src/sysc/packages/qt/Makefile 
 src/tlm_core/Makefile
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

AS_IF([test -z "${WANT_QT_THREADS_TRUE}"],[sysc_coroutine="QuickThreads"],
      [test -z "${WANT_PTHREADS_THREADS_TRUE}"],[sysc_coroutine="Pthreads"],
      [sysc_coroutine="WinFiber"])

sysc_compiler="${CXX}"
AS_VAR_IF([CC],["${CXX}"],dnl
  [AS_VAR_APPEND([sysc_compiler],[" (C/C++)"])],dnl
  [AS_VAR_APPEND([sysc_compiler],[" (C++) / ${CC} (C)"])])
AS_VAR_IF([CCAS],["${CC}"],[],dnl
  [AS_VAR_APPEND([sysc_compiler],[" / ${CCAS} (Assembler)"])])

dnl prepare printing of command-line flags
sysc_flags=""
AS_IF([test -n "${CFLAGS}"],dnl
  [AS_VAR_APPEND([sysc_flags],dnl
    ["   C compiler flags (CFLAGS)      : ${CFLAGS}${as_nl}"])])
AS_IF([test -n "${CXXFLAGS}"],dnl
  [AS_VAR_APPEND([sysc_flags],dnl
    ["   C++ compiler flags (CXXFLAGS)  : ${CXXFLAGS}${as_nl}"])])
AS_IF([test -n "${CCASFLAGS}"],dnl
  [AS_VAR_APPEND([sysc_flags],dnl
    ["   Assembler flags (CCASFLAGS)    : ${CCASFLAGS}${as_nl}"])])
AS_IF([test -n "${LDFLAGS}"],dnl
  [AS_VAR_APPEND([sysc_flags],dnl
    ["   Linker flags (LDFLAGS)         : ${LDFLAGS}${as_nl}"])])
AS_IF([test -n "${sysc_flags}"],dnl
[sysc_flags="${as_nl} User-provided command-line flags :${as_nl}${sysc_flags}"])

dnl prepare printing of additional settings
AS_IF([test -n "${PKGCONFIG_DEFINES}"],[dnl
sysc_additional="${as_nl} Detected additional settings:"
AS_IF([test -n "${PKGCONFIG_DEFINES}"],dnl
  [AS_VAR_APPEND([sysc_additional],dnl
                 ["${as_nl}    ${PKGCONFIG_DEFINES}"])])
AS_VAR_APPEND([sysc_additional],["${as_nl}"])dnl
]) dnl additional settings

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
 Compiler        : ${sysc_compiler}
 ${sysc_flags}
 Build settings:
   Enable compiler optimizations  : $enable_optimize
   Include debugging symbols      : $enable_debug
   Coroutine package for processes: $sysc_coroutine
   Enable VCD scopes by default   : $enable_vcd_scopes
   Enable legacy mem. mangement   : $enable_legacy_mem_mgmt
 ${sysc_additional}
---------------------------------------------------------------------
EOF
AS_IF([${sysc_non_1666+:} false],[dnl
cat <<EOF | tee -a config.log
WARNING: The selected SystemC library configuration is non-conforming
         to IEEE Std. 1666-2022. See INSTALL.
---------------------------------------------------------------------
EOF])

dnl Taf!
