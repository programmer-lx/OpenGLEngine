#pragma once

#include "Graphic/BufferObject.h"
#include "Graphic/VertexArrayObject.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vector>
#include <cinttypes>
#include <memory>
#include <string>

enum class MeshFileType
{
    TxtMesh
};

struct VertexData3
{
    glm::vec3 vertex = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    glm::vec2 uv = glm::vec2(0, 0);
};

struct VertexDataFull
{
    glm::vec3 vertex = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    glm::vec2 uv = glm::vec2(0, 0);
    glm::vec3 tangent = glm::vec3(0, 0, 0);
    glm::vec3 bitangent = glm::vec3(0, 0, 0);
};

class Mesh final
{
private:
    std::unique_ptr<VertexArrayObject> m_VAO;
    std::unique_ptr<BufferObject> m_VBO;
    std::unique_ptr<BufferObject> m_EBO;

    std::int32_t m_IndicesCount{};

private:
    void readTxtM(const std::string& filePath, std::vector<float>* outBuffer, std::vector<std::uint32_t>* outIndices);

private:
    Mesh();
public:
    ~Mesh();

    static std::shared_ptr<Mesh> create(const std::string& filePath, MeshFileType type);
    void reset(const std::string& filePath, MeshFileType type);

    static std::shared_ptr<Mesh> create(const std::vector<VertexData3>& vertexData3, const std::vector<std::uint32_t>& indices);
    void reset(const std::vector<VertexData3>& vertexData3, const std::vector<std::uint32_t>& indices);

    static std::shared_ptr<Mesh> create(const std::vector<VertexDataFull>& vertexDataFull, const std::vector<std::uint32_t>& indices);
    void reset(const std::vector<VertexDataFull>& vertexDataFull, const std::vector<std::uint32_t>& indices);

    void setBatchDrawMatrixM(BufferObject* instanceVBO);

    void draw();
    void batchDraw(std::int32_t count);

public:
    std::int32_t indicesCount() const;
    GLenum drawMode = GL_TRIANGLES;
};
