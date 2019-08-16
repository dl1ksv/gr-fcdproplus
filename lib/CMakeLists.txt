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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX
include(GrMiscUtils)

list(APPEND fcdproplus_sources
    fcdproplus_impl.cc
    fcdpp_control_impl.cc
    fcd_control_impl.cc
    fcd_impl.cc
)

add_library(gnuradio-fcdproplus SHARED ${fcdproplus_sources})
list(APPEND fcdproplus_libs ${LIBUSB_LIBRARIES} )

if(HIDAPI_LIBRARY)
    MESSAGE(STATUS "System Hidapi will be used")
    list(APPEND fcdproplus_libs  ${HIDAPI_LIBRARY} )
    find_path(HIDAPI_INCLUDES NAMES "hidapi.h"
        HINTS /usr/include/hidapi
        PATHS
        /usr/include
        /usr/local/include
    )
MESSAGE(STATUS "Hidapi libs: ${HIDAPI_LIBRARY} ")
MESSAGE(STATUS "Hidapi header file found at: ${HIDAPI_INCLUDES}")
else ()
    list(APPEND fcdproplus_sources ${CMAKE_CURRENT_SOURCE_DIR}/hid/hid.c)
    set(HIDAPI_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/hid")
endif()
target_link_libraries(gnuradio-fcdproplus gnuradio::gnuradio-audio gnuradio::gnuradio-blocks ${fcdproplus_libs})
target_include_directories(gnuradio-fcdproplus
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PUBLIC $<BUILD_INTERFACE:${HIDAPI_INCLUDES}>
    PUBLIC $<INSTALL_INTERFACE:include>
)

if(APPLE)
  SET_TARGET_PROPERTIES(gnuradio-fcdproplus PROPERTIES
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR}")
endif(APPLE)

set_target_properties(gnuradio-fcdproplus PROPERTIES DEFINE_SYMBOL "gnuradio_fcdproplus_EXPORTS")
GR_LIBRARY_FOO(gnuradio-fcdproplus)
