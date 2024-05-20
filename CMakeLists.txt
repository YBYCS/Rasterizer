cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

# 创建可执行文件，名称应与target_link_libraries保持一致
add_executable(Rasterizer main.cpp)

# 链接 GDI 库
target_link_libraries(Rasterizer gdi32)
