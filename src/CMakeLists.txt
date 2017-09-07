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


###############################################################################
# Configure QuickThreads
###############################################################################

# Set QuickThread sources used by SystemC depending on target platform
if (QT_ARCH STREQUAL "sparc")
  add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sysc/packages/qt/md/sparc.s
                      COMMAND sed 's/_qt_/qt_/' ${CMAKE_CURRENT_SOURCE_DIR}/sysc/packages/qt/md/sparc.s > ${CMAKE_CURRENT_BINARY_DIR}/sysc/packages/qt/md/sparc.s
                      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sysc/packages/qt/md/sparc.s)
endif (QT_ARCH STREQUAL "sparc")

if (APPLE)
  if (N_OSX_ARCHITECTURES EQUAL 1)
    if (QT_ARCH STREQUAL "i386")
      set (CMAKE_ASM_FLAGS "-arch i386")
    elseif (QT_ARCH STREQUAL "x86_64")
      set (CMAKE_ASM_FLAGS "-arch x86_64")
    elseif (QT_ARCH STREQUAL "powerpc_mach")
      set (CMAKE_ASM_FLAGS "-arch ppc")
    endif(QT_ARCH STREQUAL "i386")
  else (N_OSX_ARCHITECTURES EQUAL 1)
    set (CMAKE_ASM_FLAGS "")
  endif (N_OSX_ARCHITECTURES EQUAL 1)
endif (APPLE)


###############################################################################
# Build rules for SystemC library
###############################################################################

