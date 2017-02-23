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
# examples/tlm/lt_extension_mandatory/CMakeLists.txt --
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


add_executable (lt_extension_mandatory src/lt_extension_mandatory.cpp
                                       include/lt_extension_mandatory_top.h
                                       src/lt_extension_mandatory_top.cpp
                                       include/lt_initiator_extension_mandatory.h
                                       src/lt_initiator_extension_mandatory.cpp
                                       include/lt_target_extension_mandatory.h
                                       src/lt_target_extension_mandatory.cpp
                                       ../common/include/extension_initiator_id.h
                                       ../common/src/extension_initiator_id.cpp
                                       ../common/include/reporting.h
                                       ../common/src/report.cpp)
target_include_directories (lt_extension_mandatory PRIVATE include ../common/include)
target_link_libraries (lt_extension_mandatory SystemC::systemc)
configure_and_add_test (lt_extension_mandatory)
