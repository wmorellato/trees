# cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

set(Easy3D_DIR ${CMAKE_SOURCE_DIR}/libs/Easy3D/build)
find_package(Easy3D)
include_directories(${EASY3D_INCLUDE_DIRS})

# add main file
add_executable(trees
               main.cpp
               environment.hpp
               window.hpp window.cpp
               tree.hpp tree.cpp
               node.hpp node.cpp
               bud.hpp bud.cpp
               marker.hpp marker.cpp
)

target_include_directories(trees PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(trees
    PRIVATE project_options
            project_warnings
            glm::glm
            glfw
            dl
            yaml-cpp
            spdlog::spdlog
            ${EASY3D_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT})
