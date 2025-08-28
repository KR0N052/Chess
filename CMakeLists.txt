cmake_minimum_required(VERSION 3.10)
project(cpp_crow_add)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(server backend/main.cpp)

target_link_libraries(server PRIVATE ws2_32 mswsock)

target_link_libraries(server PRIVATE pthread ws2_32)
