#pragma once
#include <map>
#include <assimp/material.h>
#include <assimp/scene.h>

#include "mesh.h"
#include "kEn/renderer/shader.h"

namespace kEn
{
	namespace texture_type
	{
#		define ASSIMP_ENTRY(name, type)	case name: return type;
		inline aiTextureType to_assimp(const texture_type_t type)
		{
			switch (type)
			{
				TEXTURE_TYPES(ASSIMP_ENTRY)
			default:
				return aiTextureType_NONE;
			}
		}
	}

	class model
	{
	public:
		model(const std::filesystem::path& path)
		{
			load_model(path);
		}

		void render(shader& shader);
	private:
		std::vector<mesh> meshes;
		std::filesystem::path directory;
		static std::map<std::filesystem::path, std::shared_ptr<texture2D>> loaded_textures;

		void load_model(const std::filesystem::path& path);
		void process_node(aiNode* node, const aiScene* scene);
		mesh process_mesh(aiMesh* mesh, const aiScene* scene);
		void load_material_textures(aiMaterial* mat, texture_type_t type, std::vector<std::shared_ptr<texture2D>>& textures) const;
	};
}
