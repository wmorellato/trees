#ifndef TREES_SRC_ENVIRONMENT_H
#define TREES_SRC_ENVIRONMENT_H

#include "yaml-cpp/yaml.h"
#include <glm/vec3.hpp>

namespace environment
{

inline YAML::Node config = YAML::LoadFile("/home/wes/repos/trees/build/config.yaml");

inline const int qbase_max = config["qbase_max"].as<int>(150);
inline const int num_markers = config["num_markers"].as<int>(10);
inline const float r = config["perception_radius"].as<float>(4.0);
inline const float theta = config["perception_angle"].as<float>(90.0);
inline const float ro = config["occupancy_radius"].as<float>(2.0);
inline const float alpha = config["coeff_proportionality"].as<float>(2.0);
inline const float lambda = config["coeff_resource_alloc"].as<float>(0.5);

inline glm::vec3 tropism() {
  float x = config["v_tropism"][0].as<float>(0.0);
  float y = config["v_tropism"][1].as<float>(1.0);
  float z = config["v_tropism"][2].as<float>(0.0);

  return glm::vec3(x, y, z);
};

} // namespace environment

#endif
