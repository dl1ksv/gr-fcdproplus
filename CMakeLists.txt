# Copyright 2011-2014 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.


########################################################################
# Project setup
########################################################################
########################################################################
#
# First look in the install directory for package config of gnuradio
#
########################################################################
set( ENV{PKG_CONFIG_PATH} "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

cmake_minimum_required(VERSION 3.8)
project(gr-fcdproplus CXX C)

#install to PyBOMBS target prefix if defined
if(DEFINED ENV{PYBOMBS_PREFIX})
    set(CMAKE_INSTALL_PREFIX $ENV{PYBOMBS_PREFIX})
    message(STATUS "PyBOMBS installed GNU Radio. Setting CMAKE_INSTALL_PREFIX to $ENV{PYBOMBS_PREFIX}")
endif()

#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

########################################################################
#
# setup version info
#
########################################################################

set(VERSION_MAJOR 3)
set(VERSION_API 8)
SET(VERSION_ABI 0)
SET(VERSION_PATCH 0)

cmake_policy(SET CMP0011 NEW)
########################################################################
# Compiler specific setup
########################################################################
#if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
#    #http://gcc.gnu.org/wiki/Visibility
#    add_definitions(-fvisibility=hidden)
#  endif()
if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    AND NOT WIN32)
    #http://gcc.gnu.org/wiki/Visibility
    add_definitions(-fvisibility=hidden)
endif()
########################################################################
# Find gnuradio build dependencies
########################################################################

find_package(Gnuradio "3.8" REQUIRED audio blocks PATHS ${CMAKE_INSTALL_PREFIX} )
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake/Modules")
FIND_LIBRARY(HIDAPI_LIBRARY hidapi-libusb)

include(GrVersion)

########################################################################
# Find fcd(proplus) dependencies
########################################################################
if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(USB)
    if(NOT LIBUSB_FOUND)
        message(FATAL_ERROR "libusb required to compile fcdproplus")
    endif()
endif()
########################################################################
# Install directories
########################################################################
include(GrPlatform) #define LIB_SUFFIX
if(NOT CMAKE_MODULES_DIR)
  SET(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
endif(NOT CMAKE_MODULES_DIR)

set(GR_RUNTIME_DIR      bin)
set(GR_LIBRARY_DIR      lib${LIB_SUFFIX})
set(GR_INCLUDE_DIR      include/fcdproplus)
set(GR_DATA_DIR         share)
set(GR_PKG_DATA_DIR     ${CMAKE_INSTALL_PREFIX}/${GR_DATA_DIR}/${CMAKE_PROJECT_NAME})
set(GR_DOC_DIR          ${GR_DATA_DIR}/doc)
set(GR_PKG_DOC_DIR      ${GR_DOC_DIR}/${CMAKE_PROJECT_NAME})
set(GR_CONF_DIR         etc)
set(GR_PKG_CONF_DIR     ${GR_CONF_DIR}/${CMAKE_PROJECT_NAME}/conf.d)
set(GR_LIBEXEC_DIR      libexec)
set(GR_PKG_LIBEXEC_DIR  ${GR_LIBEXEC_DIR}/${CMAKE_PROJECT_NAME})
set(GRC_BLOCKS_DIR      ${CMAKE_INSTALL_PREFIX}/${GR_DATA_DIR}/gnuradio/grc/blocks)

SET(GR_CMAKE_DIR        ${CMAKE_MODULES_DIR}/gr-fcdproplus)

########################################################################
# On Apple only, set install name and use rpath correctly, if not already set
########################################################################
if(APPLE)
    if(NOT CMAKE_INSTALL_NAME_DIR)
        set(CMAKE_INSTALL_NAME_DIR
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install Name Destination Directory" FORCE)
    endif(NOT CMAKE_INSTALL_NAME_DIR)
    if(NOT CMAKE_INSTALL_RPATH)
        set(CMAKE_INSTALL_RPATH
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install RPath" FORCE)
    endif(NOT CMAKE_INSTALL_RPATH)
    if(NOT CMAKE_BUILD_WITH_INSTALL_RPATH)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH ON CACHE
            BOOL "Do Build Using Library Install RPath" FORCE)
    endif(NOT CMAKE_BUILD_WITH_INSTALL_RPATH)
endif(APPLE)

########################################################################
# Setup doxygen option
########################################################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
        option(ENABLE_DOXYGEN "Build docs using Doxygen" ON)
else(DOXYGEN_FOUND)
        option(ENABLE_DOXYGEN "Build docs using Doxygen" OFF)
endif(DOXYGEN_FOUND)

########################################################################
# Create uninstall target
########################################################################
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include/fcdproplus)
add_subdirectory(lib)
add_subdirectory(swig)
add_subdirectory(python)
add_subdirectory(grc)
add_subdirectory(docs)
########################################################################
# Create Pkg Config File
########################################################################
# fake gnuradio cpack behavior as long as we don't use it directly
set(CPACK_PACKAGE_NAME "gnuradio-fcdproplus")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GNU Radio block for Funcube Dongle Pro+")
set(CPACK_PACKAGE_VERSION ${VERSION})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/gnuradio-fcdproplus.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/gnuradio-fcdproplus.pc
@ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/gnuradio-fcdproplus.pc
    DESTINATION ${GR_LIBRARY_DIR}/pkgconfig
    COMPONENT "fcdproplus_devel"
)


########################################################################
# Print Summary
########################################################################
MESSAGE(STATUS "================ Build Summary =========================")
MESSAGE(STATUS "Building gr-fcdproplus  : ${LIBVER} for ${CMAKE_SYSTEM_NAME} ")
MESSAGE(STATUS "Building for gnuradio   : ${Gnuradio_VERSION}")
MESSAGE(STATUS "Using CMAKE Module path : ${CMAKE_MODULE_PATH}" )
MESSAGE(STATUS "CMake Modules Dir       : ${CMAKE_MODULES_DIR}" )
MESSAGE(STATUS "fcdproplus INCLUDES     : ${GR_INCLUDE_DIR}")
MESSAGE(STATUS "Using install prefix    : ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "Installing grc files to : ${GRC_BLOCKS_DIR}")
if(HIDAPI_LIBRARY)
	  MESSAGE(STATUS "System Hidapi Lib ${HIDAPI_LIBRARY} is used")
else()	  
  MESSAGE(STATUS "Bundled hidapi is used")
endif()  
MESSAGE(STATUS "========================================================")
