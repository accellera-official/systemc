# Microsoft Developer Studio Project File - Name="SystemC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SystemC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SystemC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SystemC.mak" CFG="SystemC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SystemC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SystemC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SystemC - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "../../src" /D "SC_INCLUDE_FX" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SystemC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "../../src" /D "SC_INCLUDE_FX" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SystemC - Win32 Release"
# Name "SystemC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_attribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bv_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_clock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\misc\sc_concatref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_fiber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_pthread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_qt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_event.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_event_finder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_event_queue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_export.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxcast_switch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxdefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxnum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxnum_observer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxtype_params.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxval.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxval_observer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_hash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int32_mask.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int64_io.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int64_mask.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_join.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_lambda.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_length_param.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_list.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_logic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_lv_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_main_main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_mempool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module_name.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module_registry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_name_gen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbdefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbexterns.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbutils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_object.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_object_manager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_port.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_pq.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_prim_channel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_int.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_report.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_report_handler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_sensitive.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_ports.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_resolved.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_resolved_ports.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_signed.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_simcontext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_stop_here.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_time.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_trace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_uint_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_unsigned.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_utils_ids.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\misc\sc_value_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_vcd_trace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_ver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_wait.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_wait_cthread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_wif_trace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_mant.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_pow10.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_rep.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\assert.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\bind.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\checked_delete.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\config.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\current_function.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\fx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\mem_fn.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\ref.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_attribute.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_bigint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_biguint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bit.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bit_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bit_proxies.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_boost.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bv.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_bv_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_clock.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_clock_ports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cmnhdr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_communication_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\misc\sc_concatref.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_constants.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_context.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_fiber.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_pthread.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_cor_qt.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_dynamic_processes.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_event.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_event_finder.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_event_queue.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_except.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_exception.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_export.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_externs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_fifo.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_fifo_ifs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_fifo_ports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fix.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fixed.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fx_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxcast_switch.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxnum.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxnum_observer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxtype_params.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxval.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_fxval_observer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_int_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_iostream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_join.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_kernel_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_lambda.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_lambda_defs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_lambda_exps.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_lambda_friends.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_length_param.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_list.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_logic.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_lv.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_lv_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_macros.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_macros_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_mempool.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module_name.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_module_registry.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_mutex_if.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_name_gen.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbexterns.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_nbutils.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_object.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_object_manager.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_port.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_pq.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_prim_channel.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_b.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_host.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_process_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\bit\sc_proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_report.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_report_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_runnable.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_runnable_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_semaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_semaphore_if.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_sensitive.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_ifs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_ports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_resolved.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_resolved_ports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_rv.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\communication\sc_signal_rv_ports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_signed.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_simcontext.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_simcontext_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_stop_here.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_string.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_temporary.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_time.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_trace.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_ufix.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\sc_ufixed.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_uint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_uint_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\int\sc_unsigned.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_utils_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\misc\sc_value_base.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_vcd_trace.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\utils\sc_vector.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_ver.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_wait.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\kernel\sc_wait_cthread.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\tracing\sc_wif_trace.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_ieee.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_mant.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_other_defs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_params.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_pow10.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_rep.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_string.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\datatypes\fx\scfx_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\shared_ptr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\throw_exception.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\type.hpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sysc\packages\boost\weak_ptr.hpp
# End Source File
# End Group
# End Target
# End Project
