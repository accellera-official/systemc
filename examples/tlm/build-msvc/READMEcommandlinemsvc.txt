This directory allows the examples to be launched from 
the command line on Windows.

It has been tested on Windows XP SP2 with the following compiler versions:

  Microsoft Visual C++ 2005 .NET Express Edition SP1
  Microsoft Visual C++ 2003 .NET
  Microsoft Visual C++ 2008 .NET Express Edition

Note
~~~~
In normal use, you'll probably find it easier to use the Visual C++ integrated
development environment. However this command line build system is useful for
regression of unit tests with self-checking.

Usage (Visual C++ 2005 Express Edition)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Check the path to SystemC in the file Makefile.config
2. double-click openhere.bat to open a cmd prompt
3. Run vsvars32.bat to set up Visual C++ paths
4. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean unit tests
nmake run              # run unit tests
nmake                  # compile unit tests
nmake check            # run unit tests and compare against golden results


Usage (Visual C++ 2008 Express Edition)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. copy MakefileVC9.config to Makefile.config (you might want to back-up
   Makefile.config first)
2. Check the path to SystemC in the file Makefile.config
3. double-click openhere.bat to open a cmd prompt
4. Run vcvars32VC9.bat to set up Visual C++ paths
5. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean unit tests
nmake run              # run unit tests
nmake                  # compile unit tests
nmake check            # run unit tests and compare against golden results

You may see messages like this:
"Unknown compiler version - please run the configure tests and report the results"
This is issued by the Boost library, which is used by both SystemC and by some
TLM Kit code. It can be safely ignored.



Usage (Visual C++ 2003 .NET)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. copy MakefileVC7.config to Makefile.config (you might want to back-up
   Makefile.config first)
2. Check the path to SystemC in Makefile.config
3. double-click openhere.bat to open a cmd prompt
4. Run vsvars32VC7.bat to set  up Visual C++ paths
5. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean unit tests
nmake run              # run unit tests
nmake                  # compile unit tests
nmake check            # run unit tests and compare against golden results