add_library (systemc sysc/communication/sc_clock.cpp
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
                     sysc/datatypes/int/sc_int_base.cpp
                     sysc/datatypes/int/sc_int_mask.cpp
                     sysc/datatypes/int/sc_length_param.cpp
                     sysc/datatypes/int/sc_nbexterns.cpp
                     sysc/datatypes/int/sc_nbutils.cpp
                     sysc/datatypes/int/sc_signed.cpp
                     sysc/datatypes/int/sc_uint_base.cpp
                     sysc/datatypes/int/sc_unsigned.cpp
                     sysc/datatypes/misc/sc_concatref.cpp
                     sysc/datatypes/misc/sc_value_base.cpp
                     sysc/kernel/sc_attribute.cpp
                     sysc/kernel/sc_cor_fiber.cpp
                     sysc/kernel/sc_cor_pthread.cpp
                     sysc/kernel/sc_cor_qt.cpp
                     sysc/kernel/sc_cthread_process.cpp
                     sysc/kernel/sc_event.cpp
                     sysc/kernel/sc_except.cpp
                     sysc/kernel/sc_join.cpp
                     sysc/kernel/sc_main.cpp
                     sysc/kernel/sc_main_main.cpp
                     sysc/kernel/sc_method_process.cpp
                     sysc/kernel/sc_module.cpp
                     sysc/kernel/sc_module_name.cpp
                     sysc/kernel/sc_module_registry.cpp
                     sysc/kernel/sc_name_gen.cpp
                     sysc/kernel/sc_object.cpp
                     sysc/kernel/sc_object_manager.cpp
                     sysc/kernel/sc_phase_callback_registry.cpp
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
                     sysc/utils/sc_string.cpp
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
                     sysc/datatypes/int/sc_biguint.h
                     sysc/datatypes/int/sc_int.h
                     sysc/datatypes/int/sc_int_base.h
                     sysc/datatypes/int/sc_int_ids.h
                     sysc/datatypes/int/sc_length_param.h
                     sysc/datatypes/int/sc_nbdefs.h
                     sysc/datatypes/int/sc_nbexterns.h
                     sysc/datatypes/int/sc_nbutils.h
                     sysc/datatypes/int/sc_signed.h
                     sysc/datatypes/int/sc_uint.h
                     sysc/datatypes/int/sc_uint_base.h
                     sysc/datatypes/int/sc_unsigned.h
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
                     sysc/kernel/sc_phase_callback_registry.h
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
                     sysc/packages/boost/bind/apply.hpp
                     sysc/packages/boost/bind/arg.hpp
                     sysc/packages/boost/bind/bind_cc.hpp
                     sysc/packages/boost/bind/bind_mf_cc.hpp
                     sysc/packages/boost/bind/bind_template.hpp
                     sysc/packages/boost/bind/make_adaptable.hpp
                     sysc/packages/boost/bind/mem_fn_cc.hpp
                     sysc/packages/boost/bind/mem_fn_template.hpp
                     sysc/packages/boost/bind/mem_fn_vw.hpp
                     sysc/packages/boost/bind/placeholders.hpp
                     sysc/packages/boost/bind/protect.hpp
                     sysc/packages/boost/bind.hpp
                     sysc/packages/boost/config/compiler/borland.hpp
                     sysc/packages/boost/config/compiler/comeau.hpp
                     sysc/packages/boost/config/compiler/common_edg.hpp
                     sysc/packages/boost/config/compiler/compaq_cxx.hpp
                     sysc/packages/boost/config/compiler/digitalmars.hpp
                     sysc/packages/boost/config/compiler/gcc.hpp
                     sysc/packages/boost/config/compiler/greenhills.hpp
                     sysc/packages/boost/config/compiler/hp_acc.hpp
                     sysc/packages/boost/config/compiler/intel.hpp
                     sysc/packages/boost/config/compiler/kai.hpp
                     sysc/packages/boost/config/compiler/metrowerks.hpp
                     sysc/packages/boost/config/compiler/mpw.hpp
                     sysc/packages/boost/config/compiler/sgi_mipspro.hpp
                     sysc/packages/boost/config/compiler/sunpro_cc.hpp
                     sysc/packages/boost/config/compiler/vacpp.hpp
                     sysc/packages/boost/config/compiler/visualc.hpp
                     sysc/packages/boost/config/platform/aix.hpp
                     sysc/packages/boost/config/platform/amigaos.hpp
                     sysc/packages/boost/config/platform/beos.hpp
                     sysc/packages/boost/config/platform/bsd.hpp
                     sysc/packages/boost/config/platform/cygwin.hpp
                     sysc/packages/boost/config/platform/hpux.hpp
                     sysc/packages/boost/config/platform/irix.hpp
                     sysc/packages/boost/config/platform/linux.hpp
                     sysc/packages/boost/config/platform/macos.hpp
                     sysc/packages/boost/config/platform/solaris.hpp
                     sysc/packages/boost/config/platform/win32.hpp
                     sysc/packages/boost/config/posix_features.hpp
                     sysc/packages/boost/config/select_compiler_config.hpp
                     sysc/packages/boost/config/select_platform_config.hpp
                     sysc/packages/boost/config/select_stdlib_config.hpp
                     sysc/packages/boost/config/stdlib/dinkumware.hpp
                     sysc/packages/boost/config/stdlib/libcomo.hpp
                     sysc/packages/boost/config/stdlib/libstdcpp3.hpp
                     sysc/packages/boost/config/stdlib/modena.hpp
                     sysc/packages/boost/config/stdlib/msl.hpp
                     sysc/packages/boost/config/stdlib/roguewave.hpp
                     sysc/packages/boost/config/stdlib/sgi.hpp
                     sysc/packages/boost/config/stdlib/stlport.hpp
                     sysc/packages/boost/config/stdlib/vacpp.hpp
                     sysc/packages/boost/config/suffix.hpp
                     sysc/packages/boost/config/user.hpp
                     sysc/packages/boost/config.hpp
                     sysc/packages/boost/detail/endian.hpp
                     sysc/packages/boost/detail/workaround.hpp
                     sysc/packages/boost/get_pointer.hpp
                     sysc/packages/boost/mem_fn.hpp
                     sysc/packages/boost/mpl/aux_/adl_barrier.hpp
                     sysc/packages/boost/mpl/aux_/config/adl.hpp
                     sysc/packages/boost/mpl/aux_/config/gcc.hpp
                     sysc/packages/boost/mpl/aux_/config/intel.hpp
                     sysc/packages/boost/mpl/aux_/config/msvc.hpp
                     sysc/packages/boost/mpl/aux_/config/static_constant.hpp
                     sysc/packages/boost/mpl/aux_/config/workaround.hpp
                     sysc/packages/boost/mpl/bool.hpp
                     sysc/packages/boost/mpl/bool_fwd.hpp
                     sysc/packages/boost/mpl/integral_c_tag.hpp
                     sysc/packages/boost/non_type.hpp
                     sysc/packages/boost/ref.hpp
                     sysc/packages/boost/type.hpp
                     sysc/packages/boost/utility/addressof.hpp
                     sysc/packages/boost/utility/enable_if.hpp
                     sysc/packages/boost/utility/string_view.hpp
                     sysc/packages/boost/utility/string_view_fwd.hpp
                     sysc/tracing/sc_trace.h
                     sysc/tracing/sc_trace_file_base.h
                     sysc/tracing/sc_tracing_ids.h
                     sysc/tracing/sc_vcd_trace.h
                     sysc/tracing/sc_wif_trace.h
                     sysc/utils/sc_hash.h
                     sysc/utils/sc_list.h
                     sysc/utils/sc_machine.h
                     sysc/utils/sc_mempool.h
                     sysc/utils/sc_pq.h
                     sysc/utils/sc_pvector.h
                     sysc/utils/sc_report.h
                     sysc/utils/sc_report_handler.h
                     sysc/utils/sc_stop_here.h
                     sysc/utils/sc_string.h
                     sysc/utils/sc_string_view.h
                     sysc/utils/sc_temporary.h
                     sysc/utils/sc_typeindex.h
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
                       $<$<STREQUAL:${QT_ARCH},powerpc_mach>:
                         sysc/packages/qt/md/powerpc_mach.s
                         sysc/packages/qt/md/powerpc.c>
                       $<$<STREQUAL:${QT_ARCH},powerpc_sys5>:
                         sysc/packages/qt/md/powerpc_sys5.s
                         sysc/packages/qt/md/powerpc.c>
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
                     >)

