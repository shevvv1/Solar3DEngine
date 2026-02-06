#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Math/mathUtils.h"
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
  Object3D(std::string const& path, std::shared_ptr<Shader> shader);
  Object3D(std::vector<Mesh<Vertex>>& mesh_array, std::vector<Node> node_array);

  void Draw(DrawMethod dMethod);
  void Update();

  void updateTransform();
  void updateLocal();
  void Rotate(float angle, glm::vec3 axis);
  void Scale(glm::vec3 size);
  void Move(glm::vec3 coord);

  void setMeshes(const std::vector<Mesh<Vertex>>& meshes);
  void setTransformMat(const glm::mat4& m);
  void setRoration(float angle, glm::vec3 axis);

  void setInstancingMatArr(const std::vector<glm::mat4>& M);
  void setInstancingMatArr(const glm::mat4* M, const size_t arrSize);
  void updateInstancingMatArr();

 private:
  std::vector<Mesh<Vertex>> m_meshes;
  std::vector<Node> m_nodes;
  std::shared_ptr<Shader> m_shader;

  Math::TransformProps m_transformProps;

  glm::mat4 m_local_mat;
  glm::mat4 m_transform_mat;
  glm::mat3 m_normal_mat;

  std::vector<glm::mat4> m_instance_mat;

  void loadObject3D(std::string const& path);
  glm::mat4 m_calculateHierarhyGlobalMat(int indx, glm::mat4 parentMat);

  void m_setMeshInst();
};
