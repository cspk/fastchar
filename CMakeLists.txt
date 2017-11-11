project("fastchar")
cmake_minimum_required(VERSION 3.2)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.c")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -pedantic")

add_executable(${PROJECT_NAME} ${SOURCES})
