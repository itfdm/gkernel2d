function(gkernel_add_bench target_name)
    add_executable(${target_name} ${target_name}.cpp)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../src/common)

    target_compile_options(${target_name}
        PRIVATE
        ${GKERNEL_WARNING_LEVEL}
        ${GKERNEL_TEST_COMPILE_FLAGS}
        ${GKERNEL_COMMON_COMPILE_FLAGS}
    )

    target_compile_definitions(${target_name} PRIVATE
        $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

    target_link_libraries(${target_name} PRIVATE GKERNEL::gkernel GKERNEL::file_parser benchmark::benchmark)
endfunction()

include(FetchContent)

FetchContent_Declare(
    googlebench
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.6.1)

set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googlebench)

if (TARGET GKERNEL::gkernel)
    # Define the tests
    file(GLOB _gkernel_test_list *.cpp)
    foreach(_gkernel_test_path ${_gkernel_test_list})
        get_filename_component(_gkernel_test_name ${_gkernel_test_path} NAME_WE)
        gkernel_add_bench(${_gkernel_test_name})
    endforeach()
endif()
