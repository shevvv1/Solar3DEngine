#include "Model/importer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Model/material.h"
#include "Render/mesh.h"
#include "Render/texture.h"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "assimp/types.h"

namespace AssimpImport {

std::vector<Mesh<Vertex>> processNodes(aiNode* node, const aiScene* scene, const std::string& rootModelDir,
                                       const glm::mat4& parentTransform, const AssimpImport::ImportConfig& config);
Mesh<Vertex> processMesh(aiMesh* mesh, const aiScene* scene, const std::string& rootModelDir,
                         const glm::mat4& transform, const AssimpImport::ImportConfig& config);
glm::mat4 convertAssimpMatrix(const aiMatrix4x4& from);
void applyTransformToVertices(std::vector<Vertex>& vertices, const glm::mat4& transform, bool applyToNormals = true);
BoundBox createModelBoundBox(std::vector<Mesh<Vertex>> meshes);

Material loadMaterial(aiMaterial* mat, const std::string& rootModelDir);
MaterialType detectMaterialType(aiMaterial* material);

Model LoadModelMeshes(std::string const& path, unsigned int flag_assimp, const ImportConfig& config) {
  Model model;

  Assimp::Importer import;

  import.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 80.0f);

  const aiScene* scene = import.ReadFile(path, flag_assimp);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return {};
  }

  aiVector3D sceneScale(1.0f, 1.0f, 1.0f);
  if (scene->mMetaData && scene->mMetaData->Get("UnitScaleFactor", sceneScale.x)) {
    std::cout << "Model unit scale: " << sceneScale.x << std::endl;
  }

  std::string rootModeldir = path.substr(0, path.find_last_of('/'));

  glm::mat4 rootTransform = glm::mat4(1.0f);
  rootTransform =
      glm::scale(rootTransform, glm::vec3(config.scaleFactor * sceneScale.x, config.scaleFactor * sceneScale.x,
                                          config.scaleFactor * sceneScale.x));

  if (config.convertToLeftHanded) {
    rootTransform = glm::scale(rootTransform, glm::vec3(1.0f, 1.0f, -1.0f));
  }

  std::vector<Mesh<Vertex>> meshes = processNodes(scene->mRootNode, scene, rootModeldir, rootTransform, config);

  if (meshes.empty()) {
    std::cerr << "Warning: No meshes found. Trying alternative import..." << std::endl;

    unsigned int simpleFlags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs;

    import.FreeScene();
    scene = import.ReadFile(path, simpleFlags);

    if (scene && scene->mRootNode) {
      meshes = processNodes(scene->mRootNode, scene, rootModeldir, rootTransform, config);
    }
  }

  if (meshes.empty()) {
    std::cerr << "ERROR: Failed to load any meshes from: " << path << std::endl;
    return {};
  }

  BoundBox modelBounds = createModelBoundBox(meshes);
  model.setBoundBox(modelBounds);
  model.setMeshes(meshes);

  std::cout << "Model loaded: " << path << std::endl;
  std::cout << "  Meshes: " << meshes.size() << std::endl;

  return model;
}

std::vector<Mesh<Vertex>> processNodes(aiNode* node, const aiScene* scene, const std::string& rootModelDir,
                                       const glm::mat4& parentTransform, const AssimpImport::ImportConfig& config) {
  std::vector<Mesh<Vertex>> meshes;

  glm::mat4 nodeTransform = convertAssimpMatrix(node->mTransformation);
  glm::mat4 fullTransform = parentTransform * nodeTransform;

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh m = processMesh(mesh, scene, rootModelDir, fullTransform, config);
    // if (!m.getVertices().empty()) {
    meshes.push_back(m);
    //}
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    std::vector<Mesh<Vertex>> childMeshes =
        processNodes(node->mChildren[i], scene, rootModelDir, fullTransform, config);
    meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
  }

  return meshes;
}

glm::mat4 convertAssimpMatrix(const aiMatrix4x4& from) {
  glm::mat4 to;

  to[0][0] = from.a1;
  to[1][0] = from.a2;
  to[2][0] = from.a3;
  to[3][0] = from.a4;
  to[0][1] = from.b1;
  to[1][1] = from.b2;
  to[2][1] = from.b3;
  to[3][1] = from.b4;
  to[0][2] = from.c1;
  to[1][2] = from.c2;
  to[2][2] = from.c3;
  to[3][2] = from.c4;
  to[0][3] = from.d1;
  to[1][3] = from.d2;
  to[2][3] = from.d3;
  to[3][3] = from.d4;

  return to;
}

