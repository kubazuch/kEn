#include "model.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include <mEn/functions/geometric.hpp>
#include <mEn/fwd.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>

#include <kEn/core/application.hpp>
#include <kEn/core/log.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

#include <assimp/Importer.hpp>

extern "C" {
#include <vendor/mikktspace.h>
}

namespace {

void load_material_textures(aiMaterial* mat, kEn::Material& material, const kEn::SamplerDesc& sampler,
                            const std::filesystem::path& directory, std::uint32_t mip_levels) {
  static constexpr std::pair<kEn::TextureType, aiTextureType> kTypes[] = {
      {kEn::TextureType::Diffuse, aiTextureType_DIFFUSE},
      {kEn::TextureType::Height, aiTextureType_HEIGHT},
      {kEn::TextureType::Normal, aiTextureType_NORMALS},
      {kEn::TextureType::Specular, aiTextureType_SPECULAR},
  };

  for (const auto& [type, ai_type] : kTypes) {
    for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i) {
      aiString str;
      mat->GetTexture(ai_type, i, &str);
      const std::shared_ptr<kEn::Texture> texture = kEn::device().create_texture(
          absolute(directory / str.C_Str()), sampler, kEn::TextureFormat::RGBA8, mip_levels);
      material.set_texture(type, texture, i);
    }
  }
}

struct MikkUserData {
  std::vector<kEn::Vertex>* vertices = nullptr;
};

kEn::Vertex& face_vertex(MikkUserData& data, int face, int vert) {
  return (*data.vertices)[static_cast<std::size_t>(face) * 3U + static_cast<std::size_t>(vert)];
}

const kEn::Vertex& face_vertex(const MikkUserData& data, int face, int vert) {
  return (*data.vertices)[static_cast<std::size_t>(face) * 3U + static_cast<std::size_t>(vert)];
}

int mikk_get_num_faces(const SMikkTSpaceContext* ctx) {
  const auto& data = *static_cast<const MikkUserData*>(ctx->m_pUserData);
  return static_cast<int>(data.vertices->size() / 3U);
}

int mikk_get_num_vertices_of_face(const SMikkTSpaceContext*, const int /*face*/) { return 3; }

void mikk_get_position(const SMikkTSpaceContext* ctx, float out[], const int face, const int vert) {
  const auto& data = *static_cast<const MikkUserData*>(ctx->m_pUserData);
  const auto& v    = face_vertex(data, face, vert).pos;

  out[0] = v.x;
  out[1] = v.y;
  out[2] = v.z;
}

void mikk_get_normal(const SMikkTSpaceContext* ctx, float out[], const int face, const int vert) {
  const auto& data = *static_cast<const MikkUserData*>(ctx->m_pUserData);
  const auto& v    = face_vertex(data, face, vert).normal;

  out[0] = v.x;
  out[1] = v.y;
  out[2] = v.z;
}

void mikk_get_texcoord(const SMikkTSpaceContext* ctx, float out[], const int face, const int vert) {
  const auto& data = *static_cast<const MikkUserData*>(ctx->m_pUserData);
  const auto& v    = face_vertex(data, face, vert).texture_coord;

  out[0] = v.x;
  out[1] = v.y;
}

void mikk_set_tspace_basic(const SMikkTSpaceContext* ctx, const float tangent[], const float sign, const int face,
                           const int vert) {
  auto& data = *static_cast<MikkUserData*>(ctx->m_pUserData);
  auto& v    = face_vertex(data, face, vert);

  const mEn::Vec3 t = mEn::normalize(mEn::Vec3{tangent[0], tangent[1], tangent[2]});
  v.tangent         = mEn::Vec4{t, sign};
}

void expand_indexed_mesh(std::vector<kEn::Vertex>& vertices, std::vector<std::uint32_t>& indices) {
  std::vector<kEn::Vertex> expanded;
  expanded.reserve(indices.size());

  for (const std::uint32_t index : indices) {
    expanded.push_back(vertices[index]);
  }

  vertices = std::move(expanded);
  indices.resize(vertices.size());
  std::iota(indices.begin(), indices.end(), 0U);
}

void generate_mikktspace(std::vector<kEn::Vertex>& vertices, std::vector<std::uint32_t>& indices) {
  if (indices.empty()) {
    return;
  }

  if (indices.size() % 3U != 0U) {
    throw std::runtime_error("MikkTSpace requires triangulated indices");
  }

  // MikkTSpace returns tangent space per face-vertex and explicitly warns against
  // overwriting tangent results through an existing shared index list.
  expand_indexed_mesh(vertices, indices);

  MikkUserData user_data{.vertices = &vertices};

  SMikkTSpaceInterface iface{};
  iface.m_getNumFaces          = mikk_get_num_faces;
  iface.m_getNumVerticesOfFace = mikk_get_num_vertices_of_face;
  iface.m_getPosition          = mikk_get_position;
  iface.m_getNormal            = mikk_get_normal;
  iface.m_getTexCoord          = mikk_get_texcoord;
  iface.m_setTSpaceBasic       = mikk_set_tspace_basic;

  SMikkTSpaceContext ctx{};
  ctx.m_pInterface = &iface;
  ctx.m_pUserData  = &user_data;

  if (!genTangSpaceDefault(&ctx)) {
    throw std::runtime_error("MikkTSpace tangent generation failed");
  }
}

