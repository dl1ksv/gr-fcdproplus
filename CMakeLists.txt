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


cmake_minimum_required(VERSION 3.0)
project(gr-fcdproplus CXX C)

#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")


########################################################################
# Compiler specific setup
########################################################################
if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
    #http://gcc.gnu.org/wiki/Visibility
    add_definitions(-fvisibility=hidden)
endif()
########################################################################
# Find boost
########################################################################
if(UNIX AND EXISTS "/usr/lib64")
    list(APPEND BOOST_LIBRARYDIR "/usr/lib64") #fedora 64-bit fix
endif(UNIX AND EXISTS "/usr/lib64")
set(Boost_ADDITIONAL_VERSIONS
    "1.35.0" "1.35" "1.36.0" "1.36" "1.37.0" "1.37" "1.38.0" "1.38" "1.39.0" "1.39"
    "1.40.0" "1.40" "1.41.0" "1.41" "1.42.0" "1.42" "1.43.0" "1.43" "1.44.0" "1.44"
    "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47" "1.48.0" "1.48" "1.49.0" "1.49"
    "1.50.0" "1.50" "1.51.0" "1.51" "1.52.0" "1.52" "1.53.0" "1.53" "1.54.0" "1.54"
    "1.55.0" "1.55" "1.56.0" "1.56" "1.57.0" "1.57" "1.58.0" "1.58" "1.59.0" "1.59"
    "1.60.0" "1.60" "1.61.0" "1.61" "1.62.0" "1.62" "1.63.0" "1.63" "1.64.0" "1.64"
    "1.65.0" "1.65" "1.66.0" "1.66" "1.67.0" "1.67" "1.68.0" "1.68" "1.69.0" "1.69"
)
find_package(Boost "1.35" COMPONENTS filesystem system)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost required to compile fcdproplus")
endif()

########################################################################
# Find gnuradio build dependencies
########################################################################
set(GR_REQUIRED_COMPONENTS RUNTIME PMT BLOCKS AUDIO)
find_package(Gnuradio "3.8" REQUIRED PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH)
if(NOT GNURADIO_RUNTIME_FOUND)
    message(FATAL_ERROR "GnuRadio Core required to compile fcdproplus")
endif()
FIND_LIBRARY(HIDAPI_LIBRARY hidapi-libusb)
########################################################################
#
# set search path for cmake modules to OOT, Gnuradio
#
########################################################################
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(FindGnuradioModules)

if(${GNURADIO_MODULE_DIRECTORY} STREQUAL "GNURADIO_MODULE_DIRECTORY-NOTFOUND")
	message(FATAL_ERROR "Cannot find gnuradio cmake Modules directory")
else()
	message(STATUS "Modules found in ${GNURADIO_MODULE_DIRECTORY}")
endif()	
list(APPEND CMAKE_MODULE_PATH "${GNURADIO_MODULE_DIRECTORY}" )

# Set the version information here
set(VERSION_INFO_MAJOR_VERSION 3)
set(VERSION_INFO_API_COMPAT    7)
set(VERSION_INFO_MINOR_VERSION 2)
set(VERSION_INFO_MAINT_VERSION 0)
include(GrVersion) #setup version info
########################################################################
# Install directories
########################################################################
include(GrPlatform) #define LIB_SUFFIX
set(GR_RUNTIME_DIR      bin)
set(GR_LIBRARY_DIR      lib${LIB_SUFFIX})
set(GR_INCLUDE_DIR      include/fcdproplus)
set(GR_DATA_DIR         share)
set(GR_PKG_DATA_DIR     ${GR_DATA_DIR}/${CMAKE_PROJECT_NAME})
set(GR_DOC_DIR          ${GR_DATA_DIR}/doc)
set(GR_PKG_DOC_DIR      ${GR_DOC_DIR}/${CMAKE_PROJECT_NAME})
set(GR_CONF_DIR         etc)
set(GR_PKG_CONF_DIR     ${GR_CONF_DIR}/${CMAKE_PROJECT_NAME}/conf.d)
set(GR_LIBEXEC_DIR      libexec)
set(GR_PKG_LIBEXEC_DIR  ${GR_LIBEXEC_DIR}/${CMAKE_PROJECT_NAME})
set(GRC_BLOCKS_DIR      ${GR_PKG_DATA_DIR}/grc/blocks)


if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(ALSA)
    find_package(USB)
    if(NOT LIBUSB_FOUND)
        message(FATAL_ERROR "libusb required to compile fcdproplus")
    endif()
endif()
find_package(PythonLibs 2)
find_package(SWIG)

if(NOT SWIG_FOUND)
    message(FATAL_ERROR "Swig required to build fcdproplus")
    else()    
	    message(STATUS "Swig version ${SWIG_VERSION} found")
endif()

find_package(Doxygen)
########################################################################
# Setup the include and linker paths
########################################################################
set(COMMON_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${GNURADIO_RUNTIME_INCLUDE_DIRS}
)

set(COMMON_LIB_DIRS
    ${Boost_LIBRARY_DIRS}
    ${GNURADIO_BLOCKS_LIBRARY_DIRS}
)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND COMMON_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIR})
    list(APPEND COMMON_LIB_DIRS ${LIBUSB_LIB_DIR})
endif()

# Set component parameters
set(GR_FCDPROPLUS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GR_FCDPROPLUS_SWIG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/swig CACHE INTERNAL "" FORCE)
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
MESSAGE(STATUS "Building gr-fcdproplus : ${LIBVER} for ${CMAKE_SYSTEM_NAME} ")
MESSAGE(STATUS "Build for gnuradio:    : ${Gnuradio_VERSION}")
MESSAGE(STATUS "Using CMAKE Module path: ${CMAKE_MODULE_PATH}" )
MESSAGE(STATUS "GNURADIO INCLUDES IN   : ${GNURADIO_RUNTIME_INCLUDE_DIRS}")
MESSAGE(STATUS "Using install prefix   : ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "GNURADIO LIBS          : ${GNURADIO_AUDIO_LIBRARIES}")
if(HIDAPI_LIBRARY)
	  MESSAGE(STATUS "System Hidapi Lib ${HIDAPI_LIBRARY} is used")
else()	  
  MESSAGE(STATUS "Bundled hidapi is used")
endif()  
MESSAGE(STATUS "========================================================")