add_library (SystemC::systemc ALIAS systemc)

target_compile_definitions (
  systemc
  PUBLIC
  $<$<BOOL:${DISABLE_VIRTUAL_BIND}>:SC_DISABLE_VIRTUAL_BIND>
  $<$<BOOL:${WIN32}>:WIN32>
  $<$<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>>>:
    SC_WIN_DLL>
  PRIVATE
  SC_BUILD
  SC_INCLUDE_FX
  $<$<BOOL:${DEBUG_SYSTEMC}>:DEBUG_SYSTEMC>
  $<$<BOOL:${DISABLE_ASYNC_UPDATES}>:SC_DISABLE_ASYNC_UPDATES>
  $<$<BOOL:${DISABLE_COPYRIGHT_MESSAGE}>:SC_DISABLE_COPYRIGHT_MESSAGE>
  $<$<BOOL:${DISABLE_VCD_SCOPES}>:SC_DISABLE_VCD_SCOPES>
  $<$<BOOL:${ENABLE_ASSERTIONS}>:SC_ENABLE_ASSERTIONS>
  $<$<BOOL:${ENABLE_EARLY_MAXTIME_CREATION}>:SC_ENABLE_EARLY_MAXTIME_CREATION>
  $<$<BOOL:${ENABLE_IMMEDIATE_SELF_NOTIFICATIONS}>:
    SC_ENABLE_IMMEDIATE_SELF_NOTIFICATIONS>
  $<$<BOOL:${ENABLE_PHASE_CALLBACKS}>:SC_ENABLE_SIMULATION_PHASE_CALLBACKS>
  $<$<BOOL:${ENABLE_PHASE_CALLBACKS_TRACING}>:
    SC_ENABLE_SIMULATION_PHASE_CALLBACKS_TRACING>
  $<$<BOOL:${ENABLE_PTHREADS}>:SC_USE_PTHREADS>
  $<$<BOOL:${OVERRIDE_DEFAULT_STACK_SIZE}>:
    SC_OVERRIDE_DEFAULT_STACK_SIZE=${OVERRIDE_DEFAULT_STACK_SIZE}>
  $<$<AND:$<BOOL:${WIN32}>,$<BOOL:${MSVC}>>:_LIB>)

target_compile_options(
  systemc
  PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/vmg /MP>
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
    -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable>
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /wd4244 /wd4267 /wd4996>)

target_include_directories(systemc
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if (APPLE)
  # It's OK that _sc_main is an undefined symbol.
  set_target_properties (systemc PROPERTIES LINK_FLAGS -Wl,-U,_sc_main)
endif (APPLE)

target_link_libraries (systemc PUBLIC $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>)

set_target_properties (systemc
                       PROPERTIES
                       LINKER_LANGUAGE CXX
                       VERSION ${SystemCLanguage_VERSION}
                       SOVERSION ${SystemCLanguage_SOVERSION}
                       INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR})


###############################################################################
# Install rules for SystemC library
###############################################################################

install (TARGETS systemc EXPORT  SystemCLanguageTargets
                         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
                         COMPONENT lib)

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