Mesh<Vertex> processMesh(aiMesh* mesh, const aiScene* scene, const std::string& rootModelDir,
                         const glm::mat4& transform, const AssimpImport::ImportConfig& config) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;
  BoundBox bb;

  if (!mesh->HasPositions() || !mesh->HasFaces()) {
    std::cerr << "Warning: Invalid mesh skipped (no positions or faces)" << std::endl;
    return Mesh<Vertex>(vertices, indices, material, bb);
  }

  vertices.reserve(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    glm::vec3 position;
    position.x = mesh->mVertices[i].x;
    position.y = mesh->mVertices[i].y;
    position.z = mesh->mVertices[i].z;

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
      glm::vec3 tangent, bitangent;
      tangent.x = mesh->mTangents[i].x;
      tangent.y = mesh->mTangents[i].y;
      tangent.z = mesh->mTangents[i].z;

      bitangent.x = mesh->mBitangents[i].x;
      bitangent.y = mesh->mBitangents[i].y;
      bitangent.z = mesh->mBitangents[i].z;

      vertex.Tangent = tangent;
      vertex.Bitangent = bitangent;
    }

    vertex.position = position;
    vertices.push_back(vertex);

    bb.update(position);
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

  if (transform != glm::mat4(1.0f)) {
    applyTransformToVertices(vertices, transform, true);

    bb = BoundBox();
    for (const auto& vertex : vertices) {
      bb.update(vertex.position);
    }
  }

  if (mesh->mMaterialIndex < scene->mNumMaterials) {
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    material = loadMaterial(mat, rootModelDir);
  }

  bb.update();

  return Mesh<Vertex>(vertices, indices, material, bb);
}

void applyTransformToVertices(std::vector<Vertex>& vertices, const glm::mat4& transform, bool applyToNormals) {
  glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));

  for (auto& vertex : vertices) {
    glm::vec4 pos = transform * glm::vec4(vertex.position, 1.0f);
    vertex.position = glm::vec3(pos) / pos.w;

    if (applyToNormals) {
      glm::vec3 normal = glm::normalize(glm::mat3(normalMatrix) * vertex.Normal);
      vertex.Normal = normal;

      if (glm::length(vertex.Tangent) > 0.0f) {
        vertex.Tangent = glm::normalize(glm::mat3(transform) * vertex.Tangent);
        vertex.Bitangent = glm::normalize(glm::mat3(transform) * vertex.Bitangent);
      }
    }
  }
}

BoundBox createModelBoundBox(std::vector<Mesh<Vertex>> meshes) {
  BoundBox bb = meshes[0].getBoundBox();
  glm::vec3 min = bb.min;
  glm::vec3 max = bb.max;
  for (auto mesh : meshes) {
    max = glm::max(max, mesh.getBoundBox().max);
    min = glm::min(min, mesh.getBoundBox().min);
  }
  bb.max = max;
  bb.min = min;
  bb.update();
  return bb;
}

Material loadMaterial(aiMaterial* mat, const std::string& rootModelDir) {
  Material material;

  MaterialType mat_type = detectMaterialType(mat);

  material.setType(mat_type);

  aiString str;
  std::string tex_path;

  for (auto& it : AssimpImport::aiTextureTypeUniforms) {
    if (mat->GetTexture(it.first, 0, &str) == AI_SUCCESS) {
      tex_path = rootModelDir + '/' + str.C_Str();
      material.setNewMap(it.second, TextureManager::getTexture(tex_path));
      material.setNewFlag(it.second + "_flag", true);
      str.Clear();
    } else {
      material.setNewFlag(it.second + "_flag", false);
    }
  }

  for (auto& it : AssimpImport::aiMatKeyColorsUniforms) {
    aiColor4D color;
    bool isDefaultBlack = false;
    if (aiGetMaterialColor(mat, it.first.key, it.first.type, it.first.index, &color) == AI_SUCCESS) {
      isDefaultBlack = (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f && color.a == 1.0f || color.a == 0.0f);
      material.setNewFlag(it.second + "_flag", true);

    } else if (isDefaultBlack) {
      aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color);
      material.setNewFlag(it.second + "_flag", false);
    }
    material.setNewColor(it.second, glm::vec4(color.r, color.g, color.b, color.a));
  }

  return material;
}

MaterialType detectMaterialType(aiMaterial* material) {
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

}  // namespace AssimpImport
