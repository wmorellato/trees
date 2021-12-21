#ifndef TREES_SRC_ENVIRONMENT_H
#define TREES_SRC_ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include <glm/vec3.hpp>

using namespace std;

namespace environment
{

inline YAML::Node config = YAML::LoadFile("<path_to_config>/config.yaml");

inline const float qbase_max = config["qbase_max"].as<float>(150.0);
inline const int num_markers = config["num_markers"].as<int>(10);
inline const float r = config["perception_radius"].as<float>(4.0);
inline const float theta = config["perception_angle"].as<float>(90.0);
inline const float ro = config["occupancy_radius"].as<float>(2.0);
inline const float alpha = config["coeff_proportionality"].as<float>(2.0);
inline const float lambda = config["coeff_resource_alloc"].as<float>(0.5);
inline const float voxel_size = config["voxel_size"].as<float>(2.0);
inline const int qmax = config["qmax"].as<int>(3);

inline glm::vec3 tropism() {
  float x = config["v_tropism"][0].as<float>(0.0);
  float y = config["v_tropism"][1].as<float>(1.0);
  float z = config["v_tropism"][2].as<float>(0.0);

  return glm::vec3(x, y, z);
}


class ShadowVoxel {
  private:
  public:
    float value = 0.0f;
};

class ShadowGrid {
  private:
  public:
    ShadowGrid(float _voxel_size, glm::vec3 _center);
    ShadowVoxel& getVoxelAtPosition(glm::vec3 position);
    void propagateShadow(glm::vec3 position);
    
    float voxel_size;
    glm::vec3 center;
    std::unordered_map<std::string, ShadowVoxel> grid;
};

} // namespace environment

#endif
