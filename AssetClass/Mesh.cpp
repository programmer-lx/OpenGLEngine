#include "Mesh.h"

#include "Misc/Debug.h"
#include "Misc/File.h"
#include "Misc/StringUtil.h"

void Mesh::readTxtM(const std::filesystem::path& filePath, std::vector<float>* outBuffer, std::vector<std::uint32_t>* outIndices)
{
    File file(filePath, std::ios::in);
    std::string str = file.getString();
    std::vector<std::string> arr = StringUtil::split(str, {',', ':'});
    for (int i = 0; i < arr.size(); ++i)
    {
        arr[i] = StringUtil::trim(arr[i], {' ', '\n'});
    }

    int j;
    // buffer
    for (j = 0; j < arr.size(); ++j)
    {
        if (arr[j] == "index")
            break;

        outBuffer->push_back(std::stof(arr[j]));
    }
    ++j;
    // indices
    for (; j < arr.size(); ++j)
    {
        outIndices->push_back(std::stoi(arr[j]));
    }
}

Mesh::Mesh()
{
    m_VAO = std::make_unique<VertexArrayObject>();
    m_VBO = std::make_unique<BufferObject>(GL_ARRAY_BUFFER);
    m_EBO = std::make_unique<BufferObject>(GL_ELEMENT_ARRAY_BUFFER);
}

std::shared_ptr<Mesh> Mesh::create(const std::filesystem::path& filePath, MeshFileType type)
{
    auto mesh = std::shared_ptr<Mesh>(new Mesh);
    mesh->reset(filePath, type);
    return mesh;
}

std::shared_ptr<Mesh> Mesh::create(const std::vector<VertexData3>& vertexData3, const std::vector<std::uint32_t>& indices)
{
    auto mesh = std::shared_ptr<Mesh>(new Mesh);
    mesh->reset(vertexData3, indices);
    return mesh;
}

void Mesh::reset(const std::vector<VertexData3>& vertexData3, const std::vector<std::uint32_t>& indices)
{
    m_VAO->bind();
    m_VBO->bind();
    m_EBO->bind();

    m_VBO->resetData(sizeof(VertexData3) * vertexData3.size(), vertexData3.data());
    m_EBO->resetData(sizeof(std::uint32_t) * indices.size(), indices.data());
    m_IndicesCount = indices.size();

    // vertex
    m_VAO->setLayoutAttribute(0, 3, sizeof(VertexData3), offsetof(VertexData3, vertex));
    // normal
    m_VAO->setLayoutAttribute(1, 3, sizeof(VertexData3), offsetof(VertexData3, normal));
    // uv
    m_VAO->setLayoutAttribute(2, 2, sizeof(VertexData3), offsetof(VertexData3, uv));

    m_VAO->unbind();
    m_VBO->unbind();
    m_EBO->unbind();
}

std::shared_ptr<Mesh> Mesh::create(const std::vector<VertexDataFull>& vertexDataFull, const std::vector<std::uint32_t>& indices)
{
    auto mesh = std::shared_ptr<Mesh>(new Mesh);
    mesh->reset(vertexDataFull, indices);
    return mesh;
}

Mesh::~Mesh()
{
    Debug::print("Mesh destruct");
}

void Mesh::reset(const std::filesystem::path& filePath, MeshFileType type)
{
    std::vector<float> buffer;
    std::vector<std::uint32_t> indices;

    switch (type)
    {
        case MeshFileType::TxtMesh:
            readTxtM(filePath, &buffer, &indices);
            break;
    }

    m_VAO->bind();
    m_VBO->bind();
    m_EBO->bind();

    m_VBO->resetData(sizeof(float) * buffer.size(), buffer.data());
    m_EBO->resetData(sizeof(std::uint32_t) * indices.size(), indices.data());
    m_IndicesCount = indices.size();

    // vertex
    m_VAO->setLayoutAttribute(0, 3, 8 * sizeof(float), 0);
    // normal
    m_VAO->setLayoutAttribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
    // uv
    m_VAO->setLayoutAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));

    m_VAO->unbind();
    m_VBO->unbind();
    m_EBO->unbind();
}

void Mesh::reset(const std::vector<VertexDataFull>& vertexDataFull, const std::vector<std::uint32_t>& indices)
{
    m_VAO->bind();
    m_VBO->bind();
    m_EBO->bind();

    // buffer data
    m_VBO->resetData(sizeof(VertexDataFull) * vertexDataFull.size(), vertexDataFull.data());
    m_EBO->resetData(sizeof(std::uint32_t) * indices.size(), indices.data());
    m_IndicesCount = indices.size();

    // vertex
    m_VAO->setLayoutAttribute(0, 3, sizeof(VertexDataFull), offsetof(VertexDataFull, vertex));
    // normal
    m_VAO->setLayoutAttribute(1, 3, sizeof(VertexDataFull), offsetof(VertexDataFull, normal));
    // uv
    m_VAO->setLayoutAttribute(2, 2, sizeof(VertexDataFull), offsetof(VertexDataFull, uv));
    // tangent
    m_VAO->setLayoutAttribute(3, 3, sizeof(VertexDataFull), offsetof(VertexDataFull, tangent));
    // bitangent
    m_VAO->setLayoutAttribute(4, 3, sizeof(VertexDataFull), offsetof(VertexDataFull, bitangent));

    m_VAO->unbind();
    m_VBO->unbind();
    m_EBO->unbind();
}

void Mesh::setBatchDrawMatrixM(BufferObject* instanceVBO)
{
    // layout 7, 8, 9, 10 : instanceMatrixM
    m_VAO->bind();
    m_VAO->setupBatchDrawMatrixM(7, instanceVBO);
    m_VAO->unbind();
}

void Mesh::draw()
{
    m_VAO->bind();
    glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::batchDraw(std::int32_t count)
{
    m_VAO->bind();
    glDrawElementsInstanced(drawMode, m_IndicesCount, GL_UNSIGNED_INT, nullptr, count);
}

std::int32_t Mesh::indicesCount() const
{
    return m_IndicesCount;
}
