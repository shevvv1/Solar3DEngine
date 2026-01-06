#pragma once

#include "Model/material.h"
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

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

template <typename VertexType> class Mesh {
public:
  Mesh();
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices,
       Material material);
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices);

  //  ~Mesh();
  void Draw(Shader &shader);

private:
  unsigned int VAO;
  unsigned int VBO, EBO;
  std::vector<VertexType> vertices;
  std::vector<unsigned int> indices;
  Material material;

  void setup_full_mesh();
};

template class Mesh<Vertex>;
template class Mesh<VertexP>;
template class Mesh<VertexPN>;
template class Mesh<VertexPNT>;
