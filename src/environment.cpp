#include "environment.hpp"
#include "spdlog/spdlog.h"
#include <cmath>

using namespace environment;

ShadowGrid::ShadowGrid(float _voxel_size, glm::vec3 _center)
    : voxel_size(_voxel_size), center(_center) {}

ShadowVoxel& ShadowGrid::getVoxelAtPosition(glm::vec3 position) {
    int x = static_cast<int>(ceilf((position.x - center.x)/voxel_size));
    int y = static_cast<int>(ceilf((position.y - center.y)/voxel_size));
    int z = static_cast<int>(ceilf((position.z - center.z)/voxel_size));

    std::string key = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);

    if (this->grid.find(key) != this->grid.end()) {
        ShadowVoxel& voxel = this->grid[key];
        // spdlog::info("Shadow value for ({},{},{}) (\"{}\") = {}", x, y, z, key, voxel.value);
    } else {
        ShadowVoxel* new_voxel = new ShadowVoxel();
        // this is not memory safe, should use unique_ptr and
        // move the object ownership to the map
        spdlog::info("created");
        this->grid[key] = *new_voxel;
    }

    return this->grid[key];
}

void ShadowGrid::propagateShadow(glm::vec3 position) {
    // duplicate code
    int x = static_cast<int>(ceilf((position.x - center.x)/voxel_size));
    int y = static_cast<int>(ceilf((position.y - center.y)/voxel_size));
    int z = static_cast<int>(ceilf((position.z - center.z)/voxel_size));

    // spdlog::info("Pos: {}, Voxel: {},{},{}", pv3(position), x, y, z);

    for (int q = 0; q < qmax; q++) {
        float a = 1.0f;
        float b = 2.0f;
        float shadow_value = powf(a * b, static_cast<float>(-q));
        
        for (int i = x - q; i < x + q; i++) {
            for (int k = z - q; k < z + q; k++) {
                ShadowVoxel& voxel = getVoxelAtPosition(glm::vec3(i, y - q, k));
                voxel.value += shadow_value;
            }
        }
    }
}