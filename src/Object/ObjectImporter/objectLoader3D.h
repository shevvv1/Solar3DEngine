#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Object/object3D.h"
#include "assimp/material.h"
#include "glm/ext/matrix_float4x4.hpp"

namespace Object3DImport {

struct AiMatKey {
  const char* key;
  unsigned int type;
  unsigned int index;

  AiMatKey(const char* k = nullptr, unsigned t = 0, unsigned i = 0) : key(k), type(t), index(i) {}

  bool operator==(const AiMatKey& other) const {
    return key == other.key && type == other.type && index == other.index;
  }
};

static const std::unordered_map<std::string, aiTextureType> defaultMatTextureList = {
    {"u_albedoMap", aiTextureType_DIFFUSE},
    {"u_albedoMap", aiTextureType_BASE_COLOR},
    {"u_specularMap", aiTextureType_SPECULAR},
    {"u_aoMap", aiTextureType_AMBIENT},
    {"u_aoMap", aiTextureType_AMBIENT_OCCLUSION},
    {"u_alphaMap", aiTextureType_SHEEN},
    {"u_normalMap", aiTextureType_NORMALS},
    {"u_heightMap", aiTextureType_HEIGHT},
    {"u_emissiveMap", aiTextureType_EMISSIVE},
    {"u_metallicMap", aiTextureType_METALNESS},
    {"u_roughnessMap", aiTextureType_DIFFUSE_ROUGHNESS},
    {"u_metallicRoughnessMap", aiTextureType_GLTF_METALLIC_ROUGHNESS},
    {"u_emissiveMap", aiTextureType_EMISSION_COLOR},
    {"u_lightMap", aiTextureType_LIGHTMAP}};

static const std::unordered_map<std::string, AiMatKey> defaultMatColorList = {
    // Colors
    {"u_albedoColor", {AI_MATKEY_COLOR_DIFFUSE}},          {"u_specularColor", {AI_MATKEY_COLOR_SPECULAR}},
    {"u_ambientColor", {AI_MATKEY_COLOR_AMBIENT}},         {"u_emissiveColor", {AI_MATKEY_COLOR_EMISSIVE}},
    {"u_transparentColor", {AI_MATKEY_COLOR_TRANSPARENT}}, {"u_reflectiveColor", {AI_MATKEY_COLOR_REFLECTIVE}}};

static const std::unordered_map<std::string, AiMatKey> defaultMatPropList = {
    // PBR properties
    {"u_metallicFactor", {AI_MATKEY_METALLIC_FACTOR}},
    {"u_roughnessFactor", {AI_MATKEY_ROUGHNESS_FACTOR}},

    // Phong properties
    {"u_shininess", {AI_MATKEY_SHININESS}},
    {"u_shininessStrength", {AI_MATKEY_SHININESS_STRENGTH}},
    {"u_reflectivity", {AI_MATKEY_REFLECTIVITY}},
    {"u_refractionIndex", {AI_MATKEY_REFRACTI}},

    // Opacity/Transparency
    {"u_opacity", {AI_MATKEY_OPACITY}},
    {"u_transparencyFactor", {AI_MATKEY_TRANSPARENCYFACTOR}},

    // Bump/Displacement
    {"u_bumpScaling", {AI_MATKEY_BUMPSCALING}},
};

//---------

class AssimpLoader {
 public:
  AssimpLoader() = default;
  AssimpLoader(const unsigned aiProcess_flags);

  void LoadObject3D(std::string const& path);

  std::vector<Mesh> GetMeshArr() { return m_meshes; }
  std::vector<Object3D::Node> GetNodeArr() { return m_nodes; }

 private:
  const aiScene* m_scene;
  unsigned m_aiProcess_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords |
                               aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GlobalScale;
  bool m_yUp = true;
  std::string m_rootDir;

  std::vector<Mesh> m_meshes;
  std::vector<Object3D::Node> m_nodes;

  std::unordered_map<std::string, aiTextureType> m_matTextureList = defaultMatTextureList;
  std::unordered_map<std::string, AiMatKey> m_matColorList = defaultMatColorList;
  std::unordered_map<std::string, AiMatKey> m_matPropList = defaultMatPropList;

  void m_processNodes(const aiNode* ai_node, const int16_t parentIndx);
  void m_processMeshes();
  Mesh m_processMesh(aiMesh* mesh);
  glm::mat4 m_aiToGlmMat4(const aiMatrix4x4& m);

  Material m_loadMaterial(aiMaterial* mat);
  MaterialType m_detectMaterialType(aiMaterial* material);
};
}  // namespace Object3DImport
