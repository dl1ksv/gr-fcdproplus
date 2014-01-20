# Copyright 2011,2012 Free Software Foundation, Inc.
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

include_directories(
        ${Boost_INCLUDE_DIR}
        ${LIBUSB_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/hid
)

link_directories(${Boost_LIBRARY_DIRS} )

list(APPEND fcdproplus_sources
    fcdproplus_impl.cc
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  list(APPEND fcdproplus_sources ${CMAKE_CURRENT_SOURCE_DIR}/hid/hidmac.c)
else()
  list(APPEND fcdproplus_sources ${CMAKE_CURRENT_SOURCE_DIR}/hid/hid.c)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  FIND_LIBRARY(IOKIT_LIBRARY IOKit)
  list(APPEND fcdproplus_libs ${IOKIT_LIBRARY})
else()
  list(APPEND fcdproplus_libs ${LIBUSB_LIBRARIES} )
endif()
add_library(gnuradio-fcdproplus SHARED ${fcdproplus_sources})
target_link_libraries(gnuradio-fcdproplus ${Boost_LIBRARIES}
${GNURADIO_RUNTIME_LIBRARIES} ${GNURADIO_BLOCKS_LIBRARIES}
${GNURADIO_AUDIO_LIBRARIES} ${fcdproplus_libs})
set_target_properties(gnuradio-fcdproplus PROPERTIES DEFINE_SYMBOL "gnuradio_fcdproplus_EXPORTS")
MESSAGE(STATUS "Audio LIBS: ${GNURADIO_AUDIO_LIBRARIES}")
########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-fcdproplus
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
#include(GrTest)
#
#include_directories(${CPPUNIT_INCLUDE_DIRS})
#
#list(APPEND test_fcdproplus_sources
#    ${CMAKE_CURRENT_SOURCE_DIR}/test_fcdproplus.cc
#    ${CMAKE_CURRENT_SOURCE_DIR}/qa_fcdproplus.cc
#)
#
#add_executable(test-fcdproplus ${test_fcdproplus_sources})
#
#target_link_libraries(
#  test-fcdproplus
#  ${GNURADIO_CORE_LIBRARIES}
#  ${Boost_LIBRARIES}
#  ${CPPUNIT_LIBRARIES}
#  gnuradio-fcdproplus
#)
#
#GR_ADD_TEST(test_fcdproplus test-fcdproplus)
