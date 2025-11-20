cmake_minimum_required(VERSION 3.17)
project(ACLibrary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(test_utils INTERFACE)
target_include_directories(test_utils INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
)
target_compile_options(test_utils INTERFACE
  -Wall
  -Wextra
  -Wshadow
  -Wconversion
  -Wno-sign-conversion
  -Werror
  -fsanitize=undefined,address
  -fno-sanitize-recover=all
)
target_link_options(test_utils INTERFACE
  -fsanitize=undefined,address
)

add_library(original_ac_library INTERFACE)
target_include_directories(original_ac_library INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/original
)

add_library(custom_ac_library INTERFACE)
target_include_directories(custom_ac_library INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/custom
)

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
