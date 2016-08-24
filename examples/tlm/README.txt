
TLM-2.0 examples
================

Dir: examples/tlm/
SubDirs: at_1_phase/
         at_2_phase/
         at_4_phase/
         at_extension_optional/
         at_mixed_targets/
         at_ooo/
         build-msvc/
         build-unix/
         common/
         lt/
         lt_dmi/
         lt_extension_mandatory/
         lt_temporal_decouple/
         lt_mixed_endian/
Files: README.txt


Comments
========

Running the examples:
---------------------

Support for building the examples and unit tests is provided for 'unix', and
'msvc' (project and make files).

Makefiles work hierarchically. The default target builds the tests; 
Other targets are:
   'clean', 
   'run'  (executes built tests/examples) 
   'check' (against expected results)

To run the makefiles the following environment needs to be set:
   On Unix/Linux/MacOS,
      $SYSTEMC_HOME should point to the SystemC installation
  
   On Windows, the SYSTEMC_HOME and MSVC environment variable should be used.

To run the Visual C++ solutions (.sln) or project (.vcproj) files, you must edit 
the Property sheet tlm_examples.props as follows:

1 Select Property Manager from the View menu
2 Under projectName > Debug | Win32 select systemc
3 Select Properties from the View menu
4 Select User Macros under Common Properties
5 Update the MSVC entry and apply


The examples:
-------------

Each examples comes with documentation in the form of a set of powerpoint slides
that can be found in the docs/ subdirectory of the example.
