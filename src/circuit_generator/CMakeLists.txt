add_executable(generator
    generator.cpp)

target_include_directories(generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(generator
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_TEST_COMPILE_FLAGS}
    ${GKERNEL_COMMON_COMPILE_FLAGS}
)

target_compile_definitions(generator PRIVATE
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

target_link_libraries(generator PRIVATE GKERNEL::serializer GKERNEL::gkernel)
