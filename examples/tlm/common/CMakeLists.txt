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
# examples/tlm/common/CMakeLists.txt --
# Build the shared TLM example helper sources ONCE into an OBJECT library so
# that they are not recompiled by every example target.
#
# NOTE: These sources are compiled WITHOUT USING_EXTENSION_OPTIONAL.  The
# at_extension_optional example needs traffic_generator.cpp / at_target_*.cpp
# compiled WITH that macro, so it keeps its own private copies and does not
# link this library.
#
# Only the helper sources actually consumed by the example executables are
# listed here (the header-only / template initiator variants such as
# at_initiator_*.cpp are pulled in directly by the examples that need them).
#
###############################################################################

add_library (tlm_common OBJECT
             include/at_target_1_phase.h
             src/at_target_1_phase.cpp
             include/at_target_2_phase.h
             src/at_target_2_phase.cpp
             include/at_target_4_phase.h
             src/at_target_4_phase.cpp
             include/dmi_memory.h
             src/dmi_memory.cpp
             include/extension_initiator_id.h
             src/extension_initiator_id.cpp
             include/lt_dmi_initiator.h
             src/lt_dmi_initiator.cpp
             include/lt_dmi_target.h
             src/lt_dmi_target.cpp
             include/lt_initiator.h
             src/lt_initiator.cpp
             include/lt_synch_target.h
             src/lt_synch_target.cpp
             include/lt_target.h
             src/lt_target.cpp
             include/lt_td_initiator.h
             src/lt_td_initiator.cpp
             include/memory.h
             src/memory.cpp
             include/reporting.h
             src/report.cpp
             include/select_initiator.h
             src/select_initiator.cpp
             include/traffic_generator.h
             src/traffic_generator.cpp)

target_include_directories (tlm_common PUBLIC include)
target_link_libraries (tlm_common PUBLIC SystemC::systemc)
