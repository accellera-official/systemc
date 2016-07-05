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
# examples/sysc/pkt_switch/CMakeLists.txt --
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


set (pkt_switch_HEADERS fifo.h
                        pkt.h
                        receiver.h
                        sender.h
                        switch.h
                        switch_clk.h
                        switch_reg.h)

set (pkt_switch_SOURCES main.cpp
                        receiver.cpp
                        fifo.cpp
                        sender.cpp
                        switch.cpp
                        switch_clk.cpp)

add_executable (pkt_switch ${pkt_switch_SOURCES} ${pkt_switch_HEADERS})
target_link_libraries (pkt_switch ${SystemC_LIBRARIES})
add_test (pkt_switch pkt_switch)
