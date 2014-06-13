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
# docs/tlm/doxygen/CMakeLists.txt --
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


###############################################################################
# Build source documentation with Doxygen
###############################################################################
# Attention: Currently, the Doxygen-generated documentation is written to the
# source tree!
###############################################################################

if (BUILD_SOURCE_DOC)
  set (PACKAGE_NAME ${PROJECT_NAME})
  set (PACKAGE_VERSION ${PROJECT_VERSION})
  set (top_srcdir ${CMAKE_SOURCE_DIR})
  set (top_bindir ${CMAKE_BINARY_DIR})

  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/html)
    file (MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/html)
  endif (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/html)
  add_custom_command (OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/html/index.html
                      COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                              systemc)
  add_custom_target (doxygen-tlm
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/html/index.html)
  install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/html
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/tlm/doxygen
           COMPONENT doc)
endif (BUILD_SOURCE_DOC)
