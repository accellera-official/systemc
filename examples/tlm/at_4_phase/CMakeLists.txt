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
# examples/tlm/at_4_phase/CMakeLists.txt --
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


add_executable (at_4_phase src/at_4_phase.cpp
                           include/at_4_phase_top.h
                           src/at_4_phase_top.cpp
                           include/initiator_top.h
                           src/initiator_top.cpp
                           ../common/include/traffic_generator.h
                           ../common/src/traffic_generator.cpp
                           ../common/include/memory.h
                           ../common/src/memory.cpp
                           ../common/include/reporting.h
                           ../common/src/report.cpp
                           ../common/include/at_target_4_phase.h
                           ../common/src/at_target_4_phase.cpp
                           ../common/include/select_initiator.h
                           ../common/src/select_initiator.cpp)
target_include_directories (at_4_phase PRIVATE include ../common/include)
target_link_libraries (at_4_phase SystemC::systemc)
configure_and_add_test (at_4_phase)
