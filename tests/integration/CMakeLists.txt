add_executable(pipeline pipeline.cpp)
target_link_libraries(pipeline PRIVATE GKERNEL::gkernel TBB::tbb)
