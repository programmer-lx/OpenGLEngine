#include "Utils.h"

#include "StringUtil.h"
#include "File.h"
#include "AssetClass/Mesh.h"
#include "Misc/Math.h"

#include <regex>

std::vector<Transform> Utils::decodeSceneDataFile(const std::string& filePath)
{
    File file(filePath, std::ios::in);
    if (!file.isValid())
        return {};

    std::string txt = file.getString();

    std::regex pattern(R"([\s\n]+)");
    std::smatch match;
    std::sregex_token_iterator it(txt.begin(), txt.end(), pattern, -1);
    std::vector<std::string> strs(it, std::sregex_token_iterator());

    std::vector<Transform> result;

    for (std::int64_t i = 0; i < strs.size();)
    {
        // position
        glm::vec3 position;
        position.x = std::stof(strs[i++]);
        position.y = std::stof(strs[i++]);
        position.z = std::stof(strs[i++]);

        // rotation
        glm::quat rotation;
        rotation.x = std::stof(strs[i++]);
        rotation.y = std::stof(strs[i++]);
        rotation.z = std::stof(strs[i++]);
        rotation.w = std::stof(strs[i++]);

        // scale
        glm::vec3 scale;
        scale.x = std::stof(strs[i++]);
        scale.y = std::stof(strs[i++]);
        scale.z = std::stof(strs[i++]);

        result.emplace_back(position, rotation, scale);
    }

    return result;
}

std::shared_ptr<Mesh> Utils::genPlane(float xLength, float zLength, std::uint32_t xSegment, std::uint32_t zSegment)
{
    if (xSegment % 2 != 0)
        xSegment++;
    if (zSegment % 2 != 0)
        zSegment++;

    float stepX = xLength / (float)xSegment;
    float stepZ = zLength / (float)zSegment;
    float uvStepX = 1.0f / (float)xSegment;
    float uvStepZ = 1.0f / (float)zSegment;

    // vertices
    std::vector<VertexData3> vertices;
    for (std::uint32_t z = 0; z <= zSegment; ++z)
    {
        for (std::uint32_t x = 0; x <= xSegment; ++x)
        {
            // noise
            glm::vec2 noise = {
                z % 2 == 0 ? 0.1f : -0.1f,
                x % 2 != 0 ? -0.06f : 0.06f
            };

            glm::vec3 vertex = {
                (float)x * (stepX + noise.x) - xLength * 0.5f,
                0,
                (float)z * (stepZ + noise.y) - zLength * 0.5f
            };
            glm::vec3 normal = {0, 1, 0};
            glm::vec2 uv = {
                (float)x * (uvStepX + noise.x),
                (float)z * (uvStepZ + noise.y)
            };

            vertices.emplace_back(vertex, normal, uv);
        }
    }

    // indices
    std::vector<std::uint32_t> indices;
    for (std::uint32_t z = 0; z < zSegment; ++z)
    {
        for (std::uint32_t x = 0; x < xSegment; ++x)
        {
            std::uint32_t topLeft = z * (xSegment + 1) + x;
            std::uint32_t topRight = topLeft + 1;
            std::uint32_t bottomLeft = (z + 1) * (xSegment + 1) + x;
            std::uint32_t bottomRight = bottomLeft + 1;

            indices.emplace_back(topLeft);
            indices.emplace_back(bottomLeft);
            indices.emplace_back(bottomRight);

            indices.emplace_back(topLeft);
            indices.emplace_back(bottomRight);
            indices.emplace_back(topRight);
        }
    }

    return Mesh::create(vertices, indices);
}
