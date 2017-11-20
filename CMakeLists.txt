project("fastchar")
cmake_minimum_required(VERSION 3.2)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.c")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -pedantic")
add_definitions("-DPROJECT_NAME=\"${PROJECT_NAME}\"")

find_package(PkgConfig REQUIRED)
pkg_check_modules(MODULES REQUIRED "gtk+-3.0" "x11" "libfakekey")
include_directories(${MODULES_INCLUDE_DIRS})
link_directories(${MODULES_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${MODULES_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")
