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
##  src/sysc/datatypes/Makefile.am --
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

noinst_LTLIBRARIES = libdatatypes.la

H_FILES = \
	bit/sc_bit.h \
	bit/sc_bit_ids.h \
	bit/sc_bit_proxies.h \
	bit/sc_bv.h \
	bit/sc_bv_base.h \
	bit/sc_logic.h \
	bit/sc_lv.h \
	bit/sc_lv_base.h \
	bit/sc_proxy.h \
	\
	fx/fx.h \
	fx/sc_context.h \
	fx/sc_fix.h \
	fx/sc_fixed.h \
	fx/sc_fx_ids.h \
	fx/sc_fxcast_switch.h \
	fx/sc_fxdefs.h \
	fx/sc_fxnum.h \
	fx/sc_fxnum_observer.h \
	fx/sc_fxtype_params.h \
	fx/sc_fxval.h \
	fx/sc_fxval_observer.h \
	fx/sc_ufix.h \
	fx/sc_ufixed.h \
	fx/scfx_ieee.h \
	fx/scfx_mant.h \
	fx/scfx_other_defs.h \
	fx/scfx_params.h \
	fx/scfx_rep.h \
	fx/scfx_string.h \
	fx/scfx_utils.h \
	\
	int/sc_bigint.h \
	int/sc_biguint.h \
	int/sc_int.h \
	int/sc_int_base.h \
	int/sc_int_ids.h \
	int/sc_length_param.h \
	int/sc_nbdefs.h \
	int/sc_nbexterns.h \
	int/sc_nbutils.h \
	int/sc_signed.h \
	int/sc_uint.h \
	int/sc_uint_base.h \
	int/sc_unsigned.h \
	\
	misc/sc_concatref.h \
	misc/sc_value_base.h

NO_H_FILES = \
	int/sc_nbcommon.inc \
	int/sc_nbfriends.inc \
	int/sc_signed_bitref.inc \
	int/sc_signed_subref.inc \
	int/sc_unsigned_bitref.inc \
	int/sc_unsigned_subref.inc \
	\
	fx/scfx_pow10.h

CXX_FILES = \
	bit/sc_bit.cpp \
	bit/sc_bv_base.cpp \
	bit/sc_logic.cpp \
	bit/sc_lv_base.cpp \
	\
	fx/sc_fxcast_switch.cpp \
	fx/sc_fxdefs.cpp \
	fx/sc_fxnum.cpp \
	fx/sc_fxnum_observer.cpp \
	fx/sc_fxtype_params.cpp \
	fx/sc_fxval.cpp \
	fx/sc_fxval_observer.cpp \
	fx/scfx_mant.cpp \
	fx/scfx_pow10.cpp \
	fx/scfx_rep.cpp \
	fx/scfx_utils.cpp \
	\
	int/sc_int_base.cpp \
	int/sc_int32_mask.cpp \
	int/sc_int64_io.cpp \
	int/sc_int64_mask.cpp \
	int/sc_length_param.cpp \
	int/sc_nbdefs.cpp \
	int/sc_nbexterns.cpp \
	int/sc_nbutils.cpp \
	int/sc_signed.cpp \
	int/sc_uint_base.cpp \
	int/sc_unsigned.cpp \
	\
	misc/sc_concatref.cpp \
	misc/sc_value_base.cpp

localincludedir = $(includedir)/sysc/datatypes
nobase_localinclude_HEADERS = $(H_FILES)

libdatatypes_la_SOURCES = $(NO_H_FILES) $(CXX_FILES)

## Taf!
