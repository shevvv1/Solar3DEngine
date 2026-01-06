#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

#include "Model/material.h"
#include "Render/mesh.h"

namespace AssimpImport {
std::vector<Mesh<Vertex>> LoadModelMeshes(std::string const &path);
std::vector<Mesh<Vertex>> processNodes(aiNode *node, const aiScene *scene,
                                       const std::string &rootModelDir);
Mesh<Vertex> processMesh(aiMesh *mesh, const aiScene *scene,
                         const std::string &rootModelDir);
Material loadMaterial(aiMaterial *mat, const std::string &rootModelDir);
} // namespace AssimpImport
