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
# examples/tlm/lt_mixed_endian/CMakeLists.txt --
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


include_directories (include)
add_executable (lt_mixed_endian src/lt.cpp
                                src/lt_top.cpp
                                src/initiator_top.cpp
                                src/me_traffic_generator.cpp
                                ${COMMON_SRC_DIR}/lt_initiator.cpp
                                ${COMMON_SRC_DIR}/at_target_1_phase.cpp
                                ${COMMON_SRC_DIR}/at_target_4_phase.cpp
                                ${COMMON_SRC_DIR}/memory.cpp
                                ${COMMON_SRC_DIR}/report.cpp)
target_link_libraries (lt_mixed_endian ${SystemC_LIBRARIES})
configure_and_add_test (lt_mixed_endian results/input.txt)
