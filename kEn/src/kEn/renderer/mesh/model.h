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
		explicit model(const std::filesystem::path& path, const texture_spec& spec = texture_spec())
		{
			load_model(path, spec);
		}

		void render(shader& shader, const transform& transform) const;

	private:
		std::vector<mesh> meshes;
		std::filesystem::path directory;

		void load_model(const std::filesystem::path& path, const texture_spec& spec);
		void process_node(aiNode* node, const aiScene* scene, const texture_spec& spec);
		mesh process_mesh(aiMesh* mesh, const aiScene* scene, const texture_spec& spec);
		void load_material_textures(aiMaterial* mat, const texture_type_t type, kEn::material& material, const texture_spec& spec) const;

	public:
		static const std::filesystem::path texture_path;

		static std::shared_ptr<model> load(const std::filesystem::path& path, const texture_spec& spec = texture_spec());

	private:
		static std::unordered_map<std::filesystem::path, std::shared_ptr<model>> loaded_resources_;
	};
}
