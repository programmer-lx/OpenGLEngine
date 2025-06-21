#include "Model.h"

#include "Mesh.h"
#include "Misc/Debug.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

void Model::loadRootNode(const aiScene* scene, aiNode* root, VertexDataType type)
{
    for (int i = 0; i < scene->mNumMeshes; ++i)
    {
        switch (type)
        {
            case VertexDataType::Three:
                loadMesh_3(scene->mMeshes[i]);
                break;

            case VertexDataType::Full:
                loadMesh_Full(scene->mMeshes[i]);
            break;
        }
    }

    // recursive
    // for (int i = 0; i < root->mNumChildren; ++i)
    // {
    //     loadRootNode(scene, root->mChildren[i], type);
    // }
}

void Model::loadMesh_3(aiMesh* mesh)
{
    std::vector<VertexData3> vertexData;
    std::vector<std::uint32_t> indices;

    glm::vec3 v3;
    glm::vec2 v2;

    // vertexData
    for (std::int64_t i = 0; i < mesh->mNumVertices; ++i)
    {
        VertexData3 data{};

        // vertex
        if (mesh->HasPositions())
        {
            v3.x = mesh->mVertices[i].x;
            v3.y = mesh->mVertices[i].y;
            v3.z = mesh->mVertices[i].z;
            data.vertex = v3;
        }

        // normal
        if (mesh->HasNormals())
        {
            v3.x = mesh->mNormals[i].x;
            v3.y = mesh->mNormals[i].y;
            v3.z = mesh->mNormals[i].z;
            data.normal = v3;
        }

        // uv
        if (mesh->mTextureCoords[0])
        {
            v2.x = mesh->mTextureCoords[0][i].x;
            v2.y = mesh->mTextureCoords[0][i].y;
            data.uv = v2;
        }

        // push back
        vertexData.push_back(data);
    }

    // indices
    m_FacesCount += mesh->mNumFaces;
    for (std::int64_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (std::int64_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    m_Meshes.push_back(Mesh::create(vertexData, indices));
}

void Model::loadMesh_Full(aiMesh* mesh)
{
    std::vector<VertexDataFull> vertexData;
    std::vector<std::uint32_t> indices;

    glm::vec3 v3;
    glm::vec2 v2;

    // vertexData
    for (std::int64_t i = 0; i < mesh->mNumVertices; ++i)
    {
        VertexDataFull data{};

        // vertex
        if (mesh->HasPositions())
        {
            v3.x = mesh->mVertices[i].x;
            v3.y = mesh->mVertices[i].y;
            v3.z = mesh->mVertices[i].z;
            data.vertex = v3;
        }

        // normal
        if (mesh->HasNormals())
        {
            v3.x = mesh->mNormals[i].x;
            v3.y = mesh->mNormals[i].y;
            v3.z = mesh->mNormals[i].z;
            data.normal = v3;
        }

        // uv
        if (mesh->mTextureCoords[0])
        {
            v2.x = mesh->mTextureCoords[0][i].x;
            v2.y = mesh->mTextureCoords[0][i].y;
            data.uv = v2;
        }

        // tangent bitangent
        if (mesh->HasTangentsAndBitangents())
        {
            // tangent
            v3.x = mesh->mTangents[i].x;
            v3.y = mesh->mTangents[i].y;
            v3.z = mesh->mTangents[i].z;
            data.tangent = v3;

            // bitangent
            v3.x = mesh->mBitangents[i].x;
            v3.y = mesh->mBitangents[i].y;
            v3.z = mesh->mBitangents[i].z;
            data.bitangent = v3;
        }

        // push back
        vertexData.push_back(data);
    }

    // indices
    m_FacesCount += mesh->mNumFaces;
    for (std::int64_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (std::int64_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    m_Meshes.push_back(Mesh::create(vertexData, indices));
}

const std::vector<std::shared_ptr<Mesh>>& Model::meshes() const
{
    return m_Meshes;
}

std::shared_ptr<Model> Model::create(const std::string& filePath, VertexDataType type)
{
    std::shared_ptr<Model> ptr(new Model);
    ptr->reset(filePath, type);
    return ptr;
}

void Model::reset(const std::string& filePath, VertexDataType type)
{
    m_FacesCount = 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

    // check
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        Debug::print("Failed to load model: `{}`", importer.GetErrorString());
        return;
    }

    loadRootNode(scene, scene->mRootNode, type);
}

void Model::draw()
{
    for (auto& m : m_Meshes)
    {
        m->draw();
    }
}
