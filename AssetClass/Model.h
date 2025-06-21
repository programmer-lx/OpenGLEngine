#pragma once

#include <assimp/scene.h>

#include <memory>
#include <vector>
#include <string>


enum class VertexDataType
{
    Three,
    Full
};

class Mesh;

class Model
{
private:
    std::vector<std::shared_ptr<Mesh>> m_Meshes;
    std::uint32_t m_FacesCount = 0;

private:
    Model() = default;

    void loadRootNode(const aiScene* scene, aiNode* root, VertexDataType type);

    void loadMesh_3(aiMesh* mesh);
    void loadMesh_Full(aiMesh* mesh);

public:
    const std::vector<std::shared_ptr<Mesh>>& meshes() const;

public:
    static std::shared_ptr<Model> create(const std::string& filePath, VertexDataType type);

    void reset(const std::string& filePath, VertexDataType type);

    void draw();
};
