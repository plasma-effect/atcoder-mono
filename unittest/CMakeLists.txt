add_library(unittest_utils INTERFACE)
target_include_directories(unittest_utils INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/test_utils
)

function(apply_common_options)
  set(target ${ARGV0})
  target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wconversion
    -Wno-sign-conversion
    -Werror
    -fsanitize=undefined,address
    -fno-sanitize-recover=all
  )
  target_link_options(${target} PRIVATE
    -fsanitize=undefined,address
  )
  target_link_libraries(${target} PRIVATE
    competitive
    unittest_utils
    GTest::gtest_main
  )
  if(COMPETITIVE_BUILD_MODE STREQUAL "Develop")
    target_compile_definitions(${target} PRIVATE "LOCAL_DEBUG")
  endif()
  if(${ARGC} EQUAL 1)
    gtest_discover_tests(${target})
  endif()
endfunction()

# container
add_executable(container_dual_array_unittest container/dual_array.cpp)
apply_common_options(container_dual_array_unittest)

add_executable(container_static_dual_array_unittest container/static_dual_array.cpp)
apply_common_options(container_static_dual_array_unittest)

add_executable(container_static_queue_unittest container/static_queue.cpp)
apply_common_options(container_static_queue_unittest)

add_executable(container_static_stack_unittest container/static_stack.cpp)
apply_common_options(container_static_stack_unittest)

# common utility
add_executable(utility_assert_unittest utility/assert.cpp)
apply_common_options(utility_assert_unittest)

add_executable(utility_irange_unittest utility/irange.cpp)
apply_common_options(utility_irange_unittest)

add_executable(utility_priority_queue_unittest utility/priority_queue.cpp)
apply_common_options(utility_priority_queue_unittest)

# stdin/stdout
add_executable(io_debug_print_unittest io/debug_print.cpp)
apply_common_options(io_debug_print_unittest)

add_executable(io_normal_print_unittest io/normal_print.cpp)
apply_common_options(io_normal_print_unittest)

add_executable(io_print_base_unittest io/print_base.cpp)
apply_common_options(io_print_base_unittest)

# union headers
add_executable(stdio_unittest stdio.cpp)
apply_common_options(stdio_unittest)

add_executable(utility_unittest utility.cpp)
apply_common_options(utility_unittest)
