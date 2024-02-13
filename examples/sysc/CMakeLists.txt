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
# examples/sysc/CMakeLists.txt --
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


set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)
set(TEST_FOLDER "examples/sysc" CACHE INTERNAL "" FORCE)

add_subdirectory (2.1/dpipe)
add_subdirectory (2.1/forkjoin)
add_subdirectory (2.1/reset_signal_is)
add_subdirectory (2.1/sc_export)
add_subdirectory (2.1/sc_report)
add_subdirectory (2.1/scx_barrier)
add_subdirectory (2.1/scx_mutex_w_policy)
#add_subdirectory (2.1/specialized_signals) # needs work!!!!
add_subdirectory (2.3/sc_rvd)
add_subdirectory (2.3/sc_ttd)
add_subdirectory (2.3/simple_async)
add_subdirectory (2.4/in_class_initialization)
add_subdirectory (fft/fft_flpt)
add_subdirectory (fft/fft_fxpt)
add_subdirectory (fir)
add_subdirectory (pipe)
add_subdirectory (pkt_switch)
add_subdirectory (risc_cpu)
add_subdirectory (rsa)
add_subdirectory (simple_bus)
add_subdirectory (simple_fifo)
add_subdirectory (simple_perf)
add_subdirectory (async_suspend)

