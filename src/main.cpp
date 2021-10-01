#include "config.hpp"
#include "spdlog/spdlog.h"
#include "window.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  spdlog::set_pattern("[%c] %-8l %v");

  app::Window w;
  return w.run();
}
