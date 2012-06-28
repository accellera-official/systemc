Running the SystemC Regression Suite on Windows
===============================================

These tests must be run using Cygwin and the command-line version of the
Visual C++ compiler (CL). In the instructions that follow, it is assumed that 
you have installed Visual C++ 2010 Express Edition. 

Visual C++ 2008 and Visual C++ 2005 will also work. 

Please note that if using Visual C++ 2005, you will also need to install the 
Windows Server 2003 Platform SDK SP1 so that you have the Windows header files.

Menus (such as the start menu path to Visual C++) are taken from a Windows 7 32 bit
installation. You may need to make slight adjustments to the names if using a different
version of Windows.


Installing SystemC
==================

When you install SystemC, make sure that the folder that contains the SystemC 
Visual C++ project has the correct name for the version of Visual C++ you are 
using.

For Visual C++ 2010      : msvc10
For Visual C++ 2008      : msvc90
For Visual C++ 2005      : msvc80
For Visual C++ 2003 .NET : msvc71


Running the regressions
=======================

1) The INCLUDE, LIB, and PATH environment variables must be set in Windows 
   before launching a Cygwin shell. The quickest way to do this is to launch a 
   Visual C++ 2010 command prompt:

   Start > Programs >  Microsoft Visual Studio 2010 Express > Visual Studio Command Prompt (2010)

   [Note: for Visual C++ 2005, you should also run "c:\Program Files\Microsoft Platform SDK\setenv.cmd"]

2) From that command prompt, launch a Cygwin shell:

   c:\cygwin\cygwin.bat

The remaining commands are typed at the Cygwin prompt that should have now appeared.
   
2) The SYSTEMC_HOME environment variable must be set to SystemC install 
   directory, e.g.:

      export SYSTEMC_HOME=/cygdrive/c/systemc-2.3.0

3) Set the compiler:

   export CXX=cl
 
4) Change to the systemc-regressions folder and create a directory to
   hold the test results and cd into it, e.g.:

   cd systemc-regressions
   mkdir run
   cd run
   
5) Run the verify.pl script as described in the main releasenotes, e.g.:

   ../scripts/verify.pl systemc     (runs all systemc tests using release library)
   ../scripts/verify.pl -g systemc  (runs all systemc tests using debug library)
   ../scripts/verify.pl tlm         (runs all tlm2 tests using release library)
   ../scripts/verity.pl -g tlm      (runs all tlm2 tests using debug library)
   
6) After all tests are complete, the output summary should show 100%
   passes for both systemc and tlm.
