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
# Can be updated to add new src files
set (Trie_Sources
BitString.h
BitString.cpp
PrefixTree.cpp
PrefixTree.h
Trie.cpp
Trie.h
UnibitNode.cpp
UnibitNode.h
customallocator.h
customallocator.cpp
configdebug.h
RoutingTableEntry.cpp
RoutingTableEntry.h
BaseVectorEntry.cpp
BaseVectorEntry.h
HashTableEntry.cpp
HashTableEntry.h
HashTrie.cpp
HashTrie.h
LCNode.cpp
LCNode.h
LCTrie.cpp
LCTrie.h
PrefixVectorEntry.cpp
PrefixVectorEntry.h
MultibitNode.cpp
MultibitNode.h
MultibitTrie.cpp
MultibitTrie.h
pool_allocator_defines.h
RangeTrie.h
RangeTrie.cpp
RangeTrieNode.cpp
)

find_package (pfpsim REQUIRED)
# Include PFPSIM
include_directories (
  ${PFPSIM_INCLUDE_DIRS}
   "../../"
   )

# Support for SystemC dynamic processes
add_definitions (-DSC_INCLUDE_DYNAMIC_PROCESSES)

add_library ( TlmTries STATIC ${Trie_Sources} )


#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
#--------------Compiler Flags--------------------------------------------------
include (CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG ("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG ("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  if (COMPILER_SUPPORTS_CXX11)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fmessage-length=0 -MMD -MP -Wno-reorder -Wno-return-type -Wno-unused -Wno-overloaded-virtual")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -fcolor-diagnostics")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      message("====================> g++")
    else()
      message(FATAL "UNSUPPORTED COMPILER: ${CMAKE_CXX_COMPILER} --- USE GCC or CLANG")
    endif()
  else()
    message(FATAL "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif ()
else()
  message (FATAL "MSVC is not supported - Please use GCC or CLANG ")
endif ()
