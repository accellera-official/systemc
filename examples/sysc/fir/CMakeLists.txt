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


add_executable (fir main.cpp
                    fir.h
                    fir.cpp
                    fir_const.h
                    stimulus.h
                    stimulus.cpp
                    display.h
                    display.cpp)
target_link_libraries (fir SystemC::systemc)
configure_and_add_test (fir)

add_executable (fir_rtl main_rtl.cpp
                        fir_top.h
                        fir_fsm.h
                        fir_fsm.cpp
                        fir_data.h
                        fir_data.cpp
                        fir_const.h
                        stimulus.h
                        stimulus.cpp
                        display.h
                        display.cpp)
target_link_libraries (fir_rtl SystemC::systemc)
configure_and_add_test (fir_rtl)
