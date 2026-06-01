Running the SystemC Regression Suite on Windows
===============================================

To run the regression tests on Windows, a basic Un*x shell environment like

  - Cygwin 1.3.x, or later       (http://cygwin.org)
  - MinGW/MSYS 1.0.14, or later  (http://mingw.org)

is required, even when using the Microsoft Visual C++ compiler (CL).
The test script uses the command-line version of the Visual C++ compiler.

In the instructions that follow, it is assumed that you have installed
Visual Studio 2010, other versions later than Visual C++ 2010
will also work as documented in the SystemC library itself.

Menus (such as the start menu path to Visual C++) are taken from a Windows 7
32-bit installation. You may need to make slight adjustments to the names if
using a different version of Windows.


Installing SystemC
==================

When you install SystemC, make sure that the folder that contains the SystemC
Visual C++ project has the correct name for the version of Visual C++ you are
using.  This can be done by copying the default 'msvc10' folder to the
appropriate location before upgrading the solution/project files).

For Visual C++ 2015      : msvc14
For Visual C++ 2013      : msvc12
For Visual C++ 2012      : msvc11
For Visual C++ 2010      : msvc10


Running the regressions
=======================

1) The INCLUDE, LIB, and PATH environment variables must be set in Windows 
   before launching a Cygwin or MinGW shell.

   The quickest way to do this is to launch a Visual C++ 2010 command prompt:

     Start
       > Programs
       > Microsoft Visual Studio 2010
       > Visual Studio Command Prompt (2010)

   Alternatively, you can open a plain command prompt and use the
   'scripts\vsvars.bat' file to setup the Visual C++ tools environment.
   With this batch file, you can specify the version and the platform to
   use via additional command-line options given to vsvars.bat, e.g.

      cd c:\systemc_regressions-2.3.1\scripts

      vsvars.bat [arch]               # load MSVC 2010 for [arch]
      vsvars.bat [version]            # load MSVC [version] for x86
      vsvars.bat [version] [platform] # load MSVC [version] for [platform]

      vsvars.bat 2013 amd64  # load 64-bit tools for MSVC 12.0
      vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
      vsvars.vat x86_amd64   # load x64 cross-tools for MSVC 2010


2) From that command prompt, launch a shell for your Un*x shell compatibility
   layer (Cygwin or MinGW):

   Cygwin:
     c:\cygwin\cygwin.bat

   MinGW:
     c:\MinGW\msys\1.0\msys.bat

The remaining commands are typed at the shell prompt that should
have now appeared in a new window.

2) The SYSTEMC_HOME environment variable must be set to SystemC install
   directory, e.g. for a SystemC installation at "c:\systemc-2.3.1":

   MinGW  (drop drive colon and use '/' instead of '\'):
     export SYSTEMC_HOME=/c/systemc-2.3.1

   Cygwin (additionally, paths are prefixed with '/cygdrive'):
     export SYSTEMC_HOME=/cygdrive/c/systemc-2.3.1

3) Set the compiler to use Visual C++:
   (otherwise the Cygwin/MinGW GNU C++ compiler will be used by default)

     export CXX=cl

4) Change to the systemc-regressions folder and create a directory to
   hold the test results and cd into it, e.g.:

     cd systemc-regressions
     mkdir run
     cd run

5) Run the verify.pl script as described in the main README, e.g.:

   ../scripts/verify.pl systemc     (runs all SystemC tests using release library)
   ../scripts/verify.pl -g systemc  (runs all SystemC tests using debug library)
   ../scripts/verify.pl tlm         (runs all TLM2 tests using release library)
   ../scripts/verity.pl -g tlm      (runs all TLM2 tests using debug library)

   Note: If you built the SystemC library as a Windows DLL, please add the option
         '-dll' to the verify.pl invocation for the libraries to be linked correctly.

6) After all tests are complete, the output summary should show 100%
   passes for both systemc and tlm.
