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
# examples/sysc/risc_cpu/CMakeLists.txt --
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


# TODO: Fix multiple driver issue in risc_cpu example.
# TODO: Fix segfault when risc_cpu is not started from the current directory.

add_executable (risc_cpu main.cpp
                         directive.h
                         bios.h
                         bios.cpp
                         paging.h
                         paging.cpp
                         icache.h
                         icache.cpp
                         fetch.h
                         fetch.cpp
                         decode.h
                         decode.cpp
                         exec.h
                         exec.cpp
                         mmxu.h
                         mmxu.cpp
                         floating.h
                         floating.cpp
                         dcache.h
                         dcache.cpp
                         pic.h
                         pic.cpp)
target_link_libraries (risc_cpu SystemC::systemc)

set (risc_cpu_EXTRAS  README
                      README_TYPESCRIPT
                      abc.asm
                      assembler.pl
                      bios.img
                      dcache.img
                      icache.img
                      printout.img
                      program.img
                      register.img
                      test.asm
                      test1.asm)

foreach (risc_cpu_EXTRA ${risc_cpu_EXTRAS})
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/${risc_cpu_EXTRA} ${CMAKE_CURRENT_BINARY_DIR}/${risc_cpu_EXTRA} COPYONLY)
endforeach (risc_cpu_EXTRA ${risc_cpu_EXTRAS})

configure_and_add_test (risc_cpu)
