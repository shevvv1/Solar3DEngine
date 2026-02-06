#pragma once

#include <glad/glad.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Object/material.h"

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
  glm::vec2 TexCoord;

  static std::array<VertexAttribute, 3> layout();
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
  glm::vec3 Tangent;

  static std::array<VertexAttribute, 4> layout();
};

enum class DrawMethod { TRIANGLE_SINGLE, TRIANGLE_INSTANCED };
template <typename VertexType>
class Mesh {
 public:
  Mesh();
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices);
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material material);

  //  ~Mesh();
  void Draw(std::shared_ptr<Shader> shader, DrawMethod dMethod = DrawMethod::TRIANGLE_SINGLE);

  void setMaterial(const Material& mat);

  size_t getNumOfVertices() { return vertices.size(); }
  size_t getNumOfIndices() { return indices.size(); }

  void makeInstanced(std::shared_ptr<std::vector<glm::mat4>> tranformMatArr);

 private:
  unsigned int VAO;
  unsigned int VBO, EBO, instanceVBO;
  unsigned m_NumLayouts;
  std::vector<unsigned int> indices;
  std::vector<VertexType> vertices;
  std::shared_ptr<std::vector<glm::mat4>> m_instance_mat = 0;

  Material material;

  void setup_full_mesh();
};

template class Mesh<Vertex>;
template class Mesh<VertexP>;
template class Mesh<VertexPN>;
template class Mesh<VertexPNT>;
