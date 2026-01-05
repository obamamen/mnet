cmake_minimum_required(VERSION 3.10)
project(mnet C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(mnet_development main.c)

add_subdirectory(mnet)
target_link_libraries(mnet_development mnet)
