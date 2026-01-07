#include "model.h"
#include "Model/importer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include <iostream>
#include <vector>

Model::Model(std::string const &path, glm::vec3 coord) {
  m_model_mat = glm::mat4(1.0f);
  Move(coord);
  m_rotation = glm::vec3(0.0f);
  m_scale = glm::vec3(1.f);
  updateModelMat();
  std::clog << "Loading model:" << path << "\n";
  loadModel(path);
}

void Model::Draw(Shader &shader) {

  shader.setUniformMatrix4f("u_MM", m_model_mat);

  for (unsigned int i = 0; i < m_meshes.size(); i++) {
    m_meshes[i].Draw(shader);
  }
}

void Model::updateModelMat() {
  glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), m_scale);

  glm::quat rot_x =
      glm::angleAxis(glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
  glm::quat rot_y =
      glm::angleAxis(glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
  glm::quat rot_z =
      glm::angleAxis(glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

  glm::quat total_rotation = rot_y * rot_x * rot_z;
  glm::mat4 rotation_mat = glm::mat4_cast(total_rotation);

  glm::mat4 translation_mat = glm::translate(glm::mat4(1.f), m_position);

  m_model_mat = translation_mat * rotation_mat * scale_mat;
}

void Model::Rotate(glm::vec3 coord) {
  m_rotation = coord;
  updateModelMat();
}

void Model::Scale(glm::vec3 size) {
  m_scale = size;
  updateModelMat();
}

void Model::Move(glm::vec3 coord) {
  m_position = coord;
  m_position.y = m_position.y;
  updateModelMat();
}

void Model::loadModel(std::string const &path) {
  m_meshes = AssimpImport::LoadModelMeshes(path);
}

void Model::setMeshes(std::vector<Mesh<Vertex>> meshes) { m_meshes = meshes; }
