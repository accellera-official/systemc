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
# examples/tlm/CMakeLists.txt --
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
# Add the TLM examples.
###############################################################################

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)
set(TEST_FOLDER "examples/tlm" CACHE INTERNAL "" FORCE)

add_subdirectory (at_1_phase)
add_subdirectory (at_2_phase)
add_subdirectory (at_4_phase)
add_subdirectory (at_extension_optional)
add_subdirectory (at_mixed_targets)
add_subdirectory (at_ooo)
add_subdirectory (lt)
add_subdirectory (lt_dmi)
add_subdirectory (lt_extension_mandatory)
add_subdirectory (lt_mixed_endian)
add_subdirectory (lt_temporal_decouple)
