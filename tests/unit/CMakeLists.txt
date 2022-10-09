function(gkernel_add_test target_name)
    # Define the target for test
    add_executable(${target_name} ${target_name}.cpp)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_SOURCE_DIR})

    target_compile_options(${target_name}
        PRIVATE
        ${GKERNEL_WARNING_LEVEL}
        ${GKERNEL_TEST_COMPILE_FLAGS}
        ${GKERNEL_COMMON_COMPILE_FLAGS}
    )

    add_test(NAME ${target_name} COMMAND ${target_name} --force-colors=1 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    target_compile_definitions(${target_name} PRIVATE
        $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

    target_link_libraries(${target_name} PRIVATE GKERNEL::gkernel)
endfunction()

if (TARGET GKERNEL::gkernel)
    # Define the tests
    file(GLOB _gkernel_test_list *.cpp)
    foreach(_gkernel_test_path ${_gkernel_test_list})
        get_filename_component(_gkernel_test_name ${_gkernel_test_path} NAME_WE)
        gkernel_add_test(${_gkernel_test_name})
    endforeach()
endif()
