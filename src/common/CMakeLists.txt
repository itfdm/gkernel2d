add_library(serializer
    output_serializer/serializer.cpp)

add_library(GKERNEL::serializer ALIAS serializer)

target_compile_options(serializer
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_TEST_COMPILE_FLAGS}
    ${GKERNEL_COMMON_COMPILE_FLAGS}
)

target_compile_definitions(serializer PRIVATE
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

target_link_libraries(serializer PRIVATE GKERNEL::gkernel)

add_library(file_parser
    file_parser/parser.cpp)

add_library(GKERNEL::file_parser ALIAS file_parser)

target_compile_options(file_parser
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_TEST_COMPILE_FLAGS}
    ${GKERNEL_COMMON_COMPILE_FLAGS}
)

target_compile_definitions(file_parser PRIVATE
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

target_link_libraries(file_parser PRIVATE GKERNEL::gkernel)
