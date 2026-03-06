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

class AssimpLoader {
 public:
  AssimpLoader() = default;
  AssimpLoader(const std::string& path, const unsigned aiProcess_flags);

  void LoadObject3D(std::string const& path);

  Object3D AssembleObject3D();
  SkinnedObject3D AssembleSkinnedObject3D();

  void ClearAll();

  void setAiProcessFlags(const unsigned flags) { m_aiProcess_flags = flags; }

  void setMaterialTextureList(const std::vector<std::string> u_names) { m_matTextureList = u_names; }
  void setMaterialColorList(const std::vector<std::string> u_names) { m_matColorList = u_names; }
  void setMaterialProperties(const std::vector<std::string> u_names) { m_matPropList = u_names; }

  std::vector<Mesh> GetMeshArr() { return m_meshes; }
  std::vector<Node> GetNodeArr() { return m_nodes; }
  std::vector<SkinnedObject3D::Bone> GetBoneArr() { return m_bones; }
  std::vector<Animation> GetAnimArr() { return m_animations; }

 private:
  const aiScene* m_scene;
  unsigned m_aiProcess_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords |
                               aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GlobalScale;
  std::string m_rootDir;

  std::vector<Mesh> m_meshes;
  std::vector<Node> m_nodes;
  std::unordered_map<std::string, int> m_loadedNodes;
  std::vector<SkinnedObject3D::Bone> m_bones;
  std::unordered_map<std::string, int16_t> m_loadedBones;
  std::vector<Animation> m_animations;

  std::vector<std::string> m_matTextureList;
  std::vector<std::string> m_matColorList;
  std::vector<std::string> m_matPropList;

  void m_processNodes(const aiNode* ai_node, const int16_t parentIndx);
  void m_processMeshes();
  Mesh m_processMesh(aiMesh* mesh);
  void m_processAnimations();
  void m_processAnimationChannels(aiAnimation* aAnim, Animation& anim);

  Material m_loadMaterial(aiMaterial* mat);
  MaterialType m_detectMaterialType(aiMaterial* material);

  glm::mat4 m_aiToGlmMat4(const aiMatrix4x4& m);
  glm::vec3 m_convertAssimpVec3(const aiVector3D& v);
  glm::quat m_convertAssimpQuat(const aiQuaternion& q);
};
}  // namespace Object3DImport
