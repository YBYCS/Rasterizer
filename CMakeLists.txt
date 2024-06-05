cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(Rasterizer ${SOURCE_FILES})
file(COPY ${CMAKE_SOURCE_DIR}/assets/obj/ DESTINATION ${CMAKE_BINARY_DIR}/assets/obj/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/picture/ DESTINATION ${CMAKE_BINARY_DIR}/assets/picture/)
# 链接 GDI 库
target_link_libraries(Rasterizer gdi32)
