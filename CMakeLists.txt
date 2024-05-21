cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(Rasterizer ${SOURCE_FILES})

# 链接 GDI 库
target_link_libraries(Rasterizer gdi32)
