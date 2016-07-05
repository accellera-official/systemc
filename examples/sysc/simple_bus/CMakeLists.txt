###############################################################################
#
# The following code is derived, directly or indirectly, from the SystemC
# source code Copyright (c) 1996-2011 by all Contributors.
# All Rights reserved.
#
# The contents of this file are subject to the restrictions and limitations
# set forth in the SystemC Open Source License Version 3.0 (the "License");
# You may not use this file except in compliance with such restrictions and
# limitations. You may obtain instructions on how to receive a copy of the
# License at http://www.systemc.org/. Software distributed by Contributors
# under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
# ANY KIND, either express or implied. See the License for the specific
# language governing rights and limitations under the License.
#
###############################################################################

###############################################################################
#
# examples/sysc/simple_bus/CMakeLists.txt --
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


set (simple_bus_HEADERS simple_bus.h
                        simple_bus_arbiter.h
                        simple_bus_arbiter_if.h
                        simple_bus_blocking_if.h
                        simple_bus_direct_if.h
                        simple_bus_fast_mem.h
                        simple_bus_master_blocking.h
                        simple_bus_master_direct.h
                        simple_bus_master_non_blocking.h
                        simple_bus_non_blocking_if.h
                        simple_bus_request.h
                        simple_bus_slave_if.h
                        simple_bus_slow_mem.h
                        simple_bus_test.h
                        simple_bus_types.h)

set (simple_bus_SOURCES simple_bus.cpp
                        simple_bus_arbiter.cpp
                        simple_bus_main.cpp
                        simple_bus_master_blocking.cpp
                        simple_bus_master_direct.cpp
                        simple_bus_master_non_blocking.cpp
                        simple_bus_types.cpp
                        simple_bus_tools.cpp)

add_executable (simple_bus ${simple_bus_SOURCES} ${simple_bus_HEADERS})
target_link_libraries (simple_bus ${SystemC_LIBRARIES})
add_test (simple_bus simple_bus)
