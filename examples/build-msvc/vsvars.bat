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
rem  You can specify the version and the platform to use via additional
rem  command-line options given to vsvars.bat, e.g.
rem
rem    vsvars.bat [arch]               # load MSVC 2010 for [arch]
rem    vsvars.bat [version]            # load MSVC [version] for x86
rem    vsvars.bat [version] [platform] # load MSVC [version] for [platform]
rem
rem    vsvars.bat 2010 amd64  # load 64-bit tools for MSVC 10.0
rem    vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
rem    vsvars.bat x86_amd64   # load x64 cross-tools for MSVC 2010
rem
rem ***************************************************************************

set SYSTEMC_MSVC_VERSION=
set SYSTEMC_MSVC_PLATFORM=
set VCVARSDIR=

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
if "%1" == "15.0"  goto check_MSVC150
if "%1" == "2017"  goto check_MSVC150
if "%1" == "16.0"  goto check_MSVC160
if "%1" == "2019"  goto check_MSVC160

if not "%1" == "" set SYSTEMC_MSVC_PLATFORM=%1
if     "%1" == "" set SYSTEMC_MSVC_PLATFORM=x86
goto check_MSVC100

rem We rely on the variables VSXXXCOMNTOOLS to be set by the MSVC
rem installation.  This should be usually the case by default.

rem For MSVC 2017 and newer, VSXXXCOMNTOOLS is not defined anymore,
rem and we need to set the variable ourselves. We will check for the
rem Community and Professions release in the most commonly used 
rem install directories 

:check_MSVC80
set SYSTEMC_MSVC_VERSION=8.0 (2005)
set VSINSTALLDIR=%VS80COMNTOOLS%..\..\
set MSVC=msvc80
goto load_MSVC

:check_MSVC90
set SYSTEMC_MSVC_VERSION=9.0 (2008)
set VSINSTALLDIR=%VS90COMNTOOLS%..\..\
set MSVC=msvc90
goto load_MSVC

:check_MSVC100
set SYSTEMC_MSVC_VERSION=10.0 (2010)
set VSINSTALLDIR=%VS100COMNTOOLS%..\..\
set MSVC=msvc10
goto load_MSVC

:check_MSVC110
set SYSTEMC_MSVC_VERSION=11.0 (2012)
set VSINSTALLDIR=%VS110COMNTOOLS%..\..\
set MSVC=msvc11
goto load_MSVC

:check_MSVC120
set SYSTEMC_MSVC_VERSION=12.0 (2013)
set VSINSTALLDIR=%VS120COMNTOOLS%..\..\
set MSVC=msvc12
goto load_MSVC

:check_MSVC140
set SYSTEMC_MSVC_VERSION=14.0 (2015)
set VSINSTALLDIR=%VS140COMNTOOLS%..\..\
set MSVC=msvc14
goto load_MSVC

:check_MSVC150
set SYSTEMC_MSVC_VERSION=15.0 (2017)
set MSVC=msvc15
set VCVARSDIR=Auxiliary\Build\
set VS150COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\
set VSINSTALLDIR=%VS150COMNTOOLS%..\..\
if exist "%VSINSTALLDIR%" goto load_MSVC
set VS160COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\
set VSINSTALLDIR=%VS160COMNTOOLS%..\..\
goto load_MSVC

:check_MSVC160
set SYSTEMC_MSVC_VERSION=16.0 (2019)
set MSVC=msvc16
set VCVARSDIR=Auxiliary\Build\
set VS160COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\
set VSINSTALLDIR=%VS160COMNTOOLS%..\..\
if exist "%VSINSTALLDIR%" goto load_MSVC
set VS160COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\
set VSINSTALLDIR=%VS160COMNTOOLS%..\..\
goto load_MSVC

:load_MSVC
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=%2
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=x86
if not exist "%VSINSTALLDIR%" goto error_no_MSVC_VERSION
set VCINSTALLDIR=%VSINSTALLDIR%VC\%VCVARSDIR%
if not exist "%VCINSTALLDIR%vcvarsall.bat" goto error_no_MSVC_VERSION
echo Loading settings for MS Visual C++ %SYSTEMC_MSVC_VERSION% (%SYSTEMC_MSVC_PLATFORM% platform)
call "%VCINSTALLDIR%vcvarsall.bat" %SYSTEMC_MSVC_PLATFORM%
goto end

:error_no_MSVC_VERSION
echo MS Visual C++ %SYSTEMC_MSVC_VERSION% not found.
echo Could not load compiler environment.
echo Check your MS Visual C++ %SYSTEMC_MSVC_VERSION% installation.
goto end

:end
