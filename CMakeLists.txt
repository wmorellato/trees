cmake_minimum_required(VERSION 3.10)

# project name and version
project(trees VERSION 1.0)

# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

# include dirs
include_directories(include)
find_package(glfw3 3.3 REQUIRED)
find_package(Threads REQUIRED)

# easy3d
set(Easy3D_DIR /home/wes/repos/Easy3D/build)
find_package(Easy3D REQUIRED)
include_directories(${EASY3D_INCLUDE_DIRS})

# add main file
add_executable(trees
               src/main.cpp
               src/environment.hpp
               src/window.hpp src/window.cpp
               src/tree.hpp src/tree.cpp
               src/node.hpp src/node.cpp
               src/bud.hpp src/bud.cpp
               src/marker.hpp src/marker.cpp
)

# replace config file
configure_file(src/config.hpp.in config.hpp)

target_include_directories(trees PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(trees glfw)
target_link_libraries(trees dl)
target_link_libraries(trees ${EASY3D_LIBRARIES})
target_link_libraries(trees ${CMAKE_SOURCE_DIR}/libs/libspdlog.a)
target_link_libraries(trees ${CMAKE_SOURCE_DIR}/libs/libyaml-cpp.a)
target_link_libraries(trees ${CMAKE_THREAD_LIBS_INIT})
