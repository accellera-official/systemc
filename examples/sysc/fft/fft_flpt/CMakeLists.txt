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
# examples/sysc/fft/fft_flpt/CMakeLists.txt --
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


set (fft_flpt_HEADERS fft.h
                      sink.h
                      source.h)

set (fft_flpt_SOURCES fft.cpp
                      main.cpp
                      sink.cpp
                      source.cpp)

set (IMAGE_FILES in_imag
                 in_imag.1
                 in_imag.2
                 in_imag.3
                 in_imag.4
                 in_real
                 in_real.1
                 in_real.2
                 in_real.3
                 in_real.4
                 out_imag.1.golden
                 out_imag.2.golden
                 out_imag.3.golden
                 out_imag.4.golden
                 out_real.1.golden
                 out_real.2.golden
                 out_real.3.golden
                 out_real.4.golden)

add_executable (fft_flpt ${fft_flpt_SOURCES} ${fft_flpt_HEADERS})
target_link_libraries (fft_flpt ${SystemC_LIBRARIES})

foreach (IMAGE_FILE ${IMAGE_FILES})
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/${IMAGE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_FILE} COPYONLY)
endforeach (IMAGE_FILE ${IMAGE_FILES})

configure_and_add_test (fft_flpt)
