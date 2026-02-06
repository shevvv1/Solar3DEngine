cmake_minimum_required(VERSION 3.5.0)
project(3DEng VERSION 0.1.0)

set(CMAKE_CXX_SYANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_subdirectory(libs/SDL3)
add_subdirectory(libs/glm)
add_subdirectory(libs/assimp)

file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp" "src/external/glad/src/glad.c")

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC
     libs/SDL3/include
     libs/assimp/include
     src/external/glad/include
     src/
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        SDL3::SDL3
        glm
        assimp
)
