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

# project
project ( external_jansson CXX C )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 2.6.0.3 )
set ( ${PROJECT_NAME}_DESCRIPTION "Jansson JSON library build for Staticlibs" )
set ( ${PROJECT_NAME}_URL https://github.com/staticlibs/external_jansson )

# options
SET ( BUILD_DOCS OFF CACHE BOOL "" )
# there are complication with linking tests for i386
SET ( WITHOUT_TESTS ON CACHE BOOL "" )
SET ( STATIC_CRT ON CACHE BOOL "" )

# jansson library
add_subdirectory ( jansson )
# workaround jansson's cflags rewrite, first attempt won't work because of quotation
# target_compile_options ( jansson PRIVATE ${CMAKE_C_FLAGS} )
get_property ( ${PROJECT_NAME}_COMPILE_FLAGS TARGET jansson PROPERTY COMPILE_FLAGS)
set_target_properties( jansson PROPERTIES COMPILE_FLAGS "${${PROJECT_NAME}_COMPILE_FLAGS} ${CMAKE_C_FLAGS}" )
set ( ${PROJECT_NAME}_OPTIONS "")
if ( ${CMAKE_CXX_COMPILER_ID}x MATCHES "MSVCx" )
    set ( ${PROJECT_NAME}_OPTIONS "${${PROJECT_NAME}_OPTIONS} /wd4706")
endif ( )
target_compile_options ( jansson PRIVATE ${${PROJECT_NAME}_OPTIONS} )
target_compile_definitions ( jansson PRIVATE -DVALGRIND=1 )


set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/jansson/include )

# pkg-config
if ( NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
endif ( )
set ( ${PROJECT_NAME}_MODULE jansson )
set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/jansson/include ${${PROJECT_NAME}_OPTIONS}" )
set ( ${PROJECT_NAME}_PC_LIBS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -ljansson" )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/jansson.pc )
