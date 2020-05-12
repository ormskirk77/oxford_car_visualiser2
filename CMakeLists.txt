cmake_minimum_required(VERSION 3.16)
project(oxford_car_visualiser2)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)

include_directories(includes glfw GLEW)
include_directories(/usr/local/Cellar/glfw/3.3.2/include)
include_directories(includes/vendor/glm)

link_directories(/usr/local/Cellar/glew/2.1.0_1/lib)

add_executable(oxford_car_visualiser2 main.cpp src/Shader.cc src/Renderer.cc src/point_cloud_builder.cc)

target_link_libraries(oxford_car_visualiser2 glfw ${OPENGL_gl_LIBRARY} GLEW)