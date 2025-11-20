function(apply_common_options target)
  target_link_libraries(${target} test_utils GTest::gtest_main)
  if(CMAKE_BUILD_TYPE STREQUAL "Original")
    target_link_libraries(${target} original_ac_library)
  else()
    target_compile_definitions(${target} PRIVATE LOCAL_DEBUG)
    target_link_libraries(${target} custom_ac_library)
  endif()
  gtest_discover_tests(${target})
endfunction()
  
add_executable(BasicTest basic_test.cpp)
apply_common_options(BasicTest)

add_executable(SegtreeTest segtree_test.cpp)
apply_common_options(SegtreeTest)

add_executable(LazySegtreeTest lazysegtree_test.cpp)
apply_common_options(LazySegtreeTest)

add_executable(LazySegtreeStressTest lazysegtree_stress_test.cpp)
apply_common_options(LazySegtreeStressTest)

add_executable(MathTest math_test.cpp)
apply_common_options(MathTest)

add_executable(InternalMathTest internal_math_test.cpp)
apply_common_options(InternalMathTest)

add_executable(ModintTest modint_test.cpp)
apply_common_options(ModintTest)

add_executable(DSUTest dsu_test.cpp)
apply_common_options(DSUTest)

add_executable(FenwickTreeTest fenwicktree_test.cpp)
apply_common_options(FenwickTreeTest)

add_executable(SCCTest scc_test.cpp)
apply_common_options(SCCTest)

add_executable(TwosatTest twosat_test.cpp)
apply_common_options(TwosatTest)

add_executable(StringTest string_test.cpp)
apply_common_options(StringTest)

add_executable(ConvolutionTest convolution_test.cpp)
apply_common_options(ConvolutionTest)

add_executable(MaxflowTest maxflow_test.cpp)
apply_common_options(MaxflowTest)

add_executable(MincostflowTest mincostflow_test.cpp)
apply_common_options(MincostflowTest)

if(CMAKE_BUILD_TYPE STREQUAL "Original")
add_executable(BitTest bit_test.cpp)
apply_common_options(BitTest)
endif()

add_executable(TypeTraitsTest type_traits_test.cpp)
apply_common_options(TypeTraitsTest)

# utils
add_executable(UtilsMathTest utils/math_test.cpp)
apply_common_options(UtilsMathTest)
