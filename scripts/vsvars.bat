@echo off
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

if not "%1" == "" set SYSTEMC_MSVC_PLATFORM=%1
if     "%1" == "" set SYSTEMC_MSVC_PLATFORM=x86
goto check_MSVC80

rem We rely on the variables VSXXXCOMNTOOLS to be set by the MSVC
rem installation.  This should be usually the case by default.

:check_MSVC80
set SYSTEMC_MSVC_VERSION=8.0 (2005)
set VSINSTALLDIR=%VS80COMNTOOLS%..\..\
goto load_MSVC

:check_MSVC90
set SYSTEMC_MSVC_VERSION=9.0 (2008)
set VSINSTALLDIR=%VS90COMNTOOLS%..\..\
goto load_MSVC

:check_MSVC100
set SYSTEMC_MSVC_VERSION=10.0 (2010)
set VSINSTALLDIR=%VS100COMNTOOLS%..\..\
goto load_MSVC

:check_MSVC110
set SYSTEMC_MSVC_VERSION=11.0 (2012)
set VSINSTALLDIR=%VS110COMNTOOLS%..\..\
goto load_MSVC

:check_MSVC120
set SYSTEMC_MSVC_VERSION=12.0 (2013)
set VSINSTALLDIR=%VS120COMNTOOLS%..\..\
goto load_MSVC

:load_MSVC
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=%2
if "%SYSTEMC_MSVC_PLATFORM%" == "" set SYSTEMC_MSVC_PLATFORM=x86
if not exist "%VSINSTALLDIR%" goto error_no_MSVC_VERSION
set VCINSTALLDIR=%VSINSTALLDIR%VC\
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
