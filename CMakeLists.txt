# Copyright 2015, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_gcc CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_jansson CXX )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 2.6.0.1 )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/staticlibs_common.cmake )

# build steps
SET ( BUILD_DOCS OFF CACHE BOOL "" )
# there are complication with linking tests for i386
SET ( WITHOUT_TESTS ON CACHE BOOL "" )
SET ( STATIC_CRT ON CACHE BOOL "" )
add_subdirectory ( jansson )
# workaround jansson's cflags rewrite, first attempt won't work because of quotation
# target_compile_options ( jansson PRIVATE ${CMAKE_C_FLAGS} )
get_property ( ${PROJECT_NAME}_COMPILE_FLAGS TARGET jansson PROPERTY COMPILE_FLAGS)
set_target_properties( jansson PROPERTIES COMPILE_FLAGS "${${PROJECT_NAME}_COMPILE_FLAGS} ${CMAKE_C_FLAGS}" )
if ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_msvc" )
    target_compile_options ( jansson PRIVATE /wd4706 )
endif ( )
target_compile_definitions ( jansson PRIVATE -DVALGRIND=1 )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/jansson/include )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS jansson )
set ( ${PROJECT_NAME}_TEST_OPTS "" )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME}_TEST_OPTS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
