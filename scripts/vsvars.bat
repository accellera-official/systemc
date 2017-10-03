@echo off
rem ***************************************************************************
rem
rem  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
rem  more contributor license agreements.  See the NOTICE file distributed
rem  with this work for additional information regarding copyright ownership.
rem  Accellera licenses this file to you under the Apache License, Version 2.0
rem  (the "License"); you may not use this file except in compliance with the
rem  License.  You may obtain a copy of the License at
rem
rem    http://www.apache.org/licenses/LICENSE-2.0
rem
rem  Unless required by applicable law or agreed to in writing, software
rem  distributed under the License is distributed on an "AS IS" BASIS,
rem  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
rem  implied.  See the License for the specific language governing
rem  permissions and limitations under the License.
rem
rem ***************************************************************************
rem
rem  vsvars.bat --
rem  Run this file in a command prompt to load MS Visual C++ environment
rem
rem  Original Author: Philipp A. Hartmann, OFFIS, 2013-12-09
rem
rem ***************************************************************************
rem
rem  You can specify the version and the architecture to use via additional
rem  command-line options given to vsvars.bat, e.g.
rem
rem    vsvars.bat [arch]           # load MSVC 2010 for [arch]
rem    vsvars.bat [version]        # load MSVC [version] for x86
rem    vsvars.bat [version] [arch] # load MSVC [version] for [arch]
rem
rem    vsvars.bat 2010 amd64  # load 64-bit tools for MSVC 10.0
rem    vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
rem    vsvars.vat x86_amd64   # load x64 cross-tools for MSVC 2010
rem
rem  The installation location for each version is determined by the
rem      VS<major><minor>COMNTOOLS
rem  environment variable (e.g. VS140COMNTOOLS for version 14.0/2015).
rem
rem  Note: Visual Studio 2017 no longer sets the VS<ver>COMNTOOLS variable
rem        by default.  In order to use this helper script, you need to
rem        set it manually in your environment.
rem ***************************************************************************

set SYSTEMC_MSVC_VERSION=
set SYSTEMC_MSVC_PLATFORM=

if "%1" == "8.0"   goto check_MSVC80
if "%1" == "2005"  goto check_MSVC80
if "%1" == "9.0"   goto check_MSVC90
if "%1" == "2008"  goto check_MSVC90
if "%1" == "10.0"  goto check_MSVC100
if "%1" == "2010"  goto check_MSVC100
if "%1" == "11.0"  goto check_MSVC110
if "%1" == "2011"  goto check_MSVC110
if "%1" == "2012"  goto check_MSVC110
if "%1" == "12.0"  goto check_MSVC120
if "%1" == "2013"  goto check_MSVC120
if "%1" == "2014"  goto check_MSVC120
if "%1" == "14.0"  goto check_MSVC140
if "%1" == "2015"  goto check_MSVC140
if "%1" == "14.1"  goto check_MSVC141
if "%1" == "15.0"  goto check_MSVC150
if "%1" == "2017"  goto check_MSVC150

if not "%1" == "" set SYSTEMC_MSVC_PLATFORM=%1
if     "%1" == "" set SYSTEMC_MSVC_PLATFORM=x86
goto check_MSVC100

rem We rely on the variables VSXXXCOMNTOOLS to be set by the MSVC
rem installation.  This should be usually the case by default.

:check_MSVC80
set SYSTEMC_MSVC_VERSION=8.0 (2005)
set SYSTEMC_VSCOMNTOOLS=%VS80COMNTOOLS%
set SYSTEMC_VSVERSHORT=80
goto load_MSVC

:check_MSVC90
set SYSTEMC_MSVC_VERSION=9.0 (2008)
set SYSTEMC_VSCOMNTOOLS=%VS90COMNTOOLS%
set SYSTEMC_VSVERSHORT=90
goto load_MSVC

:check_MSVC100
set SYSTEMC_MSVC_VERSION=10.0 (2010)
set SYSTEMC_VSCOMNTOOLS=%VS100COMNTOOLS%
set SYSTEMC_VSVERSHORT=100
goto load_MSVC

