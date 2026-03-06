#include <iostream>

#include "Object/ObjectImporter/objectLoader3D.h"
#include "Object/object3D.h"

SkinnedObject3D::SkinnedObject3D(const std::string& path, std::shared_ptr<Shader> shader, Mesh::Type meshType) {
  std::clog << "Loading skinned model:" << path << "\n";
  m_meshType = meshType;
  m_shader = shader;
  Load(path);
  Update();
}

void SkinnedObject3D::Load(const std::string& path) {
  Object3DImport::AssimpLoader loader;
  loader.LoadObject3D(path);
  m_meshes = loader.GetMeshArr();
  m_nodes = loader.GetNodeArr();
  m_bones = loader.GetBoneArr();
  m_animations = loader.GetAnimArr();
  for (auto& m : m_meshes) {
    m.setVertexType(m_meshType);
    m.CreateVAO();
  }
}

void SkinnedObject3D::Draw(DrawMethod dMethod) {
  m_shader->setUniformBool("hasBone", true);
  for (int i = 0; i < m_bones.size(); ++i) {
    m_shader->setUniformMatrix4f("u_Bones[" + std::to_string(i) + "]", m_bones[i].m_finalBoneMatrix);
  }
  for (auto& n : m_nodes) {
    if (n.mesh_i.empty()) continue;
    if (dMethod == DrawMethod::TRIANGLE_SINGLE) {
      m_shader->setUniformMatrix4f("u_MM", m_transform_mat * n.globalTransform);
    } else {
      m_shader->setUniformMatrix4f("u_MM", n.globalTransform);
    }

    m_normal_mat = n.normalMatrix;
    m_shader->setUniformMatrix3f("u_NM", m_normal_mat);

    for (auto& i : n.mesh_i) m_meshes[i].Draw(m_shader, dMethod);
  }
}

void SkinnedObject3D::RunAnimation(float deltaTime) {
  if (m_animations.empty() || m_anim_i == -1) return;
  if (m_anim_time > m_animations[m_anim_i].duration) m_anim_time = 0.0;
  m_anim_time += m_animations[m_anim_i].ticksPerSecond * deltaTime;
  m_anim_time = fmod(m_anim_time, m_animations[m_anim_i].duration);
  m_local_mat = m_calculateHierarhyGlobalMat(0, glm::mat4(1));
}

glm::mat4 SkinnedObject3D::m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat) {
  Node* node = &m_nodes[indx];
  glm::mat4 nodeTransform = node->localTransform;
  if (node->bone_i >= 0) {
    auto it = m_animations[m_anim_i].channels.find(node->name);
    if (it != m_animations[m_anim_i].channels.end()) {
      glm::mat4 T = it->second.interpolatePosition(m_anim_time);
      glm::mat4 R = it->second.interpolateRotation(m_anim_time);
      glm::mat4 S = it->second.interpolateScaling(m_anim_time);
      nodeTransform = T * R * S;
    }
  }
  node->globalTransform = parentMat * nodeTransform;
  glm::mat4 normalMat = glm::transpose(glm::inverse(glm::mat3(node->globalTransform)));

  if (node->bone_i >= 0) {
    m_bones[node->bone_i].m_finalBoneMatrix = node->globalTransform * m_bones[node->bone_i].offsetMatrix;
    normalMat = glm::transpose(glm::inverse(glm::mat3(m_bones[node->bone_i].m_finalBoneMatrix)));
  }
  node->normalMatrix = normalMat;

  for (int child : node->children_i) {
    m_calculateHierarhyGlobalMat(child, node->globalTransform);
  }
  return m_nodes[indx].globalTransform;
}
