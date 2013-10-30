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

set (risc_cpu_HEADERS bios.h
                      dcache.h
                      decode.h
                      directive.h
                      exec.h
                      fetch.h
                      floating.h
                      icache.h
                      mmxu.h
                      paging.h
                      pic.h)

set (risc_cpu_SOURCES bios.cpp
                      dcache.cpp
                      decode.cpp
                      exec.cpp
                      fetch.cpp
                      floating.cpp
                      icache.cpp
                      main.cpp
                      mmxu.cpp
                      paging.cpp
                      pic.cpp)

set (risc_cpu_EXTRAS  README
                      README_TYPESCRIPT
                      abc.asm
                      assembler.pl
                      bios
                      dcache
                      icache
                      printout
                      program
                      register
                      test.asm
                      test1.asm
                      run_risc_cpu.sh)

add_executable (risc_cpu ${risc_cpu_SOURCES} ${risc_cpu_HEADERS})
target_link_libraries (risc_cpu systemc)

foreach (risc_cpu_EXTRA ${risc_cpu_EXTRAS})
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/${risc_cpu_EXTRA} ${CMAKE_CURRENT_BINARY_DIR}/${risc_cpu_EXTRA} COPYONLY)
endforeach (risc_cpu_EXTRA ${risc_cpu_EXTRAS})

add_test (risc_cpu sh ${CMAKE_CURRENT_BINARY_DIR}/run_risc_cpu.sh)
