cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME openglCourse)

project(${PROJECT_NAME})

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
