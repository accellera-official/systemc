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
# examples/CMakeLists.txt --
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


if (NOT BUILD_EXAMPLES)
  set_directory_properties (PROPERTIES EXCLUDE_FROM_ALL 1)
endif (NOT BUILD_EXAMPLES)

include_directories (BEFORE ${SystemC_SOURCE_DIR}/src)

if (WIN32)
  # Not sure, if these preprocessor definitions are strictly required to build
  # console applications with Visual Studio. Instead of _CONSOLE, _LIB may be
  # more appropriate. Maybe also WIN32, _WIN32, _WIN64 needs to be #defined to
  # generate correct VS projects.
  #
  # Resources:
  # <http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/9e45a760-f2b8-47b2-8b8c-dfeeb1b24c92/>.
  # <http://www.itk.org/Bug/view.php?id=13292>
  # <http://www.sunlightd.com/Archive/Windows/FAQ.aspx>
  # <http://msdn.microsoft.com/en-us/library/9yb4317s(v=vs.80).aspx>
  # <http://www.windows-api.com/microsoft/VC-MFC/32308312/preprocessor-definitions-for-x64-projects.aspx>
  add_definitions (-D_CONSOLE -DNOGDI)
endif (WIN32)

# On many platforms (e.g., Linux), code that links to SystemC also needs to
# link to ${CMAKE_THREAD_LIBS_INIT}.
#
# TODO: Check why CMake doesn't add automatically ${CMAKE_THREAD_LIBS_INIT},
# as already target systemc depends on it.
set (SystemC_LIBRARIES systemc ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory (sysc)
add_subdirectory (tlm)
