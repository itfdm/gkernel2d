# main library
add_library(gkernel
    src/containers.cpp
    src/parser.cpp
    src/serializer.cpp
    src/intersection.cpp)

add_library(GKERNEL::gkernel ALIAS gkernel)

target_compile_definitions(gkernel
    PUBLIC
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

target_include_directories(gkernel
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(gkernel
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_COMMON_COMPILE_FLAGS})
