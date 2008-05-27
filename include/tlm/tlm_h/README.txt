
TLM-2.0 interoperability standard header files
==============================================
	
Dir: include/tlm/tlm_h/

SubDirs: tlm_analysis/
	 tlm_quantum/
	 tlm_req_rsp/
	 tlm_trans/

Files: README.txt
       tlm_version.h


Comments
========
User code should only #include the tlm.h header file in the include/ directory
and avoid including any of the include files in this directory directly. 
All objects defined in this file hierarchy are in the tlm namespace.

tlm_version.h contains the definitions for the version string and integer values



The header files are organizated, by subdirectory, as follows:

tlm_analysis/ 
--------------
This contains the basic mechanisms for doing analysis

Files:
  tlm_analysis.h        (includes the other header files in this directory )
  tlm_analysis_fifo.h   (defines tlm_analysis_fifo )
  tlm_analysis_if.h     (defines tlm_analysis_if and tlm_delayed_analysis_if )
  tlm_analysis_port.h   (defines tlm_analysis_port )
  tlm_analysis_triple.h (defines tlm_analysis_triple )
  tlm_write_if.h        (defines tlm_write_if and tlm_delayed_write_if )


tlm_quantum/
------------
This contains the quantum keeper for temporal decoupling:

Files:
  tlm_quantum.h        ( includes the other header file in this directory )
  tlm_global_quantum.h ( defines tlm_global_quantum ) 


tlm_trans/
----------
This contains the TLM2.0 additional interfaces with support for pass by 
reference, also contains the definitions for the TLM2.0 sockets and 
generic payload.

Files:
  tlm_trans.h (includes the key header files from the other directories )

  tlm_2_interfaces/
      tlm_2_interfaces.h (includes the other header files in this directory )
      tlm_fw_bw_ifs.h    (defines the TLM 2.0 interface API's:
					tlm_fw_nonblocking_transport_if
					tlm_bw_nonblocking_transport_if
					tlm_blocking_transport_if
					tlm_fw_direct_mem_if
					tlm_bw_direct_mem_if
					tlm_transport_dbg_if
			  the enumeration type
			   		tlm_sync_enum
			  and the TLM 2.0 standard interfaces using the API's
			   		tlm_fw_transport_if
					tlm_bw_transport_if )
      tlm_dmi.h          (defines tlm_dmi)

  tlm_generic_payload/
      tlm_generic_payload.h ( includes the other header files in this directory)
      tlm_gp.h              (defines the TLM 2.0 generic payload classes:
      					tlm_generic_payload
					tlm_extension
					tlm_extension_base
					tlm_mm_interface
			     and the enumeration types
			    		tlm_command
					tlm_response_status  ) 
      tlm_array.h           (defines array class used by the extention 
      			     mechanism )
      tlm_endian_conv.h     (defines the implementation for the endianness 
      			     helper functions:
			    		tlm_to_hostendian_generic()
					tlm_from_hostendian_generic()
					tlm_to_hostendian_word()
					tlm_from_hostendian_word()
					tlm_to_hostendian_aligned()
					tlm_from_hostendian_aligned()
					tlm_to_hostendian_single()
					tlm_from_hostendian_single()  )
					  
      tlm_helpers.h         (defines the helper functions to determine the
      			     hostendianness:
					get_host_endianness()
					host_has_little_endianness()
					has_host_endianness()
			     and defines the enumeration type:
			     		tlm_endianness	
      tlm_phase.h           (defines tlm_phase as an extendable enum type)
  
  tlm_sockets/
      tlm_sockets.h          (includes the other header files in this directory)
      tlm_initiator_socket.h (defines the initiator sockets:
      					tlm_initiator_socket_base
					tlm_initiator_socket
					tlm_conv_initiator_socket
      tlm_target_socket.h    (defines the target sockets:
      					tlm_target_socket_base
					tlm_target_socket
					tlm_conv_target_socket


tlm_req_rsp/
------------
This provides support for TLM modeling based on a request/response pair that 
are passed by value. This is  the original TLM 1.0 standard, with the addition 
of a few new methods such as the reference based form of transport.

Files:
  tlm_req_rsp.h  (includes the key header files from the other directories)

  tlm_1_interfaces/
      tlm_core_ifs.h          (defines the TLM 1.0 core interfaces:
					tlm_transport_if
					tlm_blocking_get_if
					tlm_blocking_put_if
					tlm_nonblocking_get_if
					tlm_nonblocking_put_if
					tlm_get_if 
					tlm_put_if 
					tlm_blocking_peek_if
					tlm_nonblocking_peek_if
					tlm_peek_if 
					tlm_blocking_get_peek_if 
					tlm_nonblocking_get_peek_if 
					tlm_get_peek_if              )     
      tlm_fifo_ifs.h	     ( defines the TLM1.0 fifo interfaces:
					tlm_fifo_debug_if			
					tlm_fifo_put_if
					tlm_fifo_get_if
					tlm_fifo_config_size_if )
      tlm_master_slave_ifs.h ( defines the TLM1.0 master slave interfaces:
					tlm_blocking_master_if
					tlm_blocking_slave_if
					tlm_nonblocking_master_if
					tlm_nonblocking_slave_if 
					tlm_master_if 
					tlm_slave_if )
      tlm_tag.h	             ( defines tlm_tag )
  
  tlm_ports/
      tlm_nonblocking_port.h (defines tlm_nonblocking_put_port, 
                                      tlm_nonblocking_get_port and
				      tlm_nonblocking_peek_port )
      tlm_event_finder.h     (defines tlm_event_finder_t )

  tlm_channels/
      tlm_fifo/
          tlm_fifo.h         (defines tlm_fifo, includes the other files )
          tlm_fifo_peek.h    (defines peek and poke interfaces for tlm_fifo )
          tlm_fifo_put_get.h (defines put and get interfaces for tlm_fifo )
          tlm_fifo_resize.h  (defines expand, reduce, bound and unbound
	                      interfaces for tlm_fifo )
          circular_buffer.h  (defines circular buffer used by tlm_fifo )
      tlm_req_rsp_channels/
          tlm_req_rsp_channels.h (defines tlm_req_rsp_channel and
	  			          tlm_transport_channel )
          tlm_put_get_imp.h      (contains implementatins used by the channels)
  
  tlm_adapters/
  	tlm_adapters.h  (defines transport_to_master and tlm_slave_to_transport)
	


