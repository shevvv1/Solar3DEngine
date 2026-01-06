#pragma once

#include <vector>

#include "Render/mesh.h"
#include "Render/shader.h"

class Model {
public:
  Model() = default;
  Model(std::vector<Mesh<Vertex>> &mesh_array,
        glm::vec3 coord = glm::vec3(0.0f));
  Model(Mesh<Vertex> &Mesh, glm::vec3 coord = glm::vec3(0.0f));
  Model(std::string const &path, glm::vec3 coord = glm::vec3(0.0f));

  void Draw(Shader &shader);
  void Rotate(glm::vec3 coord);
  void Scale(glm::vec3 size);
  void Move(glm::vec3 coord);
  void updateModelMat();

  void setMeshes(std::vector<Mesh<Vertex>> meshes);

private:
  std::vector<Mesh<Vertex>> m_meshes;

  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
  glm::mat4 m_model_mat;

  void loadModel(std::string const &path);
};
