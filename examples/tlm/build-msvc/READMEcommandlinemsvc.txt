This directory allows the examples to be launched from 
the command line on Windows.

It has been tested on Windows XP SP2 with Microsoft Visual C++ 2005 .NET Express Edition SP1.

Note
~~~~
In normal use, you'll probably find it easier to use the Visual C++ integrated
development environment. However this command line build system is useful for 
regression of examples with self-checking.

Usage
~~~~~

1. Check the paths to TLM and SYSTEMC in the file Makefile.config
2. double-click openhere.bat to open a cmd prompt
3. Run vsvars32.bat to set up Visual C++ paths
4. You can then use the Microsoft tool nmake, e.g.

nmake clean            # clean all unit-tests
nmake run              # run all unit-tests
nmake                  # compile all unit-tests
nmake check            # run all tests and compare against golden results


Changing to other versions
~~~~~~~~~~~~~~~~~~~~~~~~~~
To use with e.g. Visual C++ 2003 .NET you need to 

1. find the appropriate version of vsvars32.bat. This is normally in 

C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools

2. Edit the include and library paths in Makefile.config

Then the above steps should work.


Alan Fitch
2007-11-13
