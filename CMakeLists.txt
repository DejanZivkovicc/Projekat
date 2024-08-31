cmake_minimum_required(VERSION 3.25)
project(Projekat)

include_directories(include/)
find_package(OpenGL REQUIRED)

add_subdirectory(libs/glad)
set(CMAKE_CXX_STANDARD 17)

add_executable(Projekat src/main.cpp src/Shader.cpp include/Shader.h src/Camera.cpp include/Camera.h src/stb_image.cpp src/Model.cpp include/Model.h src/Mesh.cpp include/Mesh.h)
target_link_libraries(Projekat glfw glad OpenGL::GL Xrandr pthread Xi X11 dl glfw3 GL assimp)