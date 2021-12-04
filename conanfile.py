from conans import ConanFile, tools


class Trees(ConanFile):
    name = "Trees"
    version = "0.1"
    requires = (
        "glm/0.9.9.8",
        "glfw/3.3.5",
        "catch2/2.13.7",
        "docopt.cpp/0.6.2",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "yaml-cpp/0.7.0",
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
