cmake_minimum_required(VERSION 3.14)

set(_gkernel_ver_major 0)
set(_gkernel_ver_minor 3)

project(GKERNEL VERSION ${_gkernel_ver_major}.${_gkernel_ver_minor} LANGUAGES CXX)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF) # use -std=c++... instead of -std=gnu++...
if (LINUX AND CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
endif()

set(GKERNEL_COMMON_COMPILE_FLAGS "")

option(GKERNEL_GENERATOR "Build generator" OFF)
option(GKERNEL_VISUALIZER "Build visualizer" OFF)
option(GKERNEL_UNIT_TESTING "Enable unit testing" ON)
option(GKERNEL_INTEGRATION_TESTING "Enable integration testing" OFF)
option(GKERNEL_PERF_TESTING "Enable performance testing" OFF)
option(GKERNEL_STRICT "Treat compiler warnings as errors" OFF)
option(GKERNEL_DOCS "Enable documentation build" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE release CACHE STRING "Build type" FORCE)
    message(STATUS "CMAKE_BUILD_TYPE is not specified. Using default: ${CMAKE_BUILD_TYPE}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

foreach(output_type LIBRARY ARCHIVE PDB RUNTIME)
    if (CMAKE_BUILD_TYPE)
        string(TOLOWER ${CMAKE_BUILD_TYPE} _gkernel_build_type_lower)
        set(CMAKE_${output_type}_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_${_gkernel_build_type_lower})
        unset(_gkernel_build_type_lower)
    endif()

    if (CMAKE_CONFIGURATION_TYPES)
        foreach(suffix ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${suffix} _gkernel_suffix_upper)
            string(TOLOWER ${suffix} _gkernel_suffix_lower)
            set(CMAKE_${output_type}_OUTPUT_DIRECTORY_${_gkernel_suffix_upper} ${CMAKE_BINARY_DIR}/bin_${_gkernel_suffix_lower})
        endforeach()
        unset(_gkernel_suffix_lower)
        unset(_gkernel_suffix_upper)
    endif()
endforeach()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GKERNEL_WARNING_LEVEL -Wall -Wextra $<$<BOOL:${GKERNEL_STRICT}>:-Werror> -Wfatal-errors)
endif()

if (MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP8")
    set(GKERNEL_WARNING_LEVEL /W4 $<$<BOOL:${GKERNEL_STRICT}>:/WX>)
endif()

add_subdirectory(src/library)

if (GKERNEL_GENERATOR)
    add_subdirectory(src/circuit_generator)
endif()

if (GKERNEL_VISUALIZER)
    add_subdirectory(src/visualizer)
endif()

if (GKERNEL_UNIT_TESTING OR GKERNEL_INTEGRATION_TESTING)
    enable_testing()
endif()

if (GKERNEL_UNIT_TESTING)
    add_subdirectory(tests/unit)
endif()

if (GKERNEL_INTEGRATION_TESTING)
    add_subdirectory(tests/integration)
endif()

if (GKERNEL_PERF_TESTING)
    add_subdirectory(tests/perf)
endif()

add_subdirectory(docs_infra)
