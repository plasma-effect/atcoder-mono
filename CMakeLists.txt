set(CMAKE_C_COMPILER gcc-15)
set(CMAKE_CXX_COMPILER g++-15)
set(HOME_DIR $ENV{HOME})
cmake_minimum_required(VERSION 3.20)

project(AtCoderMono LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(competitive INTERFACE)
target_include_directories(competitive INTERFACE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ac-library/custom
)
if(COMPETITIVE_BUILD_MODE STREQUAL "Develop")
  target_compile_definitions(competitive INTERFACE "LOCAL_DEBUG")
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

function(check_testfile_exists)
  file(GLOB_RECURSE ALL_HEADERS "${CMAKE_SOURCE_DIR}/include/competitive/*.hpp")
  set(MISSING_TESTS "")
  foreach(header IN LISTS ALL_HEADERS)
    file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR}/include/competitive ${header})
    string(REPLACE ".hpp" ".cpp" test_path ${rel_path})
    set(full_test_path ${CMAKE_SOURCE_DIR}/unittest/${test_path})
    
    if(NOT EXISTS ${full_test_path})
      list(APPEND MISSING_TESTS ${rel_path})
    endif()
  endforeach()

  if(MISSING_TESTS)
    string(REPLACE ";" "\n  - " MISSING_MSG "${MISSING_TESTS}")
    message(FATAL_ERROR "Missing test files:\n  - ${MISSING_MSG}")
  endif()
endfunction()
check_testfile_exists()

function(check_unittest_exists)
  file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/unittest/*.cpp")
  set(MISSING_TARGETS "")

  foreach(test_src IN LISTS TEST_SOURCES)
    file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR}/unittest ${test_src})
    string(REPLACE ".cpp" "" remove_ext ${rel_path})
    string(REPLACE "/" "_" target_name ${remove_ext})
    set(full_target_name "${target_name}_unittest")

    if(NOT TARGET ${full_target_name})
      list(APPEND MISSING_TARGETS 
        "add_executable(${full_target_name} ${rel_path})\n  apply_common_options(${full_target_name})")
    endif()
  endforeach()

  if(MISSING_TARGETS)
    string(REPLACE ";" "\n  " MISSING_MSG "${MISSING_TARGETS}")
    message(FATAL_ERROR "Missing Test Targets Detected (add follow tests):\n  ${MISSING_MSG}")
  endif()
endfunction()
check_unittest_exists()
