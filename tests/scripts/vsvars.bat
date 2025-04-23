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
rem    vsvars.bat [platform]           # load MSVC 2019 for [platform]
rem    vsvars.bat [version]            # load MSVC [version] for x86
rem    vsvars.bat [version] [platform] # load MSVC [version] for [platform]
rem
rem    vsvars.bat 2022 amd64  # load 64-bit tools for MSVC 2022 (17.0)
rem    vsvars.bat 17.0        # load default (x86) tools for MSVC 2022 (17.0)
rem    vsvars.bat x86_amd64   # load x64 cross-tools for MSVC 2019 (16.0)
rem
rem ***************************************************************************

set SYSTEMC_MSVC_VERSION=
set SYSTEMC_MSVC_PLATFORM=
set VCVARSDIR=

if "%1" == "15.0"  goto check_MSVC150
if "%1" == "2017"  goto check_MSVC150
if "%1" == "16.0"  goto check_MSVC160
if "%1" == "2019"  goto check_MSVC160
if "%1" == "17.0"  goto check_MSVC170
if "%1" == "2022"  goto check_MSVC170

if not "%1" == "" set SYSTEMC_MSVC_PLATFORM=%1
if     "%1" == "" set SYSTEMC_MSVC_PLATFORM=x86
goto check_MSVC160

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

:check_MSVC170
set SYSTEMC_MSVC_VERSION=17.0 (2022)
set MSVC=msvc17
set VCVARSDIR=Auxiliary\Build\
set VS170COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\Common7\Tools\
set VSINSTALLDIR=%VS170COMNTOOLS%..\..\
if exist "%VSINSTALLDIR%" goto load_MSVC
set VS170COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2022\Professional\Common7\Tools\
set VSINSTALLDIR=%VS170COMNTOOLS%..\..\
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
