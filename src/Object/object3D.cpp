#include "Object/object3D.h"

#include <iostream>
#include <memory>
#include <vector>

#include "Math/mathUtils.h"
#include "Object/ObjectImporter/objectLoader3D.h"
#include "Object/mesh3D.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"

Object3D::Object3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType) {
  std::clog << "Loading model:" << path << "\n";
  m_meshType = meshType;
  m_shader = shader;
  Load(path);
  Update();
}

void Object3D::Load(std::string const& path) {
  Object3DImport::AssimpLoader loader;
  loader.LoadObject3D(path);
  m_meshes = loader.GetMeshArr();
  m_nodes = loader.GetNodeArr();
  for (auto& m : m_meshes) {
    m.setVertexType(m_meshType);
    m.CreateVAO();
  }
}

void Object3D::Draw(DrawMethod dMethod) {
  m_shader->setUniformBool("hasBone", false);
  for (auto& n : m_nodes) {
    if (n.mesh_i.empty()) continue;
    if (dMethod == DrawMethod::TRIANGLE_SINGLE) {
      m_shader->setUniformMatrix4f("u_MM", m_transform_mat * n.globalTransform);
    } else {
      m_shader->setUniformMatrix4f("u_MM", n.globalTransform);
    }
    m_shader->setUniformMatrix3f("u_NM", m_normal_mat);

    for (auto& i : n.mesh_i) m_meshes[i].Draw(m_shader, dMethod);
  }
}

void Object3D::Update() {
  updateTransform();
  updateLocal();
  m_normal_mat = glm::transpose(glm::inverse(glm::mat3(m_local_mat)));
}

void Object3D::updateLocal() { m_local_mat = m_calculateHierarhyGlobalMat(0, glm::mat4(1)); }

glm::mat4 Object3D::m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat) {
  m_nodes[indx].globalTransform = parentMat * m_nodes[indx].localTransform;

  for (int child : m_nodes[indx].children_i) {
    m_calculateHierarhyGlobalMat(child, m_nodes[indx].globalTransform);
  }
  return m_nodes[indx].globalTransform;
}

void Object3D::updateTransform() {
  glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), m_transformProps.scale);

  glm::mat4 rotation_mat = glm::mat4_cast(m_transformProps.rotation);

  glm::mat4 translation_mat = glm::translate(glm::mat4(1.f), m_transformProps.position);

  m_transform_mat = translation_mat * rotation_mat * scale_mat;
}

void Object3D::Rotate(float angle, glm::vec3 axis) {
  m_transformProps.rotation = glm::angleAxis(glm::radians(angle), axis);
  updateTransform();
}

void Object3D::Scale(glm::vec3 size) {
  m_transformProps.scale = size;
  updateTransform();
}

void Object3D::Move(glm::vec3 coord) {
  m_transformProps.position = coord;
  updateTransform();
}

void Object3D::setMeshes(const std::vector<Mesh>& meshes) { m_meshes = meshes; }
void Object3D::setTransformMat(const glm::mat4& m) {
  m_transform_mat = m;
  m_transformProps = Math::decomposeTransformM(m_transform_mat);
}

void Object3D::setInstancingMatArr(const std::vector<glm::mat4>& M) {
  m_instance_mat.clear();
  m_instance_mat = M;
  m_instance_mat.shrink_to_fit();
  m_setMeshInst();
}

void Object3D::setInstancingMatArr(const glm::mat4* M, const size_t arrSize) {
  m_instance_mat.clear();
  m_instance_mat.assign(M, M + arrSize);
  m_instance_mat.shrink_to_fit();
  m_setMeshInst();
}

void Object3D::m_setMeshInst() {
  if (m_instance_mat.empty() || m_meshes.empty()) {
    std::cerr << "Warning no instancing matricies or Meshes!" << std::endl;
    return;
  }
  for (auto& m : m_meshes) {
    m.makeInstanced(std::make_shared<std::vector<glm::mat4>>(m_instance_mat));
  }
}
