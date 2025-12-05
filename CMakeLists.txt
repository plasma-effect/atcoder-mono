set(CMAKE_C_COMPILER gcc-15)
set(CMAKE_CXX_COMPILER g++-15)
set(HOME_DIR $ENV{HOME})
cmake_minimum_required(VERSION 3.20)

project(AtCoderMono LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(competitive STATIC
  ${PROJECT_SOURCE_DIR}/src/assert.cpp)
target_include_directories(competitive PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ac-library/custom
)
if(COMPETITIVE_BUILD_MODE STREQUAL "Develop")
  target_compile_definitions(competitive PUBLIC "LOCAL_DEBUG")
endif()

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
add_subdirectory(unittest)
