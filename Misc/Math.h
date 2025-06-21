#pragma once

#include "AssetClass/Mesh.h"

#include <glm/vec3.hpp>
#include <cmath>
#include <random>

class Math
{
public:
    static void setPrecision(float& num, int precision)
    {
        const float p = std::pow(10, precision);
        num = static_cast<std::int64_t>(num * static_cast<std::int64_t>(p)) / p;
    }

    static void setPrecision(glm::vec3& vec, int precision)
    {
        setPrecision(vec.x, precision);
        setPrecision(vec.y, precision);
        setPrecision(vec.z, precision);
    }

    /**
     * @param childPos 物体坐标在父空间的表示
     * @param childX 子空间X轴在父空间的表示
     * @param childY 子空间Y轴在父空间的表示
     * @param childZ 子空间Z轴在父空间的表示
     * @param scale 物体在子空间下的缩放
     * @return 子空间到父空间的变换矩阵
     */
    static glm::mat4x4 childToParentSpace(const glm::vec3& childPos,
                                          const glm::vec3& childX,
                                          const glm::vec3& childY,
                                          const glm::vec3& childZ,
                                          const glm::vec3& scale)
    {
        glm::mat4x4 result(1);

        // 由于 OpenGL 的矩阵存储规则是列主序，所以行列要颠倒
        result[0][0] = childX.x * scale.x;
        result[0][1] = childX.y * scale.x;
        result[0][2] = childX.z * scale.x;

        result[1][0] = childY.x * scale.y;
        result[1][1] = childY.y * scale.y;
        result[1][2] = childY.z * scale.y;

        result[2][0] = childZ.x * scale.z;
        result[2][1] = childZ.y * scale.z;
        result[2][2] = childZ.z * scale.z;

        result[3][0] = childPos.x;
        result[3][1] = childPos.y;
        result[3][2] = childPos.z;

        return result;
    }

    static glm::mat4x4 modelMatrix(const glm::vec3& worldPosition, const Transform& transform)
    {
        return childToParentSpace(worldPosition, transform.right(), transform.up(), -transform.forward(), transform.scale());
    }

    /**
     * @tparam T floating point value
     * @return uniform random value [min, max)
     */
    template<typename T>
        requires std::is_floating_point_v<T>
    static T uniformRandom(T min, T max)
    {
        std::mt19937 engine(std::random_device{}());
        std::uniform_real_distribution<T> dis(min, max);
        return dis(engine);
    }

    static void genSphereVertices(std::vector<VertexData3>& outVertices, std::vector<std::uint32_t>& outIndices)
    {
        // float radius = 1.0f;
        // uint32_t sectorCount = 36;
        // uint32_t stackCount = 18;
        //
        // outVertices.clear();
        // outIndices.clear();
        //
        // const float PI = glm::pi<float>();
        //
        // for (uint32_t i = 0; i <= stackCount; ++i)
        // {
        //     float stackAngle = PI / 2 - i * (PI / stackCount); // 从 pi/2 到 -pi/2
        //     float xy = radius * cosf(stackAngle);              // r * cos(u)
        //     float z = radius * sinf(stackAngle);               // r * sin(u)
        //
        //     for (uint32_t j = 0; j <= sectorCount; ++j)
        //     {
        //         float sectorAngle = j * (2 * PI / sectorCount); // 从 0 到 2pi
        //
        //         float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
        //         float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
        //
        //         glm::vec3 position(x, y, z);
        //         glm::vec3 normal = glm::normalize(position);
        //         glm::vec2 uv((float)j / sectorCount, (float)i / stackCount);
        //
        //         VertexData3 vertex;
        //         vertex.vertex = position;
        //         vertex.normal = normal;
        //         vertex.uv = uv;
        //         outVertices.push_back(vertex);
        //     }
        // }
        //
        // // 生成索引
        // for (uint32_t i = 0; i < stackCount; ++i)
        // {
        //     uint32_t k1 = i * (sectorCount + 1); // 当前层起始点
        //     uint32_t k2 = k1 + sectorCount + 1;  // 下一层起始点
        //
        //     for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
        //     {
        //         if (i != 0)
        //         {
        //             outIndices.push_back(k1);
        //             outIndices.push_back(k2);
        //             outIndices.push_back(k1 + 1);
        //         }
        //
        //         if (i != (stackCount - 1))
        //         {
        //             outIndices.push_back(k1 + 1);
        //             outIndices.push_back(k2);
        //             outIndices.push_back(k2 + 1);
        //         }
        //     }
        // }

        // std::vector<glm::vec3> positions;
        // std::vector<glm::vec2> uv;
        // std::vector<glm::vec3> normals;
        // std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;

        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                // positions.push_back(glm::vec3(xPos, yPos, zPos));
                // uv.push_back(glm::vec2(xSegment, ySegment));
                // normals.push_back(glm::normalize(glm::vec3(xPos, yPos, zPos)));
                VertexData3 data;
                data.vertex = glm::vec3(xPos, yPos, zPos);
                data.normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
                data.uv = glm::vec2(xSegment, ySegment);
                outVertices.push_back(data);
            }
        }

        // 使用 GL_TRIANGLES，正常地拼三角面
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x < X_SEGMENTS; ++x)
            {
                unsigned int i0 = y * (X_SEGMENTS + 1) + x;
                unsigned int i1 = (y + 1) * (X_SEGMENTS + 1) + x;
                unsigned int i2 = (y + 1) * (X_SEGMENTS + 1) + (x + 1);
                unsigned int i3 = y * (X_SEGMENTS + 1) + (x + 1);

                // 每个小四边形由两个三角形组成
                outIndices.push_back(i0);
                outIndices.push_back(i1);
                outIndices.push_back(i2);

                outIndices.push_back(i0);
                outIndices.push_back(i2);
                outIndices.push_back(i3);
            }
        }
    }
};
