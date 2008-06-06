
TLM-2.0 unit tests
==================
	
Dir: unit_test/tlm/
SubDirs: build-cygwin/
         build-msvc/
         build-unix/
         bus/
         bus_dmi/
         common/
         endian_conv/
         gp_extension/
         multi_sockets/
         p2p/
         static_extensions/
Files: README.txt


Comments
========

Running the tests:
------------------
Support for building the examples and unit tests is provided for 'unix', and
'msvc' (project and make files).

Makefiles work hierarchically. The default target builds the tests; 
Other targets are:
   'clean', 
   'run'  (executes built tests/examples) 
   'check' (against expected results)

To run the makefiles the following environment needs to be set:
   On Unix/Linux/MacOS, 
      $TLM_HOME should be set to this kit's location
      $SYSTEMC_HOME shold point to the SystemC installation
      $TARGET_ARCH defines the appendix of the systemc library directory
      these should be set such that $SYSTEMC_HOME/lib-$TARGET_ARCH is a 
      directory containing the systemc library.  
   On Windows, the SYSTEMC and TLM environments variables should be used.
   
   The multi_sockets test is dependent on the boost library, you will need to 
   have an environment variable 'BOOST_HOME' set to build the test
   
   To run the tests with SystemC-2.1v1, boost and gcc-3.4.4 you need to change
   the Makefile.config in build-unix/ to unset the FLAG_WERROR
   You also need to unset this flag when using gcc-4.1.2 and above


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
    
endian_conv/ :
      unit test for the endianness conversion ftions, there is only a build for
      linux provided, the C++ test performs a single conversion, there is a 
      python script using the program to do a more extensive test     
      
multi_sockets/ :
      test system using a TLM2 compliant AT busmodel using a single socket to
      bind all targets and initiators to, in combination with LT and AT
      initiators and targets. This test also uses instance specific extentions
      in the generic payload
      
      NOTE: this test does not compile with SystemCv2.1.v1 due to 
            incompatibilities with the boost library that is required for the
	    multi_socket implementation
      
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
