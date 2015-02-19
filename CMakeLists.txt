
cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_clang CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_jansson CXX )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists_common.cmake )

# build steps
OPTION (BUILD_DOCS "Build documentation (uses python-sphinx)." OFF)
# there are complication with linking tests for i386
OPTION (WITHOUT_TESTS "Don't build tests ('make test' to execute tests)" ON)
add_subdirectory ( jansson )
# workaround janssons' cflags rewrite
get_property ( ${PROJECT_NAME}_COMPILE_FLAGS TARGET jansson PROPERTY COMPILE_FLAGS)
set_target_properties( jansson PROPERTIES COMPILE_FLAGS "${${PROJECT_NAME}_COMPILE_FLAGS} ${CMAKE_C_FLAGS}" )
target_compile_definitions( jansson PRIVATE -DVALGRIND=1 )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/jansson/include )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS jansson )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
