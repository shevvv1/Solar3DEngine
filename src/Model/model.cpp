#include "model.h"

#include <iostream>
#include <vector>

#include "Model/importer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"

Model::Model(std::string const& path, glm::vec3 coord) {
  std::clog << "Loading model:" << path << "\n";
  loadModel(path);
  m_scale = glm::vec3(1.f);
  m_rotation = glm::vec3(0.0f);
  m_model_mat = glm::mat4(1.0f);
  Move(coord);
  Rotate(m_rotation);
  Scale(m_scale);
  updateModelMat();
}

void Model::Draw(Shader& shader) {
  shader.setUniformMatrix4f("u_MM", m_model_mat);
  shader.setUniformMatrix3f("u_NM", m_normal_mat);

  for (unsigned int i = 0; i < m_meshes.size(); i++) {
    m_meshes[i].Draw(shader);
  }
}

void Model::updateModelMat() {
  glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), m_scale);

  glm::quat rot_x = glm::angleAxis(glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
  glm::quat rot_y = glm::angleAxis(glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
  glm::quat rot_z = glm::angleAxis(glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

  glm::quat total_rotation = rot_y * rot_x * rot_z;
  glm::mat4 rotation_mat = glm::mat4_cast(total_rotation);

  glm::mat4 translation_mat = glm::translate(glm::mat4(1.f), m_position);

  m_model_mat = translation_mat * rotation_mat * scale_mat;
  m_normal_mat = glm::transpose(glm::inverse(glm::mat3(m_model_mat)));
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
  updateModelMat();
}

void Model::loadModel(std::string const& path) { *this = AssimpImport::LoadModelMeshes(path); }

void Model::setMeshes(const std::vector<Mesh<Vertex>>& meshes) { m_meshes = meshes; }

void Model::setBoundBox(const BoundBox& bb) { m_boundBox = bb; }
