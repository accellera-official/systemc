###############################################################################
#
# Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
# more contributor license agreements.  See the NOTICE file distributed
# with this work for additional information regarding copyright ownership.
# Accellera licenses this file to you under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.  See the License for the specific language governing
# permissions and limitations under the License.
#
###############################################################################

###############################################################################
#
# src/CMakeLists.txt --
# CMake script to configure the SystemC sources and to generate native
# Makefiles and project workspaces for your compiler environment.
#
# Original Author: Torsten Maehne, Université Pierre et Marie Curie, Paris,
#                  2013-06-11
#
###############################################################################

###############################################################################
#
# MODIFICATION LOG - modifiers, enter your name, affiliation, date and
# changes you are making here.
#
#     Name, Affiliation, Date:
# Description of Modification:
#
###############################################################################

cmake_minimum_required(VERSION 3.8) # cxx_std_17

###############################################################################
# Configure QuickThreads
###############################################################################

# Set QuickThread sources used by SystemC depending on target platform
if (QT_ARCH STREQUAL "sparc")
  add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sysc/packages/qt/md/sparc.s
                      COMMAND sed 's/_qt_/qt_/' ${CMAKE_CURRENT_SOURCE_DIR}/sysc/packages/qt/md/sparc.s > ${CMAKE_CURRENT_BINARY_DIR}/sysc/packages/qt/md/sparc.s
                      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sysc/packages/qt/md/sparc.s)
endif (QT_ARCH STREQUAL "sparc")

###############################################################################
# Build rules for SystemC library
###############################################################################

function(add_systemc_library libName scBuildDefine)

    add_library(${libName} ${ARGN})

    target_compile_features(
        ${libName}
        PUBLIC
        cxx_std_17)

    target_compile_definitions (
        ${libName}
        PUBLIC
        $<$<BOOL:${WIN32}>:WIN32>
        $<$<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>>>:
        SC_WIN_DLL>
        $<$<BOOL:${ALLOW_DEPRECATED_IEEE_API}>:SC_ALLOW_DEPRECATED_IEEE_API>
        PRIVATE
        ${scBuildDefine}
        SC_INCLUDE_FX
        $<$<BOOL:${DEBUG_SYSTEMC}>:DEBUG_SYSTEMC>
        $<$<BOOL:${DISABLE_ASYNC_UPDATES}>:SC_DISABLE_ASYNC_UPDATES>
        $<$<BOOL:${DISABLE_COPYRIGHT_MESSAGE}>:SC_DISABLE_COPYRIGHT_MESSAGE>
        $<$<BOOL:${DISABLE_VCD_SCOPES}>:SC_DISABLE_VCD_SCOPES>
        $<$<BOOL:${ENABLE_ASSERTIONS}>:SC_ENABLE_ASSERTIONS>
        $<$<BOOL:${ENABLE_PTHREADS}>:SC_USE_PTHREADS>
        $<$<BOOL:${OVERRIDE_DEFAULT_STACK_SIZE}>:
        SC_OVERRIDE_DEFAULT_STACK_SIZE=${OVERRIDE_DEFAULT_STACK_SIZE}>
        $<$<AND:$<BOOL:${WIN32}>,$<BOOL:${MSVC}>>:_LIB>)

    target_compile_options(
        ${libName}
        PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/vmg /MP>
        PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
          -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable>
        $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>>:
          -xassembler-with-cpp>
        $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>>:
          -Qunused-arguments>
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /wd4244 /wd4267 /wd4996>)

    target_include_directories(
        ${libName}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

    if (APPLE)
        # It's OK that _sc_main, __sanitizer_start_switch_fiber, and
        # __sanitizer_finish_switch_fiber are undefined symbols.
        set_target_properties (${libName} PROPERTIES LINK_FLAGS
        -Wl,-U,_sc_main,-U,___sanitizer_start_switch_fiber,-U,___sanitizer_finish_switch_fiber)
    endif(APPLE)

    target_link_libraries(
        ${libName}
        PUBLIC
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>)

    set_target_properties(
        ${libName}
        PROPERTIES
        LINKER_LANGUAGE CXX
        VERSION ${SystemCLanguage_VERSION}
        SOVERSION ${SystemCLanguage_SOVERSION}
        INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR})

