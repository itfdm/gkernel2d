add_executable(visualizer
    main.cpp)

find_package(PythonLibs REQUIRED)

target_include_directories(visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIRS})

target_compile_options(visualizer
    PRIVATE
    ${GKERNEL_WARNING_LEVEL}
    ${GKERNEL_TEST_COMPILE_FLAGS}
    ${GKERNEL_COMMON_COMPILE_FLAGS}
)

target_compile_definitions(visualizer PRIVATE
    $<$<CONFIG:DEBUG>:GKERNEL_DEBUG>)

if (APPLE)
    target_link_libraries(visualizer PRIVATE GKERNEL::gkernel ${PYTHON_LIBRARIES}/../Python3)
else()
    target_link_libraries(visualizer PRIVATE GKERNEL::gkernel ${PYTHON_LIBRARIES})
endif()
