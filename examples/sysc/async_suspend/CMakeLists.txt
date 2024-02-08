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
# examples/sysc/async_suspend/CMakeLists.txt --
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

option(ASYNC_SUSPEND_MATPLOT "Build async_suspend example with matplotlib for grahpical output" OFF)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_executable(async_suspend async_suspend.cpp)
target_link_libraries(async_suspend SystemC::systemc Threads::Threads)

if(ASYNC_SUSPEND_MATPLOT)
    find_package(Python3 REQUIRED COMPONENTS Development)
    target_link_libraries(async_suspend ${Python3_LIBRARIES})
    target_include_directories(async_suspend PRIVATE ${Python3_INCLUDE_DIRS})
    add_definitions(-DWITHMATPLOT)
endif()

configure_and_add_test (async_suspend)
