cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(contractor VERSION 0.1 LANGUAGES CXX)
include(CTest)
set(CONTRACTOR_DEFAULT_PROVER "contractor::prove_clever_lazy" CACHE STRING "default prover")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(example example.cpp)
add_executable(tests test/main.cpp test/test_cyclic.cpp test/test_prover.cpp)

target_include_directories(example PRIVATE .)
target_include_directories(tests PRIVATE .)

target_compile_definitions(example PRIVATE CONTRACTOR_DEFAULT_PROVER=${CONTRACTOR_DEFAULT_PROVER})
target_compile_definitions(tests PRIVATE CONTRACTOR_DEFAULT_PROVER=${CONTRACTOR_DEFAULT_PROVER})

set_target_properties(example tests PROPERTIES CXX_STANDARD 14)

add_test(tests tests)
