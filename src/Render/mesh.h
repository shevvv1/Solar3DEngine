#pragma once

#include <glad/glad.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Model/material.h"

struct VertexAttribute {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  size_t offset;
};

struct VertexP {
  glm::vec3 position;

  static std::array<VertexAttribute, 1> layout();
};

struct VertexPN {
  glm::vec3 position;
  glm::vec3 Normal;

  static std::array<VertexAttribute, 2> layout();
};

struct VertexPNT {
  glm::vec3 position;
  glm::vec3 Normal;

  static std::array<VertexAttribute, 3> layout();
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;

  static std::array<VertexAttribute, 5> layout();
};

struct BoundBox {
  glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
  glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());
  glm::vec3 center = glm::vec3(0.0f);
  glm::vec3 size = glm::vec3(0.0f);
  float maxDimension = 0.0f;

  void update(const glm::vec3& position) {
    min = glm::min(min, position);
    max = glm::max(max, position);
    update();
  }

  void update() {
    center = (min + max) * 0.5f;
    size = max - min;
    maxDimension = glm::max(size.x, glm::max(size.y, size.z));
  }
};

template <typename VertexType>
class Mesh {
 public:
  Mesh();
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material material);
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material& material, BoundBox& bb);
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices);

  //  ~Mesh();
  void Draw(Shader& shader);

  void setBoundBox(BoundBox bb);

  BoundBox getBoundBox() { return m_boundBox; }

 private:
  unsigned int VAO;
  unsigned int VBO, EBO;
  std::vector<unsigned int> indices;
  std::vector<VertexType> vertices;

  Material material;
  BoundBox m_boundBox;

  void setup_full_mesh();
};

template class Mesh<Vertex>;
template class Mesh<VertexP>;
template class Mesh<VertexPN>;
template class Mesh<VertexPNT>;
