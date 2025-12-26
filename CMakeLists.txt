cmake_minimum_required(VERSION 3.10)
project(mnet_development C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(mnet)

add_executable(mnet_development main.c)
target_link_libraries(mnet_development mnet)