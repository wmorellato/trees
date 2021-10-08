#include "config.hpp"
#include "spdlog/spdlog.h"
#include "window.hpp"
#include <iostream>
#include <string>
#include <random>

int main(int argc, char* argv[]) {
  srand(time(0));
  spdlog::set_pattern("[%c] %-8l %v");

  if (argc < 2) {
    std::cout << "Usage: trees <iterations>" << std::endl;
    return 1;
  }

  app::Window w;
  std::cout << "Running " << argv[1] << " iterations" << std::endl;
  return w.run(std::stoi(argv[1]));
}
