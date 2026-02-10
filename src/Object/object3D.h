#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Math/mathUtils.h"
#include "Object/Animation/animator.h"
#include "Object/mesh3D.h"
#include "Render/shader.h"
#include "glm/fwd.hpp"

class Object3D {
 public:
  struct Node {
    std::string name;
    int16_t parent_i = -1;
    std::vector<int16_t> children_i;

    glm::mat4 localTransform;
    glm::mat4 globalTransform;

    std::vector<int16_t> mesh_i;
  };
  Object3D() = default;
  Object3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType = Mesh::Type::VERTEX);
  Object3D(std::vector<Mesh>& mesh_array, std::vector<Object3D::Node> node_array);

  void Draw(DrawMethod dMethod);
  void Update();

  void updateTransform();
  void updateLocal();
  void Rotate(float angle, glm::vec3 axis);
  void Scale(glm::vec3 size);
  void Move(glm::vec3 coord);

  void setMeshes(const std::vector<Mesh>& meshes);
  void setTransformMat(const glm::mat4& m);
  void setRoration(float angle, glm::vec3 axis);

  void setInstancingMatArr(const std::vector<glm::mat4>& M);
  void setInstancingMatArr(const glm::mat4* M, const size_t arrSize);
  void updateInstancingMatArr();

 protected:
  Mesh::Type m_meshType = Mesh::Type::VERTEX;
  std::vector<Mesh> m_meshes;
  std::shared_ptr<Shader> m_shader;

  Math::TransformProps m_transformProps;

  glm::mat4 m_local_mat;
  glm::mat4 m_transform_mat;
  glm::mat3 m_normal_mat;

  std::vector<glm::mat4> m_instance_mat;

  void loadObject3D(std::string const& path);
  glm::mat4 m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat);

  void m_setMeshInst();

 private:
  std::vector<Object3D::Node> m_nodes;
};

class SkinnedObject3D : public Object3D {
 public:
  struct Node : Object3D::Node {
    int16_t bone_i = -1;
  };
  struct Bone {
    int16_t node_i = -1;
    glm::mat4 offsetMatrix;
  };

  SkinnedObject3D() = default;
  SkinnedObject3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType = Mesh::Type::VERTEX);

 private:
  std::vector<SkinnedObject3D::Node> m_nodes;
  std::vector<Bone> m_bones;
  std::vector<glm::mat4> m_finalBoneMatrices;
  std::unordered_map<std::string, Animation> m_animations;
};
