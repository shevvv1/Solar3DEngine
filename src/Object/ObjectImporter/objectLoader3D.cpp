#include "objectLoader3D.h"

#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "File/utils.h"
#include "Object/material.h"
#include "Object/mesh3D.h"
#include "Render/texture.h"
#include "assimp/color4.h"
#include "assimp/material.h"
#include "assimp/types.h"

namespace Object3DImport {

AssimpLoader::AssimpLoader(const unsigned aiProcess_flags) { m_aiProcess_flags = aiProcess_flags; }

void AssimpLoader::LoadObject3D(std::string const& path) {
  Assimp::Importer import;
  m_scene = import.ReadFile(path, m_aiProcess_flags);

  if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  m_rootDir = FileUtils::ExtractRootDir(path.c_str());
  m_processMeshes();
  m_processNodes(m_scene->mRootNode, -1);

  std::cout << "Object3D loaded: " << path << std::endl;
  std::cout << "  Meshes: " << m_meshes.size() << std::endl;
}

void AssimpLoader::m_processNodes(const aiNode* ai_node, const int16_t parentIndx) {
  // std::cout << "NODE: " << ai_node->mName.C_Str() << " (meshes in node: " << ai_node->mNumMeshes << ")" << std::endl;

  Object3D::Node node;
  node.name = ai_node->mName.C_Str();

  glm::mat4 localTransform = m_aiToGlmMat4(ai_node->mTransformation);

  node.localTransform = localTransform;

  node.parent_i = parentIndx;
  int node_i = m_nodes.size();
  m_nodes.push_back(node);

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

  /*  for (size_t b = 0; b < mesh->mNumBones; b++) {
      aiBone* aBone = mesh->mBones[b];
      std::string name = aBone->mName.C_Str();

      int nodeIndex = m_loadedBonestoNodeI[name];

      int boneIndex;
      if (m_loadedBones.find(name) == m_loadedBones.end()) {
        boneIndex = m_bones.size();
        m_loadedBones[name] = boneIndex;

        Bone bone;
        bone.name = name;
        bone.node_indx = nodeIndex;
        m_nodes[nodeIndex].bones_indx = boneIndex;
        bone.offsetMatrix = m_convertAssimpMatrix(aBone->mOffsetMatrix);

        m_bones.push_back(bone);
      } else {
        boneIndex = m_loadedBones[name];
      }

      // 3. vertex weights
      for (size_t w = 0; w < aBone->mNumWeights; w++) {
        int v = aBone->mWeights[w].mVertexId;
        float weight = aBone->mWeights[w].mWeight;

        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
          if (vertices[v].BoneIDs[i] < 0) {
            vertices[v].Weights[i] = weight;
            vertices[v].BoneIDs[i] = boneIndex;
          }
        }
      }
    }*/

  material = m_loadMaterial(m_scene->mMaterials[mesh->mMaterialIndex]);

  return Mesh(vertices, indices, material, Name);
}

Material AssimpLoader::m_loadMaterial(aiMaterial* mat) {
  Material material;

  MaterialType mat_type = m_detectMaterialType(mat);

  material.setType(mat_type);

  aiString str;
  std::string tex_path;
  for (auto& it : m_matTextureList) {
    if (mat->GetTexture(it.second, 0, &str) == AI_SUCCESS) {
      tex_path = m_rootDir + '/' + str.C_Str();
      tex_path = FileUtils::convertTextureFilePath(str.C_Str(), m_rootDir.c_str());
      std::cout << tex_path << std::endl;
      material.setNewMap(it.first, TextureManager::getTexture(tex_path));
      material.setNewFlag(it.first + "_flag", true);
      std::cout << it.first << true << "\n";
      str.Clear();
    } else {
      material.setNewFlag(it.first + "_flag", false);
    }
  }
  for (auto& it : m_matColorList) {
    aiColor4D aiCol;
    if (mat->Get(it.second.key, it.second.type, it.second.index, aiCol) == AI_SUCCESS) {
      material.setNewColor(it.first, glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a));
      material.setNewFlag(it.first + "_flag", true);
    } else {
      material.setNewFlag(it.first + "_flag", false);
    }
  }
  for (auto& it : m_matPropList) {
    float aiProp;
    if (mat->Get(it.second.key, it.second.type, it.second.index, aiProp) == AI_SUCCESS) {
      material.setNewFloat(it.first, aiProp);
      material.setNewFlag(it.first + "_flag", true);
    } else {
      material.setNewFlag(it.first + "_flag", false);
    }
  }

  int doubleSided = 0;
  if (mat->Get(AI_MATKEY_TWOSIDED, doubleSided) == AI_SUCCESS && doubleSided) {
    material.setDoubleSide(true);
  }

  return material;
}

MaterialType AssimpLoader::m_detectMaterialType(aiMaterial* material) {
  bool hasPBRMaps = (material->GetTextureCount(aiTextureType_METALNESS) > 0) ||
                    (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) ||
                    (material->GetTextureCount(aiTextureType_GLTF_METALLIC_ROUGHNESS) > 0);
  float f;
  bool hasPBRprop = (material->Get(AI_MATKEY_METALLIC_FACTOR, f) == AI_SUCCESS ||
                     material->Get(AI_MATKEY_ROUGHNESS_FACTOR, f) == AI_SUCCESS);
  if (hasPBRMaps || hasPBRprop) {
    return MaterialType::PBR;
  } else {
    return MaterialType::PHONG;
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
}  // namespace Object3DImport
