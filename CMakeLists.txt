cmake_minimum_required(VERSION 3.10)

# project name and version
project(trees VERSION 1.0)

# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

# include dirs
include_directories(include)

# sdl2
find_package(SDL2 REQUIRED)
include_directories(trees ${SDL2_INCLUDE_DIRS})

# glfw
find_package(glfw3 3.3 REQUIRED)

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# add main file
add_executable(trees
               include/obj/obj.c
               src/main.cpp
               src/window.hpp src/window.cpp
               src/GLDemonstration.hpp src/GLFundamentals.hpp
)

# replace config file
configure_file(src/config.hpp.in config.hpp)

target_include_directories(trees PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(trees glfw)
target_link_libraries(trees ${GLEW_LIBRARIES})
target_link_libraries(trees ${OPENGL_LIBRARIES})

# sdl
# set(SDL2_INCLUDE_DIR /usr/include/SDL2)
# set(SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0)
# include_directories(/usr/include/SDL2)
# target_link_libraries(trees SDL2)

# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(trees ${SDL2_LIBRARIES})
