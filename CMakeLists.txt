cmake_minimum_required(VERSION 3.10)
project(mnet C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SOURCES
        "main.c")

add_subdirectory(mnet)

function(add_dev_executable TARGET_NAME DEFINE)
    add_executable(${TARGET_NAME} ${SOURCES})
    target_compile_definitions(${TARGET_NAME} PRIVATE ${DEFINE})
    target_link_libraries(${TARGET_NAME} mnet)
endfunction()


add_dev_executable(mnet_development DEV_MAIN)
add_dev_executable(mnet_development_client DEV_CLIENT)
add_dev_executable(mnet_development_server DEV_SERVER)
