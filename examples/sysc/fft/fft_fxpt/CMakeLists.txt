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
# examples/sysc/fft/fft_fxpt/CMakeLists.txt --
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


add_executable (fft_fxpt main.cpp
                         source.h
                         source.cpp
                         fft.h
                         fft.cpp
                         sink.h
                         sink.cpp)
target_link_libraries (fft_fxpt SystemC::systemc)

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

foreach (IMAGE_FILE ${IMAGE_FILES})
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/${IMAGE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_FILE} COPYONLY)
endforeach (IMAGE_FILE ${IMAGE_FILES})

configure_and_add_test (fft_fxpt)
