#pragma once

#include <glad/glad.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Material/material.h"

struct VertexAttribute {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  size_t offset;
};

#define POS_ATTRIB_I 0
#define NORM_ATTRIB_I 1
#define UI_ATTRIB_I 2
#define TANGENT_ATTRIB_I 3
#define BONE_ATTRIB_I 4
#define WEIGHT_ATTRIB_I 5
#define INSTANCE_ATTRIB_I 6

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
  glm::vec3 Tangent;

  int BoneIDs[MAX_BONE_INFLUENCE] = {-1, -1, -1, -1};
  float Weights[MAX_BONE_INFLUENCE] = {0.0, 0.0, 0.0, 0.0};

  static std::array<VertexAttribute, 4> layout();
};

struct VertexP {
  glm::vec3 position;

  VertexP(const Vertex& v);
  static std::array<VertexAttribute, 1> layout();
};

struct VertexPN {
  glm::vec3 position;
  glm::vec3 Normal;

  VertexPN(const Vertex& v);
  static std::array<VertexAttribute, 2> layout();
};

struct VertexPNT {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;

  VertexPNT(const Vertex& v);
  static std::array<VertexAttribute, 3> layout();
};

struct VertexPNTT {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
  glm::vec3 Tangent;

  VertexPNTT(const Vertex& v);
  static std::array<VertexAttribute, 4> layout();
};

enum class DrawMethod { TRIANGLE_SINGLE, TRIANGLE_INSTANCED };

class Mesh {
 public:
  std::string m_name;
  enum class Type { VERTEXP, VERTEXPN, VERTEXPNT, VERTEX };
  Mesh() = default;
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string& name,
       Type type = Mesh::Type::VERTEX);
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material, std::string& name,
       Type type = Type::VERTEX);

  //  ~Mesh();
  void Draw(std::shared_ptr<Shader> shader, DrawMethod dMethod = DrawMethod::TRIANGLE_SINGLE);
  void CreateVAO();

  void setVertexType(Type type) { m_type = type; }
  void setMaterial(const Material& mat);

  size_t getNumOfVertices() { return vertices.size(); }
  size_t getNumOfIndices() { return indices.size(); }

  void makeInstanced(std::shared_ptr<std::vector<glm::mat4>> tranformMatArr);

 private:
  Type m_type;
  unsigned VAO = 0;
  unsigned VBO = 0, EBO = 0, instanceVBO = 0;
  unsigned m_NumLayouts;
  std::vector<unsigned int> indices;
  std::vector<Vertex> vertices;
  std::shared_ptr<std::vector<glm::mat4>> m_instance_mat = 0;

  Material material;

  template <typename TVertex>
  void m_createVBO() {
    glDeleteBuffers(1, &VBO);
    glGenBuffers(1, &VBO);
    std::vector<TVertex> vert;
    vert.reserve(vertices.size());
    for (const Vertex& v : vertices) {
      vert.emplace_back(TVertex(v));
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(TVertex), vert.data(), GL_STATIC_DRAW);
  }
};
template void Mesh::m_createVBO<VertexP>();
template void Mesh::m_createVBO<VertexPN>();
template void Mesh::m_createVBO<VertexPNT>();
