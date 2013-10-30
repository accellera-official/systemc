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
# examples/sysc/fir/CMakeLists.txt --
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


set (fir_COMMON_HEADERS display.h
                        fir.h
                        fir_const.h
                        fir_data.h
                        fir_fsm.h
                        fir_top.h
                        stimulus.h)

set (fir_COMMON_SOURCES stimulus.cpp
                        display.cpp)

set (fir_SOURCES fir.cpp
                 main.cpp
                 ${fir_COMMON_SOURCES})

set (fir_rtl_SOURCES fir_fsm.cpp
                     fir_data.cpp
                     main_rtl.cpp
                     ${fir_COMMON_SOURCES})

add_executable (fir ${fir_SOURCES} ${fir_COMMON_HEADERS})
target_link_libraries (fir systemc)
add_test (fir fir)

add_executable (fir_rtl ${fir_rtl_SOURCES} ${fir_COMMON_HEADERS})
target_link_libraries (fir_rtl systemc)
add_test (fir_rtl fir_rtl)
