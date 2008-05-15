
TLM-2.0 standard header files
=============================
	
Dir: include/tlm/

SubDirs: tlm_h/
	 tlm_utils/

Files: README.txt
       tlm.h


Comments
========

To use the TLM-2.0 interoperability standard a user should only include the 
tlm.h header file.The tlm.h header file refers to all the header files within 
the tlm_h subdirectoy, everything within tlm.his contained in the tlm namespace.
The TLM-2.0 interoperability standard also contains the TLM-1.0 standard, these headers are included in tlm.h

The tlm_utils subdirectory, contains a set of additional definitions supported 
by the TLM-2.0 standard but which are not part of the interoperability 
requirements.It contains ease-of-use and convenience implementations for the 
interoperability standard. All objects defined in the tlm_utils directory are 
contained in the tlm_util namespace.

See the README.txt files in the subdirectories for an explanation of the 
internal organization of the header files.

