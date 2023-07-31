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
find_package(freetype-gl CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/mesh/mesh.cpp src/mesh/mesh.h src/shader/shader.cpp src/shader/shader.h src/window/window.cpp src/window/window.h src/camera/camera.cpp src/camera/camera.h src/text/text.cpp src/text/text.h src/texture/texture.cpp src/texture/texture.h src/light/light.cpp src/light/light.h src/material/material.cpp src/material/material.h src/light/directional-light.cpp src/light/directional-light.h src/light/point-light.cpp src/light/point-light.h src/shared/common-values.h src/light/spot-light.cpp src/light/spot-light.h src/model/model.cpp src/model/model.h)

# linking dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype-gl)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

# post build copying of essential files
add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
        ${CMAKE_CURRENT_BINARY_DIR}/fonts)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures)