endfunction(add_systemc_library)

set(SYSTEMC_CORE_SRC
        systemc sysc/communication/sc_clock.cpp
        sysc/communication/sc_event_finder.cpp
        sysc/communication/sc_event_queue.cpp
        sysc/communication/sc_export.cpp
        sysc/communication/sc_interface.cpp
        sysc/communication/sc_mutex.cpp
        sysc/communication/sc_port.cpp
        sysc/communication/sc_prim_channel.cpp
        sysc/communication/sc_semaphore.cpp
        sysc/communication/sc_signal.cpp
        sysc/communication/sc_signal_ports.cpp
        sysc/communication/sc_signal_resolved.cpp
        sysc/communication/sc_signal_resolved_ports.cpp
        sysc/datatypes/bit/sc_bit.cpp
        sysc/datatypes/bit/sc_bv_base.cpp
        sysc/datatypes/bit/sc_logic.cpp
        sysc/datatypes/bit/sc_lv_base.cpp
        sysc/datatypes/fx/sc_fxcast_switch.cpp
        sysc/datatypes/fx/sc_fxdefs.cpp
        sysc/datatypes/fx/sc_fxnum.cpp
        sysc/datatypes/fx/sc_fxnum_observer.cpp
        sysc/datatypes/fx/sc_fxtype_params.cpp
        sysc/datatypes/fx/sc_fxval.cpp
        sysc/datatypes/fx/sc_fxval_observer.cpp
        sysc/datatypes/fx/scfx_mant.cpp
        sysc/datatypes/fx/scfx_pow10.cpp
        sysc/datatypes/fx/scfx_rep.cpp
        sysc/datatypes/fx/scfx_utils.cpp
        sysc/datatypes/int/sc_int64_io.cpp
        sysc/datatypes/int/sc_int64_mask.cpp
        sysc/datatypes/int/sc_int_base.cpp
        sysc/datatypes/int/sc_length_param.cpp
        sysc/datatypes/int/sc_nbutils.cpp
        sysc/datatypes/int/sc_signed.cpp
        sysc/datatypes/int/sc_uint_base.cpp
        sysc/datatypes/int/sc_unsigned.cpp
        sysc/datatypes/misc/sc_value_base.cpp
        sysc/kernel/sc_attribute.cpp
        sysc/kernel/sc_cor_fiber.cpp
        sysc/kernel/sc_cor_pthread.cpp
        sysc/kernel/sc_cor_qt.cpp
        sysc/kernel/sc_cthread_process.cpp
        sysc/kernel/sc_event.cpp
        sysc/kernel/sc_except.cpp
        sysc/kernel/sc_join.cpp
        sysc/kernel/sc_method_process.cpp
        sysc/kernel/sc_module.cpp
        sysc/kernel/sc_module_name.cpp
        sysc/kernel/sc_initializer_function.h
        sysc/kernel/sc_module_registry.cpp
        sysc/kernel/sc_name_gen.cpp
        sysc/kernel/sc_object.cpp
        sysc/kernel/sc_object_manager.cpp
        sysc/kernel/sc_stage_callback_registry.cpp
        sysc/kernel/sc_process.cpp
        sysc/kernel/sc_reset.cpp
        sysc/kernel/sc_sensitive.cpp
        sysc/kernel/sc_simcontext.cpp
        sysc/kernel/sc_spawn_options.cpp
        sysc/kernel/sc_thread_process.cpp
        sysc/kernel/sc_time.cpp
        sysc/kernel/sc_ver.cpp
        sysc/kernel/sc_wait.cpp
        sysc/kernel/sc_wait_cthread.cpp
        sysc/tracing/sc_trace.cpp
        sysc/tracing/sc_trace_file_base.cpp
        sysc/tracing/sc_vcd_trace.cpp
        sysc/tracing/sc_wif_trace.cpp
        sysc/utils/sc_hash.cpp
        sysc/utils/sc_list.cpp
        sysc/utils/sc_mempool.cpp
        sysc/utils/sc_pq.cpp
        sysc/utils/sc_report.cpp
        sysc/utils/sc_report_handler.cpp
        sysc/utils/sc_stop_here.cpp
        sysc/utils/sc_utils_ids.cpp
        sysc/utils/sc_vector.cpp
        # TLM sources
        tlm_core/tlm_2/tlm_generic_payload/tlm_gp.cpp
        tlm_core/tlm_2/tlm_generic_payload/tlm_phase.cpp
        tlm_core/tlm_2/tlm_quantum/tlm_global_quantum.cpp
        tlm_utils/convenience_socket_bases.cpp
        tlm_utils/instance_specific_extensions.cpp
        # SystemC headers
        sysc/communication/sc_buffer.h
        sysc/communication/sc_clock.h
        sysc/communication/sc_clock_ports.h
        sysc/communication/sc_communication_ids.h
        sysc/communication/sc_event_finder.h
        sysc/communication/sc_event_queue.h
        sysc/communication/sc_export.h
        sysc/communication/sc_fifo.h
        sysc/communication/sc_fifo_ifs.h
        sysc/communication/sc_fifo_ports.h
        sysc/communication/sc_host_mutex.h
        sysc/communication/sc_host_semaphore.h
        sysc/communication/sc_interface.h
        sysc/communication/sc_mutex.h
        sysc/communication/sc_mutex_if.h
        sysc/communication/sc_port.h
        sysc/communication/sc_prim_channel.h
        sysc/communication/sc_semaphore.h
        sysc/communication/sc_semaphore_if.h
        sysc/communication/sc_signal.h
        sysc/communication/sc_signal_ifs.h
        sysc/communication/sc_signal_ports.h
        sysc/communication/sc_signal_resolved.h
        sysc/communication/sc_signal_resolved_ports.h
        sysc/communication/sc_signal_rv.h
        sysc/communication/sc_signal_rv_ports.h
        sysc/communication/sc_writer_policy.h
        sysc/datatypes/bit/sc_bit.h
        sysc/datatypes/bit/sc_bit_ids.h
        sysc/datatypes/bit/sc_bit_proxies.h
        sysc/datatypes/bit/sc_bv.h
        sysc/datatypes/bit/sc_bv_base.h
        sysc/datatypes/bit/sc_logic.h
        sysc/datatypes/bit/sc_lv.h
        sysc/datatypes/bit/sc_lv_base.h
        sysc/datatypes/bit/sc_proxy.h
        sysc/datatypes/fx/fx.h
        sysc/datatypes/fx/sc_context.h
        sysc/datatypes/fx/sc_fix.h
        sysc/datatypes/fx/sc_fixed.h
        sysc/datatypes/fx/sc_fx_ids.h
        sysc/datatypes/fx/sc_fxcast_switch.h
        sysc/datatypes/fx/sc_fxdefs.h
        sysc/datatypes/fx/sc_fxnum.h
        sysc/datatypes/fx/sc_fxnum_observer.h
        sysc/datatypes/fx/sc_fxtype_params.h
        sysc/datatypes/fx/sc_fxval.h
        sysc/datatypes/fx/sc_fxval_observer.h
        sysc/datatypes/fx/sc_ufix.h
        sysc/datatypes/fx/sc_ufixed.h
        sysc/datatypes/fx/scfx_ieee.h
        sysc/datatypes/fx/scfx_mant.h
        sysc/datatypes/fx/scfx_other_defs.h
        sysc/datatypes/fx/scfx_params.h
        sysc/datatypes/fx/scfx_pow10.h
        sysc/datatypes/fx/scfx_rep.h
        sysc/datatypes/fx/scfx_string.h
        sysc/datatypes/fx/scfx_utils.h
        sysc/datatypes/int/sc_bigint.h
        sysc/datatypes/int/sc_bigint_inlines.h
        sysc/datatypes/int/sc_big_ops.h
        sysc/datatypes/int/sc_biguint.h
        sysc/datatypes/int/sc_biguint_inlines.h
        sysc/datatypes/int/sc_int.h
        sysc/datatypes/int/sc_int_base.h
        sysc/datatypes/int/sc_int_ids.h
        sysc/datatypes/int/sc_int_inlines.h
        sysc/datatypes/int/sc_length_param.h
        sysc/datatypes/int/sc_nbdefs.h
        sysc/datatypes/int/sc_nbutils.h
        sysc/datatypes/int/sc_signed.h
        sysc/datatypes/int/sc_signed_friends.h
        sysc/datatypes/int/sc_signed_inlines.h
        sysc/datatypes/int/sc_signed_ops.h
        sysc/datatypes/int/sc_uint.h
        sysc/datatypes/int/sc_uint_base.h
        sysc/datatypes/int/sc_uint_inlines.h
        sysc/datatypes/int/sc_unsigned.h
        sysc/datatypes/int/sc_unsigned_friends.h
        sysc/datatypes/int/sc_unsigned_inlines.h
        sysc/datatypes/int/sc_vector_utils.h
        sysc/datatypes/misc/sc_concatref.h
        sysc/datatypes/misc/sc_value_base.h
        sysc/kernel/sc_attribute.h
        sysc/kernel/sc_cmnhdr.h
        sysc/kernel/sc_constants.h
        sysc/kernel/sc_cor.h
        sysc/kernel/sc_cor_fiber.h
        sysc/kernel/sc_cor_pthread.h
        sysc/kernel/sc_cor_qt.h
        sysc/kernel/sc_cthread_process.h
        sysc/kernel/sc_dynamic_processes.h
        sysc/kernel/sc_event.h
        sysc/kernel/sc_except.h
        sysc/kernel/sc_externs.h
        sysc/kernel/sc_join.h
        sysc/kernel/sc_kernel_ids.h
        sysc/kernel/sc_macros.h
        sysc/kernel/sc_method_process.h
        sysc/kernel/sc_module.h
        sysc/kernel/sc_module_name.h
        sysc/kernel/sc_module_registry.h
        sysc/kernel/sc_name_gen.h
        sysc/kernel/sc_object.h
        sysc/kernel/sc_object_int.h
        sysc/kernel/sc_object_manager.h
        sysc/kernel/sc_stage_callback_registry.h
        sysc/kernel/sc_process.h
        sysc/kernel/sc_process_handle.h
        sysc/kernel/sc_reset.h
        sysc/kernel/sc_runnable.h
        sysc/kernel/sc_runnable_int.h
        sysc/kernel/sc_sensitive.h
        sysc/kernel/sc_simcontext.h
        sysc/kernel/sc_simcontext_int.h
        sysc/kernel/sc_spawn.h
        sysc/kernel/sc_spawn_options.h
        sysc/kernel/sc_status.h
        sysc/kernel/sc_thread_process.h
        sysc/kernel/sc_time.h
        sysc/kernel/sc_ver.h
        sysc/kernel/sc_wait.h
        sysc/kernel/sc_wait_cthread.h
        sysc/tracing/sc_trace.h
        sysc/tracing/sc_trace_file_base.h
        sysc/tracing/sc_tracing_ids.h
        sysc/tracing/sc_vcd_trace.h
        sysc/tracing/sc_wif_trace.h
        sysc/utils/sc_hash.h
        sysc/utils/sc_iostream.h
        sysc/utils/sc_list.h
        sysc/utils/sc_machine.h
        sysc/utils/sc_mempool.h
        sysc/utils/sc_pq.h
        sysc/utils/sc_ptr_flag.h
        sysc/utils/sc_pvector.h
        sysc/utils/sc_report.h
        sysc/utils/sc_report_handler.h
        sysc/utils/sc_stop_here.h
        sysc/utils/sc_string.h
        sysc/utils/sc_string_view.h
        sysc/utils/sc_temporary.h
        sysc/utils/sc_utils_ids.h
        sysc/utils/sc_vector.h
        systemc
        systemc.h
        # TLM headers
        tlm
        tlm.h
        tlm_core/tlm_1/tlm_analysis/tlm_analysis.h
        tlm_core/tlm_1/tlm_analysis/tlm_analysis_fifo.h
        tlm_core/tlm_1/tlm_analysis/tlm_analysis_if.h
        tlm_core/tlm_1/tlm_analysis/tlm_analysis_port.h
        tlm_core/tlm_1/tlm_analysis/tlm_analysis_triple.h
        tlm_core/tlm_1/tlm_analysis/tlm_write_if.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_core_ifs.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_fifo_ifs.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_master_slave_ifs.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_tag.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_adapters/tlm_adapters.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/circular_buffer.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/tlm_fifo.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/tlm_fifo_peek.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/tlm_fifo_put_get.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_fifo/tlm_fifo_resize.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_req_rsp_channels/tlm_put_get_imp.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_req_rsp_channels/tlm_req_rsp_channels.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_ports/tlm_event_finder.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_ports/tlm_nonblocking_port.h
        tlm_core/tlm_1/tlm_req_rsp/tlm_req_rsp.h
        tlm_core/tlm_2/tlm_2_interfaces/tlm_2_interfaces.h
        tlm_core/tlm_2/tlm_2_interfaces/tlm_dmi.h
        tlm_core/tlm_2/tlm_2_interfaces/tlm_fw_bw_ifs.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_array.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_endian_conv.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_generic_payload.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_helpers.h
        tlm_core/tlm_2/tlm_generic_payload/tlm_phase.h
        tlm_core/tlm_2/tlm_quantum/tlm_global_quantum.h
        tlm_core/tlm_2/tlm_quantum/tlm_quantum.h
        tlm_core/tlm_2/tlm_sockets/tlm_base_socket_if.h
        tlm_core/tlm_2/tlm_sockets/tlm_initiator_socket.h
        tlm_core/tlm_2/tlm_sockets/tlm_sockets.h
        tlm_core/tlm_2/tlm_sockets/tlm_target_socket.h
        tlm_core/tlm_2/tlm_version.h
        tlm_utils/convenience_socket_bases.h
        tlm_utils/instance_specific_extensions.h
        tlm_utils/instance_specific_extensions_int.h
        tlm_utils/multi_passthrough_initiator_socket.h
        tlm_utils/multi_passthrough_target_socket.h
        tlm_utils/multi_socket_bases.h
        tlm_utils/passthrough_target_socket.h
        tlm_utils/peq_with_cb_and_phase.h
        tlm_utils/peq_with_get.h
        tlm_utils/simple_initiator_socket.h
        tlm_utils/simple_target_socket.h
        tlm_utils/tlm_quantumkeeper.h
        # QuickThreads
        $<$<BOOL:${QT_ARCH}>:
        sysc/packages/qt/qt.c
        $<$<STREQUAL:${QT_ARCH},sparc>:
        ${CMAKE_CURRENT_BINARY_DIR}/sysc/packages/qt/md/sparc.s>
        $<$<STREQUAL:${QT_ARCH},hppa>:sysc/packages/qt/md/hppa.s>
        $<$<STREQUAL:${QT_ARCH},x86_64>:sysc/packages/qt/md/iX86_64.s>
        $<$<STREQUAL:${QT_ARCH},i386>:sysc/packages/qt/md/i386.s>
        $<$<STREQUAL:${QT_ARCH},aarch64>:sysc/packages/qt/md/aarch64.s>
        $<$<STREQUAL:${QT_ARCH},multi>:sysc/packages/qt/md/multi.s>
        sysc/packages/qt/md/aarch64.h
        sysc/packages/qt/md/axp.h
        sysc/packages/qt/md/hppa.h
        sysc/packages/qt/md/i386.h
        sysc/packages/qt/md/iX86_64.h
        sysc/packages/qt/md/ksr1.h
        sysc/packages/qt/md/m88k.h
        sysc/packages/qt/md/mips.h
        sysc/packages/qt/md/powerpc_mach.h
        sysc/packages/qt/md/powerpc_sys5.h
        sysc/packages/qt/md/sparc.h
        sysc/packages/qt/md/vax.h
        sysc/packages/qt/qt.h
        sysc/packages/qt/qtmd.h
        >
        )

