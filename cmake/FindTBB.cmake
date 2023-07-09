include(FetchContent)

FetchContent_Declare(TBB
    GIT_REPOSITORY https://github.com/oneapi-src/onetbb.git
    GIT_TAG v2021.7.0)

if (NOT TBB_POPULATED)
    if (MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
    endif()
    set(TBB_TEST OFF CACHE BOOL INTERNAL)
    set(TBBMALLOC_BUILD OFF CACHE BOOL INTERNAL)
    FetchContent_MakeAvailable(TBB)
    if (MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
endif()

set(TBB_FOUND TRUE)
