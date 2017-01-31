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

include $(top_srcdir)/config/Make-rules.sysc

H_FILES = \
	bind.hpp \
	config.hpp \
	get_pointer.hpp \
	mem_fn.hpp \
	non_type.hpp \
	ref.hpp \
	type.hpp \
	\
	bind/apply.hpp \
	bind/arg.hpp \
	bind/bind_cc.hpp \
	bind/bind_mf_cc.hpp \
	bind/bind_template.hpp \
	bind/make_adaptable.hpp \
	bind/mem_fn_cc.hpp \
	bind/mem_fn_template.hpp \
	bind/mem_fn_vw.hpp \
	bind/placeholders.hpp \
	bind/protect.hpp \
	\
	config/compiler/borland.hpp \
	config/compiler/comeau.hpp \
	config/compiler/common_edg.hpp \
	config/compiler/compaq_cxx.hpp \
	config/compiler/digitalmars.hpp \
	config/compiler/gcc.hpp \
	config/compiler/greenhills.hpp \
	config/compiler/hp_acc.hpp \
	config/compiler/intel.hpp \
	config/compiler/kai.hpp \
	config/compiler/metrowerks.hpp \
	config/compiler/mpw.hpp \
	config/compiler/sgi_mipspro.hpp \
	config/compiler/sunpro_cc.hpp \
	config/compiler/vacpp.hpp \
	config/compiler/visualc.hpp \
	config/platform/aix.hpp \
	config/platform/amigaos.hpp \
	config/platform/beos.hpp \
	config/platform/bsd.hpp \
	config/platform/cygwin.hpp \
	config/platform/hpux.hpp \
	config/platform/irix.hpp \
	config/platform/linux.hpp \
	config/platform/macos.hpp \
	config/platform/solaris.hpp \
	config/platform/win32.hpp \
	config/posix_features.hpp \
	config/select_compiler_config.hpp \
	config/select_platform_config.hpp \
	config/select_stdlib_config.hpp \
	config/stdlib/dinkumware.hpp \
	config/stdlib/libcomo.hpp \
	config/stdlib/libstdcpp3.hpp \
	config/stdlib/modena.hpp \
	config/stdlib/msl.hpp \
	config/stdlib/roguewave.hpp \
	config/stdlib/sgi.hpp \
	config/stdlib/stlport.hpp \
	config/stdlib/vacpp.hpp \
	config/suffix.hpp \
	config/user.hpp \
	\
	detail/endian.hpp \
	detail/workaround.hpp \
	\
	mpl/aux_/adl_barrier.hpp \
	mpl/aux_/config/adl.hpp \
	mpl/aux_/config/gcc.hpp \
	mpl/aux_/config/intel.hpp \
	mpl/aux_/config/msvc.hpp \
	mpl/aux_/config/static_constant.hpp \
	mpl/aux_/config/workaround.hpp \
	mpl/bool.hpp \
	mpl/bool_fwd.hpp \
	mpl/integral_c_tag.hpp \
	\
	utility/addressof.hpp \
	utility/enable_if.hpp \
	utility/string_view.hpp \
	utility/string_view_fwd.hpp


EXTRA_DIST += \
	LICENSE_1_0.txt

localincludedir = $(includedir)/sysc/packages/boost
nobase_localinclude_HEADERS = $(H_FILES)


## Taf!