set(SYSTEMC_SC_MAIN_SRC
        systemc.h
        tlm.h
        sysc/kernel/sc_externs.h
        sysc/kernel/sc_main.cpp
        sysc/kernel/sc_main_main.cpp
        )

if (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

# Windows DLLs can't contain unresolved symbols, so we need to spilt library
# into two:
#  - ${SYSTEMC_DLL_TARGET}.dll is a shared library without unresolved symbols
#  - systemc.lib contains main() and unresolved sc_main()
# Then we merge interface library ${SYSTEMC_DLL_TARGET}.lib into systemc.lib

    set(SYSTEMC_DLL_TARGET systemc-${SystemCLanguage_VERSION})

    add_systemc_library(${SYSTEMC_DLL_TARGET} SC_BUILD SHARED ${SYSTEMC_CORE_SRC} )

    add_systemc_library(systemc SC_BUILD_OFF STATIC ${SYSTEMC_SC_MAIN_SRC} )

    if (MSVC) # lib.exe should be available from MSVC command promt

        add_custom_command(
                TARGET systemc
                POST_BUILD
                COMMAND lib.exe /OUT:$<TARGET_FILE:systemc> $<TARGET_FILE:systemc> $<TARGET_LINKER_FILE:${SYSTEMC_DLL_TARGET}>
                COMMENT "Combining SystemC libs..."
        )

    else(MSVC)

        add_custom_command(
                TARGET systemc
                POST_BUILD
                COMMAND ar x $<TARGET_FILE:systemc>
                COMMAND ar x $<TARGET_LINKER_FILE:${SYSTEMC_DLL_TARGET}>
                COMMAND del *.a
                COMMAND ar qc $<TARGET_FILE:systemc>  *.o *.obj
                COMMAND del *.o *.obj
                COMMENT "Combining SystemC libs..."
        )

    endif(MSVC)

    set(SYSTEMC_TARGETS systemc ${SYSTEMC_DLL_TARGET})

else(BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

    add_systemc_library(systemc
            SC_BUILD
            ${SYSTEMC_CORE_SRC}
            ${SYSTEMC_SC_MAIN_SRC}
            )

    set(SYSTEMC_TARGETS systemc)

endif(BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

add_library(SystemC::systemc ALIAS systemc)

install(TARGETS ${SYSTEMC_TARGETS} EXPORT SystemCLanguageTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT lib)

###############################################################################
# Install rules for SystemC library
###############################################################################

# Export targets to build tree root
export (TARGETS ${SYSTEMC_TARGETS}
        NAMESPACE SystemC::
        FILE ${CMAKE_BINARY_DIR}/SystemCLanguageTargets.cmake)

# Install the SystemC and TLM headers
install (FILES systemc tlm
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev)
install (DIRECTORY ./
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev
         FILES_MATCHING PATTERN "*.h")
install (DIRECTORY ./
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev
         FILES_MATCHING PATTERN "*.hpp")
