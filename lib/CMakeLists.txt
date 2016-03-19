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

link_directories(${COMMON_LIB_DIRS})

list(APPEND fcdproplus_sources
    fcdproplus_impl.cc
)


if(APPLE)
  FIND_LIBRARY(IOKIT_LIBRARY IOKit)
  list(APPEND fcdproplus_libs ${IOKIT_LIBRARY})
  FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
  list(APPEND fcdproplus_libs ${COREFOUNDATION_LIBRARY})
else()
 list(APPEND fcdproplus_libs ${LIBUSB_LIBRARIES} )
endif()
if(HIDAPI_LIBRARY)
  add_library(gnuradio-fcdproplus SHARED ${fcdproplus_sources})
  list(APPEND fcdproplus_libs  ${HIDAPI_LIBRARY} )
  set_target_properties(gnuradio-fcdproplus PROPERTIES COMPILE_DEFINITIONS 
	  "SYSTEM_HIDAPI")
  include_directories(
	      ${COMMON_INCLUDE_DIRS} 
	      )
else()
#  MESSAGE(STATUS "Bundled hidapi is used")
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/hid
    ${COMMON_INCLUDE_DIRS} 
  )
  if(APPLE)
    list(APPEND fcdproplus_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/hid/hidmac.c)
  else()
   list(APPEND fcdproplus_sources ${CMAKE_CURRENT_SOURCE_DIR}/hid/hid.c)
  endif()
  add_library(gnuradio-fcdproplus SHARED ${fcdproplus_sources})
endif()


target_link_libraries(gnuradio-fcdproplus ${fcdproplus_libs} ${Boost_LIBRARIES}
  ${GNURADIO_RUNTIME_LIBRARIES} ${GNURADIO_BLOCKS_LIBRARIES}
  ${GNURADIO_AUDIO_LIBRARIES}
)

target_link_libraries(gnuradio-fcdproplus ${GNURADIO_PMT_LIBRARIES})
if(APPLE)
  SET_TARGET_PROPERTIES(gnuradio-fcdproplus PROPERTIES
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR}")
endif(APPLE)

set_target_properties(gnuradio-fcdproplus PROPERTIES DEFINE_SYMBOL "gnuradio_fcdproplus_EXPORTS")
GR_LIBRARY_FOO(gnuradio-fcdproplus)
