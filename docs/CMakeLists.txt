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
# docs/CMakeLists.txt --
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


set (DOC_FILES
         DEVELOPMENT.md
         git_branches.svg
         sysc/README
         sysc/SIGN_EXTENSION.md
         tlm/tlm_README.md)

foreach (DOC_FILE ${DOC_FILES})
  get_filename_component (DOC_INSTALL_PATH ${DOC_FILE} PATH)
  install (FILES ${DOC_FILE}
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/${DOC_INSTALL_PATH}
           COMPONENT doc
           OPTIONAL)
endforeach (DOC_FILE ${DOC_FILES})

if (BUILD_SOURCE_DOCUMENTATION)
  find_package (Doxygen REQUIRED)
  add_custom_target (doxygen ALL
                     DEPENDS doxygen-sysc doxygen-tlm)

  # SystemC source code documentation
  set (PACKAGE_NAME ${PROJECT_NAME})
  string(TOLOWER "${PROJECT_NAME}" PACKAGE_TARNAME)
  set (PACKAGE_VERSION ${SystemCLanguage_VERSION})
  set (top_srcdir ${CMAKE_SOURCE_DIR})

  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/sysc/doxygen/Doxyfile.in
                  ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/Doxyfile @ONLY)
  if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html)
    file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html)
  endif (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html)
  set_source_files_properties (${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html
                               PROPERTIES GENERATED 1)
  add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html/index.html
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/Doxyfile
                      DEPENDS systemc
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen
                      COMMENT "Building the SystemC source code documentation using Doxygen.")
  add_custom_target (doxygen-sysc
                     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html/index.html)
  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/html
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/sysc/doxygen
           COMPONENT doc)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/sysc/doxygen/${PACKAGE_TARNAME}.tag
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/sysc/doxygen
           COMPONENT doc)

  # TLM source code documentation
  set (TLM_PACKAGE_VERSION ${SystemCTLM_VERSION})
  set (top_srcdir ${CMAKE_SOURCE_DIR})

  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/tlm/doxygen/Doxyfile.in
                  ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/Doxyfile @ONLY)
  if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html)
    file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html)
  endif (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html)
  set_source_files_properties (${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html
                               PROPERTIES GENERATED 1)
  add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html/index.html
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/Doxyfile
                      DEPENDS doxygen-sysc
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen
                      COMMENT "Building the TLM source code documentation using Doxygen.")
  add_custom_target (doxygen-tlm
                     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html/index.html)
  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/html
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/tlm/doxygen
           COMPONENT doc)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/tlm/doxygen/tlm.tag
           DESTINATION ${CMAKE_INSTALL_DOCDIR}/tlm/doxygen
           COMPONENT doc)
endif (BUILD_SOURCE_DOCUMENTATION)
