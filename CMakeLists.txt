cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "infinite-craft-clone")
project(${PROJECT_NAME})

find_package(SDL2 2.32.2 EXACT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/Game.cpp src/Game.hpp src/Shader.cpp src/Shader.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 GLEW::glew OpenGL::GL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)
