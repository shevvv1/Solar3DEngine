#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>

#include "Model/model.h"
#include "assimp/material.h"
#include "utils.h"

namespace AssimpImport {
const static unsigned int default_flags =
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices |
    aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_GenUVCoords | aiProcess_FlipUVs;

struct ImportConfig {
  float scaleFactor = 1.0f;
  bool convertToLeftHanded = true;
  bool fixInfacingNormals = true;
};

Model LoadModelMeshes(std::string const& path, unsigned int flag_assimp = default_flags,
                      const ImportConfig& config = ImportConfig());

// Material uniforms list
struct AiMatKey {
  const char* key;
  unsigned int type;
  unsigned int index;

  AiMatKey(const char* k = nullptr, unsigned t = 0, unsigned i = 0) : key(k), type(t), index(i) {}

  bool operator==(const AiMatKey& other) const {
    return key == other.key && type == other.type && index == other.index;
  }
};
}  // namespace AssimpImport

namespace std {
template <>
struct hash<AssimpImport::AiMatKey> {
  std::size_t operator()(const AssimpImport::AiMatKey& k) const {
    std::size_t seed = 0;
    Utils::hash_combine(seed, k.key);
    Utils::hash_combine(seed, k.type);
    Utils::hash_combine(seed, k.index);
    return seed;
  }
};
}  // namespace std

namespace AssimpImport {
static const std::unordered_map<aiTextureType, std::string> aiTextureTypeUniforms = {
    {aiTextureType_DIFFUSE, "u_albedoMap"},
    {aiTextureType_BASE_COLOR, "u_albedoMap"},
    {aiTextureType_SPECULAR, "u_specularMap"},
    {aiTextureType_AMBIENT, "u_aoMap"},
    {aiTextureType_AMBIENT_OCCLUSION, "u_aoMap"},
    {aiTextureType_SHEEN, "u_alphaMap"},
    {aiTextureType_NORMALS, "u_normalMap"},
    {aiTextureType_HEIGHT, "u_heightMap"},
    {aiTextureType_EMISSIVE, "u_emissiveMap"},
    {aiTextureType_METALNESS, "u_metallicMap"},
    {aiTextureType_DIFFUSE_ROUGHNESS, "u_roughnessMap"},
    {aiTextureType_GLTF_METALLIC_ROUGHNESS, "u_metallicRoughnessMap"},
    {aiTextureType_EMISSION_COLOR, "u_emissiveMap"},
    {aiTextureType_LIGHTMAP, "u_lightMap"}};

static const std::unordered_map<AiMatKey, std::string> aiMatKeyColorsUniforms = {
    // Colors
    {{AI_MATKEY_COLOR_DIFFUSE}, "u_albedoColor"},          {{AI_MATKEY_COLOR_SPECULAR}, "u_specularColor"},
    {{AI_MATKEY_COLOR_AMBIENT}, "u_ambientColor"},         {{AI_MATKEY_COLOR_EMISSIVE}, "u_emissiveColor"},
    {{AI_MATKEY_COLOR_TRANSPARENT}, "u_transparentColor"}, {{AI_MATKEY_COLOR_REFLECTIVE}, "u_reflectiveColor"}};

static const std::unordered_map<AiMatKey, std::string> aiMatKeyPropUniforms = {
    // PBR properties
    {{AI_MATKEY_METALLIC_FACTOR}, "u_metallicFactor"},
    {{AI_MATKEY_ROUGHNESS_FACTOR}, "u_roughnessFactor"},

    // Phong properties
    {{AI_MATKEY_SHININESS}, "u_shininess"},
    {{AI_MATKEY_SHININESS_STRENGTH}, "u_shininessStrength"},
    {{AI_MATKEY_REFLECTIVITY}, "u_reflectivity"},
    {{AI_MATKEY_REFRACTI}, "u_refractionIndex"},

    // Opacity/Transparency
    {{AI_MATKEY_OPACITY}, "u_opacity"},
    {{AI_MATKEY_TRANSPARENCYFACTOR}, "u_transparencyFactor"},

    // Bump/Displacement
    {{AI_MATKEY_BUMPSCALING}, "u_bumpScaling"},
};
}  // namespace AssimpImport
