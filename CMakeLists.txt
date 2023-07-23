cmake_minimum_required(VERSION 3.24)

# variables
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME openglCourse)

# project setup
project(${PROJECT_NAME})

# dependencies
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/mesh/mesh.cpp src/mesh/mesh.h)

# linking dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
