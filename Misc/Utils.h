#pragma once

#include "Core/Transform.h"

#include <vector>
#include <string>
#include <memory>
#include <filesystem>

class Mesh;

class Utils
{
public:
    static std::vector<Transform> decodeSceneDataFile(const std::filesystem::path& filePath);
    static std::shared_ptr<Mesh> genPlane(float xLength, float zLength, std::uint32_t xSegment, std::uint32_t zSegment);
};
