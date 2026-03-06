#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Math/mathUtils.h"
#include "Object/Animation/animator.h"
#include "Object/mesh3D.h"
#include "Render/shader.h"
#include "glm/fwd.hpp"
struct Node {
  std::string name;
  int16_t parent_i = -1;
  std::vector<int16_t> children_i;

  int16_t bone_i = -1;

  glm::mat4 localTransform;
  glm::mat4 globalTransform;
  glm::mat4 normalMatrix;

  std::vector<int16_t> mesh_i;
};

class Object3D {
 public:
  Object3D() = default;
  Object3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType = Mesh::Type::VERTEX);
  Object3D(std::vector<Mesh>& mesh_array, std::vector<Node> node_array);

  void Load(const std::string& path, const std::vector<std::string>& MatTextureList,
            const std::vector<std::string>& MatColorList, const std::vector<std::string>& MatPropList);
  void Load(const std::string& path);

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

 protected:
  Mesh::Type m_meshType = Mesh::Type::VERTEX;
  std::vector<Mesh> m_meshes;
  std::vector<Node> m_nodes;
  std::shared_ptr<Shader> m_shader;

  Math::TransformProps m_transformProps;

  glm::mat4 m_local_mat;
  glm::mat4 m_transform_mat;
  glm::mat3 m_normal_mat;

  std::vector<glm::mat4> m_instance_mat;

  glm::mat4 m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat);

  void m_setMeshInst();
};

class SkinnedObject3D : public Object3D {
 public:
  struct Bone {
    std::string name;
    glm::mat4 offsetMatrix;
    glm::mat4 m_finalBoneMatrix;
  };

  SkinnedObject3D() = default;
  SkinnedObject3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType = Mesh::Type::VERTEX);

  void Load(std::string const& path);

  void Draw(DrawMethod dMethod);
  void setAnimation(size_t indx) { m_anim_i = indx; }
  void RunAnimation(float deltaTime);

 private:
  std::vector<Bone> m_bones;
  std::vector<Animation> m_animations;
  int m_anim_i = -1;
  int m_anim_time = 0;

  glm::mat4 m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat);
};