kEn::Mesh process_mesh(aiMesh* mesh, const aiScene* scene, const kEn::SamplerDesc& sampler,
                       const std::filesystem::path& directory) {
  // Vertices
  std::vector<kEn::Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    kEn::Vertex v{};
    mEn::Vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    v.pos    = vector;

    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      v.normal = mEn::normalize(vector);
    } else {
      v.normal = mEn::Vec3(0.0F);
    }

    if (mesh->HasTangentsAndBitangents()) {
      const mEn::Vec3 t = mEn::normalize(mEn::Vec3{mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z});
      const mEn::Vec3 b =
          mEn::normalize(mEn::Vec3{mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z});
      const float sign = mEn::dot(mEn::cross(v.normal, t), b) < 0.0F ? -1.0F : 1.0F;
      v.tangent        = mEn::Vec4{t, sign};
    } else {
      v.tangent = mEn::Vec4(0.0F);
    }

    if (mesh->mTextureCoords[0]) {
      mEn::Vec2 vec;
      vec.x           = mesh->mTextureCoords[0][i].x;
      vec.y           = mesh->mTextureCoords[0][i].y;
      v.texture_coord = vec;
    } else {
      v.texture_coord = mEn::Vec2(0.0F);
    }

    vertices.emplace_back(v);
  }

  // Indices
  std::vector<uint32_t> indices;
  indices.reserve(mesh->mNumFaces * 3);
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Generate tangents when the asset does not already contain them
  if (!mesh->HasTangentsAndBitangents() && mesh->HasNormals() && mesh->mTextureCoords[0] != nullptr) {
    generate_mikktspace(vertices, indices);
  }

  // Material
  aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

  const auto luminance = [](const aiColor3D& c) { return 0.2126F * c.r + 0.7152F * c.g + 0.0722F * c.b; };

  kEn::Material material;
  aiColor3D color;
  mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
  material.ambient_factor = luminance(color);
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  material.diffuse_factor = luminance(color);
  mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
  material.specular_factor = luminance(color);
  mat->Get(AI_MATKEY_SHININESS, material.shininess_factor);
  material.shininess_factor = std::max(material.shininess_factor, 1.F);
  mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  material.emissive = luminance(color) > 0.F;

  std::uint32_t mip_levels = kEn::kFullMipChain;
  if (mat->GetTextureCount(aiTextureType_OPACITY) > 0) {
    mip_levels           = 1;
    material.transparent = true;
  }

  load_material_textures(mat, material, sampler, directory, mip_levels);

  return {mesh->mName.C_Str(), vertices, indices, material};
}

}  // namespace

namespace kEn {

std::unordered_map<std::filesystem::path, std::shared_ptr<Model>> Model::loaded_resources_;

std::shared_ptr<Model> Model::load(const std::filesystem::path& path, const SamplerDesc& sampler, bool flip_uvs) {
  if (const auto it = loaded_resources_.find(path); it != loaded_resources_.end()) {
    return it->second;
  }

  auto model = std::make_shared<Model>(path, sampler, flip_uvs);
  return loaded_resources_.emplace(path, std::move(model)).first->second;
}

std::span<const Mesh> Model::opaque_meshes() const noexcept { return opaque_meshes_; }
std::span<Mesh> Model::opaque_meshes() noexcept { return opaque_meshes_; }
std::span<const Mesh> Model::transparent_meshes() const noexcept { return transparent_meshes_; }
std::span<Mesh> Model::transparent_meshes() noexcept { return transparent_meshes_; }

void Model::render(Shader& shader, const Transform& transform) const {
  for (const auto& mesh : opaque_meshes_) {
    mesh.render(shader, transform);
  }

  // TODO(kuzu): implement depth (re)sorting at render time
  for (const auto& mesh : transparent_meshes_) {
    mesh.render(shader, transform);
  }
}

void Model::load_model(const std::filesystem::path& path, const SamplerDesc& sampler, bool flip_uvs) {
  Assimp::Importer importer;
  const unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | (flip_uvs ? aiProcess_FlipUVs : 0U);
  const aiScene* scene     = importer.ReadFile(path.string(), flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    KEN_CORE_ERROR("ASSIMP: {}", importer.GetErrorString());
    throw std::runtime_error(importer.GetErrorString());
  }

  process_node(scene->mRootNode, scene, sampler, path.parent_path());
}

void Model::process_node(aiNode* node, const aiScene* scene, const SamplerDesc& sampler,
                         const std::filesystem::path& directory) {
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh   = scene->mMeshes[node->mMeshes[i]];
    Mesh processed = process_mesh(mesh, scene, sampler, directory);
    if (processed.material.transparent) {
      transparent_meshes_.push_back(std::move(processed));
    } else {
      opaque_meshes_.push_back(std::move(processed));
    }
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    process_node(node->mChildren[i], scene, sampler, directory);
  }
}

}  // namespace kEn
