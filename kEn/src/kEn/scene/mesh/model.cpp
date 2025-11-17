#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <imgui/imgui.h>

#include <assimp/Importer.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/scene/mesh/model.hpp>
#include <kenpch.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>

namespace kEn {
std::unordered_map<std::filesystem::path, std::shared_ptr<Model>> Model::loaded_resources_;
const std::filesystem::path Model::kModelPath("assets/models");

std::shared_ptr<Model> Model::load(const std::filesystem::path& path, const TextureSpec& spec) {
  if (const auto it = loaded_resources_.find(path); it != loaded_resources_.end()) {
    return it->second;
  }

  return loaded_resources_[path] = std::make_shared<Model>(path, spec);
}

void Model::render(Shader& shader, const Transform& transform) const {
  for (const auto& mesh : meshes_) {
    mesh.render(shader, transform);
  }
}

void Model::load_model(const std::filesystem::path& path, const TextureSpec& spec) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    KEN_ERROR("ASSIMP: {}", importer.GetErrorString());
    return;
  }

  directory_ = path.parent_path();

  process_node(scene->mRootNode, scene, spec);
}

void Model::process_node(aiNode* node, const aiScene* scene, const TextureSpec& spec) {
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh   = scene->mMeshes[node->mMeshes[i]];
    Mesh processed = process_mesh(mesh, scene, spec);
    if (processed.material.transparent) {
      meshes_.push_back(std::move(processed));
    } else {
      meshes_.push_front(std::move(processed));
    }
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    process_node(node->mChildren[i], scene, spec);
  }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene, TextureSpec spec) {
  // Vertices
  std::vector<Vertex> vertices;
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    Vertex v{};
    mEn::Vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    v.pos    = vector;

    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      v.normal = vector;
    }

    if (mesh->mTextureCoords[0]) {
      mEn::Vec2 vec;

      vec.x           = mesh->mTextureCoords[0][i].x;
      vec.y           = mesh->mTextureCoords[0][i].y;
      v.texture_coord = vec;
    } else {
      v.texture_coord = mEn::Vec2(0.0F);
    }

    // TODO(): tangents and bitangents

    vertices.emplace_back(v);
  }

  // Indices
  std::vector<uint32_t> indices;
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Material

  aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

  kEn::Material material;
  aiColor3D color;
  mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
  material.ambient_factor = color.r;
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  material.diffuse_factor = color.r;
  mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
  material.specular_factor = color.r;
  mat->Get(AI_MATKEY_SHININESS, material.shininess_factor);
  material.shininess_factor = std::max(material.shininess_factor, 1.F);
  mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  material.emissive = color.r > 0;

  if (mat->GetTextureCount(aiTextureType_OPACITY) > 0) {
    spec.mipmap_levels   = 1;
    material.transparent = true;
  }

  load_material_textures(mat, kEn::texture_type::diffuse, material, spec);
  // TODO(): other types

  return {mesh->mName.C_Str(), vertices, indices, material};
}

void Model::load_material_textures(aiMaterial* mat, const texture_type_t type, kEn::Material& material,
                                   const TextureSpec& spec) const {
  aiTextureType ai_type = texture_type::to_assimp(type);
  for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i) {
    aiString str;
    mat->GetTexture(ai_type, i, &str);

    auto path = directory_ / str.C_Str();

    std::shared_ptr<Texture2D> texture = Texture2D::create(absolute(path), spec);
    material.set_texture(type, texture, i);
  }
}

void Model::imgui() {
  if (ImGui::TreeNode("Meshes")) {
    ImGui::BeginChild("Meshes", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
    for (size_t i = 0; i < meshes_.size(); ++i) {
      ImGui::PushID(static_cast<int>(i));
      meshes_[i].imgui();
      ImGui::PopID();
    }
    ImGui::EndChild();
    ImGui::TreePop();
  }
}
}  // namespace kEn
