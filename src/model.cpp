#include "model.h"

#include "log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& path)
{
	Assimp::Importer importer;
	const auto* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Assimp error: {}", importer.GetErrorString());
		return;
	}

	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		auto* aiMesh = scene->mMeshes[i];
		for (uint32_t j = 0; j < aiMesh->mNumVertices; j++)
		{
			Vertex vertex{};
			vertex.position = { aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z };
			vertex.normal = { aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z };
			if (aiMesh->mTextureCoords[0])
			{
				vertex.texCoords = { aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y };
			}
			else
			{
				vertex.texCoords = { 0.0f, 0.0f };
			}
			vertices.push_back(vertex);
		}

		for (uint32_t j = 0; j < aiMesh->mNumFaces; j++)
		{
			auto& face = aiMesh->mFaces[j];
			for (uint32_t k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}

		std::vector<std::shared_ptr<Texture>> textures;
		if (aiMesh->mMaterialIndex >= 0)
		{
			auto* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];
			for (uint32_t j = 0; j < aiMaterial->GetTextureCount(aiTextureType_DIFFUSE); j++)
			{
				aiString str;
				aiMaterial->GetTexture(aiTextureType_DIFFUSE, j, &str);
				std::string dirPath = path.substr(0, path.find_last_of('/'));
				std::string texturePath = dirPath + '/' + str.C_Str();
				textures.push_back(Texture::create(TextureType::Diffuse, TextureFormat::Rgba, texturePath));
			}
			for (uint32_t j = 0; j < aiMaterial->GetTextureCount(aiTextureType_SPECULAR); j++)
			{
				aiString str;
				aiMaterial->GetTexture(aiTextureType_SPECULAR, j, &str);
				std::string dirPath = path.substr(0, path.find_last_of('/'));
				std::string texturePath = dirPath + '/' + str.C_Str();
				textures.push_back(Texture::create(TextureType::Specular, TextureFormat::Rgba, texturePath));
			}
		}

		m_meshes.push_back(Mesh::create(vertices, indices, textures));
	}
}

Model::~Model()
{
}

std::shared_ptr<Model> Model::create(const std::string& path)
{
	return std::make_shared<Model>(path);
}
