cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_subdirectory(trifles/)

add_executable(main main.cpp)
# target_include_directories(main PUBLIC dumb/)
