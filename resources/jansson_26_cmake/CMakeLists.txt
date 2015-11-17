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

# this file is based on https://github.com/akheron/jansson/blob/v2.6/CMakeLists.txt
cmake_minimum_required ( VERSION 2.8.12 )
project ( jansson C )

set ( ${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../jansson )

OPTION (USE_URANDOM "Use /dev/urandom to seed the hash function." ON)
OPTION (USE_WINDOWS_CRYPTOAPI "Use CryptGenRandom to seed the hash function." ON)

# for CheckFunctionKeywords
set ( CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake" )

include (CheckCSourceCompiles)
include (CheckFunctionExists)
include (CheckFunctionKeywords)
include (CheckIncludeFiles)
include (CheckTypeSize)

check_include_files ( endian.h HAVE_ENDIAN_H)
check_include_files (fcntl.h HAVE_FCNTL_H)
check_include_files (sched.h HAVE_SCHED_H)
check_include_files (unistd.h HAVE_UNISTD_H)
check_include_files (sys/param.h HAVE_SYS_PARAM_H)
check_include_files (sys/stat.h HAVE_SYS_STAT_H)
check_include_files (sys/time.h HAVE_SYS_TIME_H)
check_include_files (sys/time.h HAVE_SYS_TYPES_H)

check_function_exists (close HAVE_CLOSE)
check_function_exists (getpid HAVE_GETPID)
check_function_exists (gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists (open HAVE_OPEN)
check_function_exists (read HAVE_READ)
check_function_exists (sched_yield HAVE_SCHED_YIELD)

# Check for the int-type includes
check_include_files (stdint.h HAVE_STDINT_H)

# Check our 64 bit integer sizes
check_type_size (__int64 __INT64)
check_type_size (int64_t INT64_T)
check_type_size ("long long" LONG_LONG_INT)

# Check our 32 bit integer sizes
check_type_size (int32_t INT32_T)
check_type_size (__int32 __INT32)
check_type_size ("long" LONG_INT)
check_type_size ("int" INT)
if (HAVE_INT32_T)
   set (JSON_INT32 int32_t)
elseif (HAVE___INT32)
   set (JSON_INT32 __int32)
elseif (HAVE_LONG_INT AND (${LONG_INT} EQUAL 4))
   set (JSON_INT32 long)
elseif (HAVE_INT AND (${INT} EQUAL 4))
   set (JSON_INT32 int)
else ()
   message (FATAL_ERROR "Could not detect a valid 32-bit integer type")
endif ()

check_type_size (uint32_t UINT32_T)
check_type_size (__uint32 __UINT32)
check_type_size ("unsigned long" UNSIGNED_LONG_INT)
check_type_size ("unsigned int" UNSIGNED_INT)
if (HAVE_UINT32_T)
   set (JSON_UINT32 uint32_t)
elseif (HAVE___UINT32)
   set (JSON_UINT32 __uint32)
elseif (HAVE_UNSIGNED_LONG_INT AND (${UNSIGNED_LONG_INT} EQUAL 4))
   set (JSON_UINT32 "unsigned long")
elseif (HAVE_UNSIGNED_INT AND (${UNSIGNED_INT} EQUAL 4))
   set (JSON_UINT32 "unsigned int")
else ()
   message (FATAL_ERROR "Could not detect a valid unsigned 32-bit integer type")
endif ()

# Check for ssize_t and SSIZE_T existance.
check_type_size(ssize_t SSIZE_T)
check_type_size(SSIZE_T UPPERCASE_SSIZE_T)
if(NOT HAVE_SSIZE_T)
   if(HAVE_UPPERCASE_SSIZE_T)
      set(JSON_SSIZE SSIZE_T)
   else()
      set(JSON_SSIZE int)
   endif()
endif()
set(CMAKE_EXTRA_INCLUDE_FILES "")

# Check for all the variants of strtoll
check_function_exists (strtoll HAVE_STRTOLL)
check_function_exists (strtoq HAVE_STRTOQ)
check_function_exists (_strtoi64 HAVE__STRTOI64)

# Figure out what variant we should use
if (HAVE_STRTOLL)
   set (JSON_STRTOINT strtoll)
elseif (HAVE_STRTOQ)
   set (JSON_STRTOINT strtoq)
elseif (HAVE__STRTOI64)
   set (JSON_STRTOINT _strtoi64)
else ()
   # fallback to strtol (32 bit)
   # this will set all the required variables
   set (JSON_STRTOINT strtol)
   set (JSON_INT_T long)
   set (JSON_INTEGER_FORMAT "\"ld\"")
endif ()

# if we haven't defined JSON_INT_T, then we have a 64 bit conversion function.
# detect what to use for the 64 bit type.
# Note: I will prefer long long if I can get it, as that is what the automake system aimed for.
if (NOT DEFINED JSON_INT_T)
   if (HAVE_LONG_LONG_INT AND (${LONG_LONG_INT} EQUAL 8))
      set (JSON_INT_T "long long")
   elseif (HAVE_INT64_T)
      set (JSON_INT_T int64_t)
   elseif (HAVE___INT64)
      set (JSON_INT_T __int64)
   else ()
      message (FATAL_ERROR "Could not detect 64 bit type, although I detected the strtoll equivalent")
   endif ()

   # Apparently, Borland BCC and MSVC wants I64d,
   # Borland BCC could also accept LD
   # and gcc wants ldd,
   # I am not sure what cygwin will want, so I will assume I64d

   if (WIN32) # matches both msvc and cygwin
      set (JSON_INTEGER_FORMAT "\"I64d\"")
   else ()
      set (JSON_INTEGER_FORMAT "\"lld\"")
   endif ()
endif ()


# If locale.h and localeconv() are available, define to 1, otherwise to 0.
check_include_files (locale.h HAVE_LOCALE_H)
check_function_exists (localeconv HAVE_LOCALECONV)

if (HAVE_LOCALECONV AND HAVE_LOCALE_H)
   set (JSON_HAVE_LOCALECONV 1)
else ()
   set (JSON_HAVE_LOCALECONV 0)
endif ()

# check if we have setlocale
check_function_exists (setlocale HAVE_SETLOCALE)

# Check what the inline keyword is.
# Note that the original JSON_INLINE was always set to just 'inline', so this goes further.
check_function_keywords("inline")
check_function_keywords("__inline")
check_function_keywords("__inline__")

if (HAVE_INLINE)
   set (JSON_INLINE inline)
elseif (HAVE___INLINE)
   set (JSON_INLINE __inline)
elseif (HAVE___INLINE__)
   set (JSON_INLINE __inline__)
else (HAVE_INLINE)
   # no inline on this platform
   set (JSON_INLINE)
endif (HAVE_INLINE)

# Find our snprintf
check_function_exists (snprintf HAVE_SNPRINTF)
check_function_exists (_snprintf HAVE__SNPRINTF)

if (HAVE_SNPRINTF)
   set (JSON_SNPRINTF snprintf)
elseif (HAVE__SNPRINTF)
   set (JSON_SNPRINTF _snprintf)
endif ()

check_c_source_compiles ("int main() { unsigned long val; __sync_bool_compare_and_swap(&val, 0, 1); return 0; } " HAVE_SYNC_BUILTINS)
check_c_source_compiles ("int main() { char l; unsigned long v; __atomic_test_and_set(&l, __ATOMIC_RELAXED); __atomic_store_n(&v, 1, __ATOMIC_RELEASE); __atomic_load_n(&v, __ATOMIC_ACQUIRE); return 0; }" HAVE_ATOMIC_BUILTINS)

# configure the public config file
configure_file (${${PROJECT_NAME}_SOURCE_DIR}/cmake/jansson_config.h.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/include/jansson_config.h)

# Copy the jansson.h file to the public include folder
file (COPY ${${PROJECT_NAME}_SOURCE_DIR}/src/jansson.h
           DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/)


# configure the private config file
configure_file (${${PROJECT_NAME}_SOURCE_DIR}/cmake/config.h.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/private_include/config.h)


# check deplibs cache
if ( STATICLIB_USE_DEPLIBS_CACHE )
    set ( ${PROJECT_NAME}_CACHED_LIB_PATH ${STATICLIB_DEPLIBS_CACHE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} )
    if ( EXISTS ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
        set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        message ( STATUS "Using cached library: [${${PROJECT_NAME}_CACHED_LIB_PATH}]" )
        return ( )
    endif ( )
endif ( )

# Add the lib sources.
file ( GLOB C_FILES ${${PROJECT_NAME}_SOURCE_DIR}/src/*.c )
add_library ( jansson STATIC ${C_FILES} )
target_include_directories ( jansson BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories ( jansson BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/private_include)
target_compile_definitions ( jansson PRIVATE -DHAVE_CONFIG_H )
target_compile_definitions ( jansson PRIVATE -DVALGRIND=1 )
if ( ${CMAKE_CXX_COMPILER_ID}x MATCHES "MSVCx" )
    target_compile_definitions ( jansson PRIVATE -D_CRT_SECURE_NO_WARNINGS )
    target_compile_options ( jansson PRIVATE /wd4005 /wd4996 /wd4706 )
endif ( )


