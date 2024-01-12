#include "kenpch.h"
#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace kEn
{
	std::unordered_map<std::filesystem::path, std::shared_ptr<model>> model::loaded_resources_;

	void model::render(shader& shader)
	{
		for (auto& mesh : meshes)
			mesh.render(shader);
	}

	void model::load_model(const std::filesystem::path& path, const texture_spec& spec)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenNormals);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			KEN_ERROR("ASSIMP: {}", importer.GetErrorString());
			return;
		}

		directory = path.parent_path();

		process_node(scene->mRootNode, scene, spec);
	}

	void model::process_node(aiNode* node, const aiScene* scene, const texture_spec& spec)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(process_mesh(mesh, scene, spec));
		}
	
		for(unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			process_node(node->mChildren[i], scene, spec);
		}
	}

	mesh model::process_mesh(aiMesh* mesh, const aiScene* scene, const texture_spec& spec)
	{
		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<texture2D>> textures;

		// Vertices
		for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertex v;
			glm::vec3 vector;
	
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			v.pos = vector;
	
			if(mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				v.normal = vector;
			}
	
			if(mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
	
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				v.texture_coord = vec;
			}
			else
			{
				v.texture_coord = glm::vec2(0.0f);
			}

			//TODO: tangents and bitangents

			vertices.emplace_back(v);
		}

		// Indices
		for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// Texture
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		load_material_textures(material, kEn::texture_type::diffuse, textures, spec);
		// TODO: other types

		return { vertices, indices, textures };
	}

	void model::load_material_textures(aiMaterial* mat, const texture_type_t type, std::vector<std::shared_ptr<texture2D>>& textures, const texture_spec& spec) const
	{
		aiTextureType ai_type = texture_type::to_assimp(type);
		for(unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i)
		{
			aiString str;
			mat->GetTexture(ai_type, i, &str);

			auto path = directory / str.C_Str();
			std::shared_ptr<texture2D> texture = texture2D::create(path, spec);
			texture->set_type(type);
			textures.push_back(texture);
		}
	}


}