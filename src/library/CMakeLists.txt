# main library
add_library(gkernel
    src/containers.cpp
    src/parser.cpp
    src/intersection.cpp
    src/area_analyzer.cpp
    src/serializer.cpp)

add_library(GKERNEL::gkernel ALIAS gkernel)

target_compile_definitions(gkernel
    PUBLIC
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

target_include_directories(gkernel
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/tbb/include)

find_package(TBB REQUIRED)

set_target_properties(tbb PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
)

target_compile_options(gkernel
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_COMMON_COMPILE_FLAGS})

target_link_libraries(gkernel PRIVATE TBB::tbb)
