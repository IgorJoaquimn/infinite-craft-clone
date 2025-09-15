cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "infinite-craft-clone")
project(${PROJECT_NAME})

# --- Find required packages ---
find_package(SDL2 2.30.0 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# --- Add GLM using FetchContent ---
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# --- Collect source files ---
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Game/Game.cpp
    ${SRC_DIR}/Shader/Shader.cpp
    ${SRC_DIR}/Component/TextComponent.cpp
    ${SRC_DIR}/Core/Window.cpp
    ${SRC_DIR}/Core/Renderer.cpp
    ${SRC_DIR}/Core/InputManager.cpp
    ${SRC_DIR}/Core/ResourceManager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# --- Include directories ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# --- Link all necessary libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    GLEW::glew
    OpenGL::GL
    glm
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Post-build commands and asset copying ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
