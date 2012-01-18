
TLM-2.0 unit tests
==================
	
Dir: unit_test/tlm/
SubDirs: build-cygwin/
         build-msvc/
         build-unix/
         bus/
         bus_dmi/
         cancel_all/
         common/
         endian_conv/
         multi_sockets/
         nb2b_adapter/
         p2p/
         static_extensions/
         update_original/
Files: README.txt


Comments
========

Running the tests:
------------------

Support for building the examples and unit tests is provided for makefiles under 'unix', 
and for both makefiles and Microsoft Visual C++ project files under 'msvc' 

Automated regression tests can be run under Unix/Linux and Microsoft Windows using hierarchical makefiles. The default target builds the tests; 
Other targets are:
   'clean', 
   'run'   (executes tests/examples) 
   'check' (actual against expected results)

To run the makefiles the following environment needs to be set:

On Unix/Linux/MacOS, 
   $TLM_HOME should be set to this kit's location
   $SYSTEMC_HOME shold point to the SystemC installation
   $TARGET_ARCH defines the appendix of the systemc library directory
   these should be set such that $SYSTEMC_HOME/lib-$TARGET_ARCH is a 
   directory containing the systemc library.  
      
   To run the tests with SystemC-2.1v1 and gcc-3.4.4 you need to change
   the Makefile.config in build-unix/ to unset the FLAG_WERROR
   You also need to unset this flag when using gcc-4.1.2 and above
   
   To run the tests with SystemC-2.2.0 and gcc-4.2.2 and above, 
   edit the file systemc-2.2.0/include/sysc/packages/boost/bind/placeholders.hpp
   change line 28
   from
      #if defined(__BORLANDC__)
    to
      #if defined(__BORLANDC__) || defined(__GNUC__)

On Windows, the SYSTEMC and TLM environments variables should be used.
   For detailed instructions see ./build_msvc/READMEcommandlinemsvc.txt
   
   Optionally, to run the Visual C++ solutions (.sln) or project (.vcproj) files, 
   you must edit the Property sheet as follows:

   1 Select Property Manager from the View menu
   2 Under projectName > Debug | Win32 select systemc
   3 Select Properties from the View menu
   4 Select User Macros under Common Properties
   5 Update the SYSTEMC and TLM entries and apply


The tests:
----------

All tests are build using a set of models that can be found in the subdirectory
common/include/models. For a description of the models see the README.txt in 
that subdirectory.

The test themselves can be found in the following subdirectories:
bus/ : 
      test system using a TLM2 compliant AT bus model and a combination of
      LT and AT targets and initiators, with and without temporal decoupling. 
      
bus_dmi/ : 
      test system using a similar system as in the bus/ test but now with DMI
      support added for the LT initiators and targets.
      
cancel_all/ :
      Tests the cancel_all() methods of the two PEQs, peq_with_cb_and_phase and peq_with_get.
      Along the way, it also tests the basic operation of each PEQ.
    
endian_conv/ :
      unit test for the endianness conversion ftions, there is only a build for
      linux provided, the C++ test performs a single conversion, there is a 
      python script using the program to do a more extensive test     
      
multi_sockets/ :
      test system using a TLM2 compliant AT busmodel using a single socket to
      bind all targets and initiators to, in combination with LT and AT
      initiators and targets. This test also uses instance specific extentions
      in the generic payload
      
nb2b_adapter/ :
      Primarily a regression test for a bug in the nb2b adapter of the simple_target_socket.
      Tests the operation of the nb2b adapter, and also exercises the peq_with_cb_and_phase
      and the instance_specific_extension.
      
p2p/ :
  BaseSocketLT/ :
  	simple point to point test for LT initiator and target
  CoreDecoupling/
  	simple test for using the LT initiator with temporal decoupling
  EndEventLT/
  	??
  HierarchicalSocket/
    	tests sockets on hierachical modules
  RegisterSocketProcessLT/
  	simple test for initiator and target using callback registry in sockets
  SimpleAT/
  	simple point to point test for AT initiator and target using GP
	AT phases and TLM_ACCEPTED
  SimpleAT_TA/
  	simple point to point test for AT initiator and target using GP
	AT phases and TLM_UPDATED

static_extentions/ :
	contains 3 unit tests to verify the GP extention mechanism

update_original/ :
      Tests the deep_copy_from() and update_original_from() methods of tlm_generic_payload. 
      Along the way, it also tests the use of byte enables for read and write commands.