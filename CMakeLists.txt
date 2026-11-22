cmake_minimum_required(VERSION 3.16)
project(lsm_kv_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Avertismente stricte pentru cod curat
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Biblioteca principala a motorului nostru
add_library(kstore_lib
        src/bloom_filter.cpp
)
target_include_directories(kstore_lib PUBLIC include)

# Descarcam GoogleTest automat
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Executabilul de teste
add_executable(kstore_tests
        tests/bloom_filter_tests.cpp
)
target_link_libraries(kstore_tests PRIVATE kstore_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(kstore_tests)