#include "Model/importer.h"
#include "Model/material.h"
#include "Render/texture.h"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "assimp/types.h"
#include <iostream>
#include <memory>
#include <string>

std::vector<Mesh<Vertex>>
AssimpImport::LoadModelMeshes(std::string const &path) {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                aiProcess_JoinIdenticalVertices);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return {};
  }
  std::string rootModeldir = path.substr(0, path.find_last_of('/'));
  return AssimpImport::processNodes(scene->mRootNode, scene, rootModeldir);
}
MaterialType detectMaterialType(aiMaterial *material);
std::shared_ptr<Texture> getTexture_ptr(aiTextureType type);

std::vector<Mesh<Vertex>>
AssimpImport::processNodes(aiNode *node, const aiScene *scene,
                           const std::string &rootModelDir) {
  // process all the node's meshes (if any)
  std::vector<Mesh<Vertex>> meshes;
  std::clog << "  Processing Node " << node->mName.C_Str() << std::endl;
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh m = AssimpImport::processMesh(mesh, scene, rootModelDir);
    meshes.push_back(m);
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    std::vector<Mesh<Vertex>> tmp_vec =
        AssimpImport::processNodes(node->mChildren[i], scene, rootModelDir);
    meshes.insert(meshes.end(), tmp_vec.begin(), tmp_vec.end());
  }
  return meshes;
}

Mesh<Vertex> AssimpImport::processMesh(aiMesh *mesh, const aiScene *scene,
                                       const std::string &rootModelDir) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    vector.x = (float)mesh->mVertices[i].x;
    vector.y = (float)mesh->mVertices[i].y;
    vector.z = (float)mesh->mVertices[i].z;
    vertex.position = vector;
    if (mesh->HasNormals()) {
      vector.x = (float)mesh->mNormals[i].x;
      vector.y = (float)mesh->mNormals[i].y;
      vector.z = (float)mesh->mNormals[i].z;
      vertex.Normal = vector;
    }
    if (mesh->HasTextureCoords(0)) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoord = vec;
    } else {
      vertex.TexCoord = glm::vec2(0.0f, 0.0f);
    }
    if (mesh->HasTangentsAndBitangents()) {
      glm::vec3 T, B;
      T.x = mesh->mTangents[i].x;
      T.y = mesh->mTangents[i].y;
      T.z = mesh->mTangents[i].z;

      B.x = mesh->mBitangents[i].x;
      B.y = mesh->mBitangents[i].y;
      B.z = mesh->mBitangents[i].z;
      vertex.Tangent = T;
      vertex.Bitangent = B;
    }
    vertices.push_back(vertex);
  }
  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  // process material
  aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

  material = AssimpImport::loadMaterial(mat, rootModelDir);

  return Mesh(vertices, indices, material);
}

Material AssimpImport::loadMaterial(aiMaterial *mat,
                                    const std::string &rootModelDir) {

  Material material;

  MaterialType mat_type = detectMaterialType(mat);

  material.type = mat_type;

  aiString str;
  std::string tex_path;

  if (mat->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS) {
    tex_path = rootModelDir + '/' + str.C_Str();
    material.setNewMap("u_normalMap", TextureManager::getTexture(tex_path));
    material.setNewFlag("materialPhong.hasNormalMap", true);
    str.Clear();
  } else {
    material.setNewFlag("materialPhong.hasNormalMap", false);
  }

  switch (mat_type) {
  case MaterialType::PHONG:
    if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
      tex_path = rootModelDir + '/' + str.C_Str();
      material.setNewMap("u_diffuseMap", TextureManager::getTexture(tex_path));
      material.setNewFlag("materialPhong.hasDiffuseMap", true);
      str.Clear();
    } else {
      material.setNewFlag("materialPhong.hasDiffuseMap", false);
      aiColor4D color;
      aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color);
      material.setNewColor("materialPhong.diffuse",
                           glm::vec4(color.r, color.g, color.b, color.a));
    }
    if (mat->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS) {
      tex_path = rootModelDir + '/' + str.C_Str();
      material.setNewMap("u_specularMap", TextureManager::getTexture(tex_path));
      material.setNewFlag("materialPhong.hasSpecularMap", true);
      str.Clear();
    } else {
      material.setNewFlag("materialPhong.hasSpecularMap", false);
      aiColor4D color;
      aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color);
      material.setNewColor("materialPhong.specular",
                           glm::vec4(color.r, color.g, color.b, color.a));
    }

    if (mat->GetTexture(aiTextureType_AMBIENT, 0, &str) == AI_SUCCESS) {
      tex_path = rootModelDir + '/' + str.C_Str();
      material.setNewMap("u_ambientMap", TextureManager::getTexture(tex_path));
      material.setNewFlag("materialPhong.hasAmbientMap", true);
      str.Clear();
    } else {
      material.setNewFlag("materialPhong.hasAmbientMap", false);
      aiColor4D color;
      aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color);
      material.setNewColor("materialPhong.ambient",
                           glm::vec4(color.r, color.g, color.b, color.a));
    }
    float shine, sh_str;
    aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shine);
    material.setNewFloat("materialPhong.shininess", shine);
    aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH, &sh_str);
    material.setNewFloat("materialPhong.shininessStrength", sh_str);
    break;

  case MaterialType::PBR:
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
    tex_path = rootModelDir + '/' + str.C_Str();
    material.setNewMap("u_diffuseMap", TextureManager::getTexture(tex_path));
    material.setNewFlag("materialPhong.hasDiffuseMap", true);
    str.Clear();
    break;
  case MaterialType::CUSTOM:
    break;
  }
  return material;
}

MaterialType detectMaterialType(aiMaterial *material) {

  bool hasPBRMaps =
      (material->GetTextureCount(aiTextureType_METALNESS) > 0) ||
      (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0);
  float f;
  bool hasPBRprop =
      (material->Get(AI_MATKEY_METALLIC_FACTOR, f) == AI_SUCCESS ||
       material->Get(AI_MATKEY_ROUGHNESS_FACTOR, f) == AI_SUCCESS);
  if (hasPBRMaps || hasPBRprop)
    return MaterialType::PBR;
  else
    return MaterialType::PHONG;
}
