project(a)

cmake_minimum_required(VERSION 3.0)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(co dts_coroutine.c test.c)
