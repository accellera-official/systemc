This directory allows the examples to be launched from 
the command line on Windows.

It has been tested on Windows XP SP2 with Microsoft Visual C++ 2005 .NET Express Edition SP1
and with Microsoft Visual C++ 2003 .NET

Note
~~~~
In normal use, you'll probably find it easier to use the Visual C++ integrated
development environment. However this command line build system is useful for 
regression of examples with self-checking.


Usage (Visual C++ 2005 Express Edition)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Check the paths to TLM and SYSTEMC in the file Makefile.config
2. double-click openhere.bat to open a cmd prompt
3. Run vsvars32.bat to set up Visual C++ paths
4. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean examples
nmake run              # run examples
nmake                  # compile examples
nmake check            # run examples and compare against golden results


Usage (Visual C++ 2003 .NET)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To use with e.g. Visual C++ 2003 .NET you need to 

1. copy MakefileVC7.config to Makefile.config (you might want to back-up
   Makefile.config first)
2. Check the paths to TLM and SYSTEMC in Makefile.confg
3. double-click openhere.bat to open a cmd prompt
4. Run vsvars32VC7.bat to set  up Visual C++ paths
5. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean examples
nmake run              # run examples
nmake                  # compile examples
nmake check            # run examples and compare against golden results
