cmake_minimum_required(VERSION 3.16)
project(oxford_car_visualiser2)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)


include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} glfw GLEW)
include_directories(includes)
include_directories(includes/vendor/glm)

link_directories(/usr/lib/x86_64-linux-gnu)

add_executable(oxford_car_visualiser2 main.cpp src/Shader.cc src/Renderer.cc src/point_cloud_builder.cc)

target_link_libraries(oxford_car_visualiser2 glfw ${OPENGL_LIBRARY} GLEW ${GLUT_LIBRARY})
