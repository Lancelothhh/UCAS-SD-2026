cmake_minimum_required(VERSION 3.10)
project(avl_dynamic_search_table C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(avl_tree src/avl_tree.c)
target_include_directories(avl_tree PUBLIC src)

add_executable(avl_demo src/main.c)
target_link_libraries(avl_demo PRIVATE avl_tree)

add_executable(test_avl tests/test_avl.c)
target_link_libraries(test_avl PRIVATE avl_tree)

add_executable(benchmark_avl benchmark/benchmark_avl.c)
target_link_libraries(benchmark_avl PRIVATE avl_tree)

enable_testing()
add_test(NAME correctness COMMAND test_avl)