:check_MSVC110
set SYSTEMC_MSVC_VERSION=11.0 (2012)
set SYSTEMC_VSCOMNTOOLS=%VS110COMNTOOLS%
set SYSTEMC_VSVERSHORT=110
goto load_MSVC

:check_MSVC120
set SYSTEMC_MSVC_VERSION=12.0 (2013)
set SYSTEMC_VSCOMNTOOLS=%VS120COMNTOOLS%
set SYSTEMC_VSVERSHORT=120
goto load_MSVC

:check_MSVC140
set SYSTEMC_MSVC_VERSION=14.0 (2015)
set SYSTEMC_VSCOMNTOOLS=%VS140COMNTOOLS%
set SYSTEMC_VSVERSHORT=140
goto load_MSVC

:check_MSVC141
rem In MSVC 2017, PlatformToolset and IDE version diverged.
rem  - report the PlatformToolset version here
set SYSTEMC_MSVC_VERSION=14.1 (2017)
rem  - we use the PlatformToolset instead of the IDE version for the arch identifier
set SYSTEMC_VSCOMNTOOLS=%VS141COMNTOOLS%
set SYSTEMC_VSVERSHORT=141
rem try %VS150COMNTOOLS%, if %VS141COMNTOOLS% is not set
if not exist "%SYSTEMC_VSCOMNTOOLS%vsdevcmd.bat" goto check_MSVC150
goto load_MSVC

:check_MSVC150
rem In MSVC 2017, PlatformToolset and IDE version diverged.
rem  - report the IDE version here
set SYSTEMC_MSVC_VERSION=15.0 (2017)
set SYSTEMC_VSCOMNTOOLS=%VS150COMNTOOLS%
set SYSTEMC_VSVERSHORT=150
goto load_MSVC

:load_MSVC
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=%2
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=x86

echo Platform = Microsoft Visual C++ %SYSTEMC_MSVC_VERSION% (%SYSTEMC_MSVC_PLATFORM% architecture)
echo Location = "%SYSTEMC_VSCOMNTOOLS%" (VS%SYSTEMC_VSVERSHORT%COMNTOOLS)

if not exist "%SYSTEMC_VSCOMNTOOLS%vsdevcmd.bat" goto load_MSVC_VARSALL
echo Loading vsdevcmd.bat for Microsoft Visual C++ %SYSTEMC_MSVC_VERSION% (%SYSTEMC_MSVC_PLATFORM% architecture)
call "%SYSTEMC_VSCOMNTOOLS%vsdevcmd.bat" -arch=%SYSTEMC_MSVC_PLATFORM%
goto end

rem Try locating and loading vsvarsall.bat manually
:load_MSVC_VARSALL
set SYSTEMC_VSINSTALLDIR=%SYSTEMC_VSCOMNTOOLS%..\..\
if not exist "%SYSTEMC_VSINSTALLDIR%" goto error_no_MSVC_VERSION
set SYSTEMC_VCINSTALLDIR=%SYSTEMC_VSINSTALLDIR%VC\
set SYSTEMC_VCVARSALL=%SYSTEMC_VCINSTALLDIR%vcvarsall.bat
if not exist "%SYSTEMC_VCVARSALL%" set SYSTEMC_VCVARSALL=%SYSTEMC_VCINSTALLDIR%Auxiliary\Build\vcvarsall.bat
if not exist "%SYSTEMC_VCVARSALL%" goto error_no_MSVC_VERSION
echo Loading vcvarsall.bat for Microsoft Visual C++ %SYSTEMC_MSVC_VERSION% (%SYSTEMC_MSVC_PLATFORM% architecture)
call "%SYSTEMC_VCVARSALL%" %SYSTEMC_MSVC_PLATFORM%
goto end

:error_no_MSVC_VERSION
echo ERROR: Microsoft Visual C++ %SYSTEMC_MSVC_VERSION% not found. Environment not loaded.
echo Please check your MS Visual C++ %SYSTEMC_MSVC_VERSION% installation, and make
echo sure to set the VS%SYSTEMC_VSVERSHORT%COMNTOOLS environment variable.

:end
