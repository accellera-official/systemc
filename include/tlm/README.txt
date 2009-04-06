
TLM-2.0 standard header files
=============================
	
Dir: include/tlm/

SubDirs: tlm_1/
         tlm_h/
	 tlm_utils/

Files: README.txt
       tlm.h


Comments
========

To use the TLM-2.0 interoperability standard a user should only include the 
tlm.h header file. The same holds for the TLM-1.0 implementation that is 
included as part of this kit; only include tlm.h. The tlm.h header file refers 
to all the header files within the tlm_h and tlm_1 subdirectories, everything 
within tlm.h is contained in the tlm namespace. 

The tlm_utils subdirectory contains a set of additional definitions supported 
by the TLM-2.0 standard but which are not part of the interoperability 
requirements. It contains ease-of-use and convenience implementations for the 
interoperability standard. All objects defined in the tlm_utils directory are 
contained in the tlm_util namespace.

See the README.txt files in the subdirectories for an explanation of the 
internal organization of the header files.

