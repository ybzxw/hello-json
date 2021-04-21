# 所需cmake最低版本
cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)

# 设置项目名字
project(hello_json)

# 设置源代码目录
aux_source_directory(. DIR_SRCS)

# 添加头文件目录
include_directories("${PROJECT_SOURCE_DIR}/src")

# 添加库
add_subdirectory(src)

# 添加可执行文件
add_executable(test test.cpp)

target_link_libraries(test src)