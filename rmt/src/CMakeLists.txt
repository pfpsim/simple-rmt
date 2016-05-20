#
# simple-rmt: Example RMT simulation model using the PFPSim Framework
#
# Copyright (C) 2016 Concordia Univ., Montreal
#     Samar Abdi
#     Umair Aftab
#     Gordon Bailey
#     Faras Dewal
#     Shafigh Parsazad
#     Eric Tremblay
#
# Copyright (C) 2016 Ericsson
#     Bochra Boughzala
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#

cmake_minimum_required (VERSION 2.8.7)
set(ProjectName
rmt
)
set(Executable
rmt-sim
)

include(ExternalProject)
project (${ProjectName})

add_subdirectory(behavioural)
add_subdirectory(structural)
set(SOURCE_FILES
  ${STRUCTURAL_SRC}
  ${BEHAVIOURAL_SRC}
)

#Tries Lib
set ( TLM_TRIES_NAME TlmTries )
set ( TLM_TRIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tries )
set ( TLM_TRIES_PATH ${PROJECT_BINARY_DIR}/tries-build/lib${TLM_TRIES_NAME}.a  )
if("${CMAKE_VERSION}" VERSION_GREATER 3.2.0.0)
  ExternalProject_Add(tries
     SOURCE_DIR ${TLM_TRIES_DIR}/src
     BINARY_DIR ${PROJECT_BINARY_DIR}/tries-build
     BUILD_BYPRODUCTS ${PROJECT_BINARY_DIR}/tries-build/libTlmTries.a
     INSTALL_COMMAND /bin/true
  )
else()
  # Sadly for versions < 3.2 BUILD_BYPRODUCTS and BUILD_ALWAYS don't exist
  ExternalProject_Add(tries
     SOURCE_DIR ${TLM_TRIES_DIR}/src
     BINARY_DIR ${PROJECT_BINARY_DIR}/tries-build
     INSTALL_COMMAND /bin/true
  )
endif()
set_source_files_properties( ${TLM_TRIES_PATH} PROPERTIES
   EXTERNAL_OBJECT TRUE
   GENERATED TRUE
)

#Executable
add_executable (${Executable} ${SOURCE_FILES} ${TLM_TRIES_PATH})
add_dependencies(${Executable} tries)

# Find Packages
find_package(pfpsim REQUIRED)
find_package(pfp-p4 REQUIRED)

# Support for SystemC dynamic processes
add_definitions (-DSC_INCLUDE_DYNAMIC_PROCESSES)

# Include SystemC, TLM, and third-party headers
include_directories (
  ${PFP_P4_INCLUDE_DIRS}
  ${PFPSIM_INCLUDE_DIRS}
  "${TLM_TRIES_DIR}/src"
   "./"
 )

# Link the appropriate libraries
target_link_libraries(${Executable}
    ${PROJECT_BINARY_DIR}/tries-build/libTlmTries.a
    ${PFP_P4_LIBRARIES}
    ${PFPSIM_LIBRARIES}
    -lpthread
    -lgmp
  )

#-----------IDEs (especially VS)------------------------------------------------
source_group("" FILES ${SOURCE_FILES})
# Account for Eclipse's weird C++11 syntax handling
if (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
   add_definitions (-DCMAKE_CXX_COMPILER_ARG1=-std=c++11)
   add_definitions (-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE)   # Also some niceties for Eclipse project file handling
endif (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
#--------------PFPDEBUGGER FLAGS------------------------------------------------
option(PFPSIMDEBUGGER "PFPSIMDEBUGGER" ON)
if("${PFPSIMDEBUGGER}" MATCHES "ON")
  message (STATUS "PFPSIM DEBUGGER FLAG IS ON")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0 -DPFPSIM_DEBUGGER")
else ()
  message (STATUS "PFPSIM DEBUGGER FLAG IS OFF")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
endif()
#--------------Compiler Flags--------------------------------------------------
include (CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG ("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG ("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  if (COMPILER_SUPPORTS_CXX11)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fmessage-length=0 -MMD -MP -Wno-reorder -Wno-return-type -Wno-unused -Wno-overloaded-virtual")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -fcolor-diagnostics")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      message(FATAL "UNSUPPORTED COMPILER: ${CMAKE_CXX_COMPILER} --- USE GCC or CLANG")
    endif()
  else()
    message(FATAL "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif ()
else()
  message (FATAL "MSVC is not supported - Please use GCC or CLANG ")
endif ()
