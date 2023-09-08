cmake_minimum_required(VERSION 3.5)
project(yf_server)

# 设置语言标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成提示文件
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# 设置库生成路径
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
message(STATUS "library dir: ${CMAKE_SOURCE_DIR}/lib")

# 设置二进制文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
message(STATUS "executable dir: ${CMAKE_SOURCE_DIR}/bin")

# 设置头文件包含路径
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/log
    ${CMAKE_SOURCE_DIR}/src/thread
    ${CMAKE_SOURCE_DIR}/src/network
)

# 设置不同编译模式的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3")

option(USE_TEST "is use test file?" True)
if(USE_TEST)
    set(CMAKE_BUILD_TYPE "Debug")
else(USE_TEST)
    set(CMAKE_BUILD_TYPE "Release")
endif(USE_TEST)

# 新增子cmake路径
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
if(USE_TEST)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif(USE_TEST)