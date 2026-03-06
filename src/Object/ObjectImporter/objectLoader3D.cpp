#include "objectLoader3D.h"

#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "File/utils.h"
#include "Object/Material/material.h"
#include "Object/mesh3D.h"
#include "Object/object3D.h"
#include "assimp/types.h"

namespace Object3DImport {

AssimpLoader::AssimpLoader(const std::string& path, const unsigned aiProcess_flags) {
  m_aiProcess_flags = aiProcess_flags;
  LoadObject3D(path);
}

void AssimpLoader::LoadObject3D(std::string const& path) {
  Assimp::Importer import;
  m_scene = import.ReadFile(path, m_aiProcess_flags);

  if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  m_rootDir = FileUtils::ExtractRootDir(path.c_str());

  m_processNodes(m_scene->mRootNode, -1);
  m_processMeshes();
  m_processAnimations();

  std::cout << "Object3D loaded: " << path << std::endl;
  std::cout << "  Meshes: " << m_meshes.size() << std::endl;
}

void AssimpLoader::m_processNodes(const aiNode* ai_node, const int16_t parentIndx) {
  // std::cout << "NODE: " << ai_node->mName.C_Str() << " (meshes in node: " << ai_node->mNumMeshes << ")" << std::endl;

  Node node;
  node.name = ai_node->mName.C_Str();

  glm::mat4 localTransform = m_aiToGlmMat4(ai_node->mTransformation);

  node.localTransform = localTransform;

  node.parent_i = parentIndx;
  int node_i = m_nodes.size();
  m_nodes.push_back(node);
  m_loadedNodes[node.name] = node_i;

  if (parentIndx >= 0) m_nodes[parentIndx].children_i.push_back(node_i);

  for (unsigned i = 0; i < ai_node->mNumMeshes; i++) m_nodes[node_i].mesh_i.push_back(ai_node->mMeshes[i]);

  for (unsigned int i = 0; i < ai_node->mNumChildren; i++) {
    m_processNodes(ai_node->mChildren[i], node_i);
  }
}

void AssimpLoader::m_processMeshes() {
  for (size_t i = 0; i < m_scene->mNumMeshes; i++) {
    m_meshes.push_back(m_processMesh(m_scene->mMeshes[i]));
  }
}

Mesh AssimpLoader::m_processMesh(aiMesh* mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;
  std::string Name = mesh->mName.C_Str();

  if (!mesh->HasPositions() || !mesh->HasFaces()) {
    std::cerr << "Warning: Invalid mesh skipped (no positions or faces)" << std::endl;
    return Mesh(vertices, indices, Name);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    if (face.mNumIndices < 3) {
      std::cerr << "Face with " << face.mNumIndices << " indices skipped!" << std::endl;
      continue;
    }
  }

  vertices.reserve(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    glm::vec3 position;
    position.x = mesh->mVertices[i].x;
    position.y = mesh->mVertices[i].y;
    position.z = mesh->mVertices[i].z;
    vertex.position = position;

    if (mesh->HasNormals()) {
      glm::vec3 normal;
      normal.x = mesh->mNormals[i].x;
      normal.y = mesh->mNormals[i].y;
      normal.z = mesh->mNormals[i].z;
      vertex.Normal = normal;
    } else {
      vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    if (mesh->HasTextureCoords(0)) {
      glm::vec2 uv;
      uv.x = mesh->mTextureCoords[0][i].x;
      uv.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoord = uv;
    } else {
      vertex.TexCoord = glm::vec2(0.0f, 0.0f);
    }

    if (mesh->HasTangentsAndBitangents()) {
      glm::vec3 tangent;
      tangent.x = mesh->mTangents[i].x;
      tangent.y = mesh->mTangents[i].y;
      tangent.z = mesh->mTangents[i].z;

      vertex.Tangent = tangent;
    }

    vertices.push_back(vertex);
  }

  indices.reserve(mesh->mNumFaces * 3);
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    if (face.mNumIndices != 3) {
      std::cerr << "Warning: Non-triangular face skipped" << std::endl;
      continue;
    }
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  for (size_t b = 0; b < mesh->mNumBones; b++) {
    aiBone* aBone = mesh->mBones[b];
    std::string name = aBone->mName.C_Str();

    int nodeIndex = m_loadedNodes[name];

    int boneIndex;
    if (m_loadedBones.find(name) == m_loadedBones.end()) {
      boneIndex = m_bones.size();
      m_loadedBones[name] = boneIndex;

      SkinnedObject3D::Bone bone;
      bone.name = name;
      m_nodes[nodeIndex].bone_i = boneIndex;
      bone.offsetMatrix = m_aiToGlmMat4(aBone->mOffsetMatrix);
      m_bones.push_back(bone);
    } else {
      boneIndex = m_loadedBones[name];
    }

    for (size_t w = 0; w < aBone->mNumWeights; w++) {
      int v = aBone->mWeights[w].mVertexId;
      float weight = aBone->mWeights[w].mWeight;

      for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (vertices[v].BoneIDs[i] < 0) {
          vertices[v].Weights[i] = weight;
          vertices[v].BoneIDs[i] = boneIndex;
          break;
        }
      }
    }
  }

  material = m_loadMaterial(m_scene->mMaterials[mesh->mMaterialIndex]);

  return Mesh(vertices, indices, material, Name);
}

void AssimpLoader::m_processAnimations() {
  {
    for (unsigned i = 0; i < m_scene->mNumAnimations; i++) {
      aiAnimation* aAnim = m_scene->mAnimations[i];
      Animation anim;

      anim.name = aAnim->mName.C_Str();
      anim.duration = (float)aAnim->mDuration;
      anim.ticksPerSecond = aAnim->mTicksPerSecond != 0 ? (float)aAnim->mTicksPerSecond : 25.0f;

      m_processAnimationChannels(aAnim, anim);

      m_animations.push_back(anim);
    }
  }
}

void AssimpLoader::m_processAnimationChannels(aiAnimation* aAnim, Animation& anim) {
  for (unsigned c = 0; c < aAnim->mNumChannels; c++) {
    aiNodeAnim* channel = aAnim->mChannels[c];

    std::string nodeName = channel->mNodeName.C_Str();

    AnimationChannel ch;

    for (unsigned i = 0; i < channel->mNumPositionKeys; i++) {
      ch.positions.push_back(
          {(float)channel->mPositionKeys[i].mTime, m_convertAssimpVec3(channel->mPositionKeys[i].mValue)});
    }

    for (unsigned i = 0; i < channel->mNumRotationKeys; i++) {
      ch.rotations.push_back(
          {(float)channel->mRotationKeys[i].mTime, m_convertAssimpQuat(channel->mRotationKeys[i].mValue)});
    }

    for (unsigned i = 0; i < channel->mNumScalingKeys; i++) {
      ch.scales.push_back(
          {(float)channel->mScalingKeys[i].mTime, m_convertAssimpVec3(channel->mScalingKeys[i].mValue)});
    }

    anim.channels[nodeName] = ch;
  }
}

glm::mat4 AssimpLoader::m_aiToGlmMat4(const aiMatrix4x4& m) {
  glm::mat4 to;

  to[0][0] = m.a1;
  to[1][0] = m.a2;
  to[2][0] = m.a3;
  to[3][0] = m.a4;
  to[0][1] = m.b1;
  to[1][1] = m.b2;
  to[2][1] = m.b3;
  to[3][1] = m.b4;
  to[0][2] = m.c1;
  to[1][2] = m.c2;
  to[2][2] = m.c3;
  to[3][2] = m.c4;
  to[0][3] = m.d1;
  to[1][3] = m.d2;
  to[2][3] = m.d3;
  to[3][3] = m.d4;

  return to;
}

glm::vec3 AssimpLoader::m_convertAssimpVec3(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }

glm::quat AssimpLoader::m_convertAssimpQuat(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }

}  // namespace Object3DImport
