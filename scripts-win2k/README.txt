Running the SystemC Regression Suite with Visual C++ 7.1
========================================================

These tests must be run using Cygwin and the command-line version of the
Visual C++ compiler (CL).

1) The INCLUDE, LIB, and PATH environment variables must be set in Windows 
   before launching a Cygwin shell. The correct setting for these can be
   obtained by running the vcvars32.bat file (in the Vc7\bin subdirectory
   of the Visual Studio directory), e.g.

     INCLUDE = C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\include;
               C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\include;
               C:\Program Files\Microsoft Visual Studio .NET 2003\SDK\v1.1\include\

     LIB = C:\Program Files\Microsoft Visual Studio .NET 2003\VC7\LIB;
           C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Lib;
           C:\Program Files\Microsoft Visual Studio .NET 2003\SDK\v1.1\Lib\
           
     VCINSTALLDIR = C:\Program Files\Microsoft Visual Studio .NET 2003
     DevEnvDir = %VSINSTALLDIR%
     MSVCDir = C:\Program Files\Microsoft Visual Studio .NET 2003
     PATH = %DevEnvDir%;%MSVCDir%\BIN;%VCINSTALLDIR%\Common7\Tools;%PATH%

2) The SYSTEMC_HOME environment variable must be set to SystemC install 
   directory, e.g.

        SYSTEMC_HOME =  C:\apps\Systemc2.2

3) The CXX environment variable must be set to cl

4) Launch a Cygwin shell and cd to the systemc_tests-xxx directory.

5) Create a new directory to hold the test results and cd into it, e.g.

   $> mkdir run
   $> cd run
   
6) Run the verify.pl script as described in the main releasenotes, e.g.

   $> ../scripts-win2k/verify.pl systemc  (runs all tests using release library)
   $> ../scripts-win2k/verify.pl systemc  (runs all tests using debug library)
   
7) After all tests are complete, the output summary should look similar to the
   following:
   
-------------------------------------------------------------------------------
 Tests that failed :
-------------------------------------------------------------------------------
                diff                : systemc/communication/sc_semaphore/test02/
test02.cpp
                diff                : systemc/datatypes/int/arith/arith09/arith0
9.cpp
                diff                : systemc/datatypes/int/misc/test02/test02.c
pp
                diff                : systemc/datatypes/int/parse_binary_bits/te
st02/test02.cpp
             compiling              : systemc/examples/isqrt/isqrt.cpp
                diff                : systemc/kernel/dynamic_processes/test02/te
st02.cpp

Total passed : 726
Total failed : 6

   GUT METER : 99 %

*** verify.pl finished successfully on host nnnnnnn   
        
The differences in the above results from the "golden" models are believed to 
be due to limitations and different interpretations of the C++ standard by 
the MSVC compiler.

