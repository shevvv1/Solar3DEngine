#pragma once

#include "Model/material.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

class Mesh {
public:
  Mesh();
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       Material material);

  //  ~Mesh();
  void Draw(Shader &shader);

private:
  unsigned int VAO;
  unsigned int VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  void setup_mesh();
};
