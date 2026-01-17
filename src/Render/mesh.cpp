#include "mesh.h"

#include "glad/glad.h"

template <typename VertexType>
Mesh<VertexType>::Mesh() {
  setup_full_mesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material material) {
  this->vertices = vertices;
  this->indices = indices;
  this->material = material;
  setup_full_mesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material& material,
                       BoundBox& bb) {
  this->vertices = vertices;
  this->indices = indices;
  this->material = material;
  this->m_boundBox = bb;
  setup_full_mesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices) {
  this->vertices = vertices;
  this->indices = indices;
  setup_full_mesh();
}

/*Mesh::~Mesh(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
}*/

template <typename VertexType>
void Mesh<VertexType>::setup_full_mesh() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);

  if (!indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    size_t sizeEBO = sizeof(glm::vec3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  }
  // Specify vertex attributes
  const auto& layout = VertexType::layout();
  for (const auto& attr : layout) {
    glEnableVertexAttribArray(attr.index);
    glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride,
                          reinterpret_cast<void*>(attr.offset));
  }
  glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::Draw(Shader& shader) {
  glBindVertexArray(VAO);

  if (!material.Empty()) {
    material.Bind(shader);
  }
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

  unbindTextures();
  glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::setBoundBox(BoundBox bb) {
  m_boundBox = bb;
}

std::array<VertexAttribute, 5> Vertex::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)},
           {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent)},
           {4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Bitangent)}}};
}

std::array<VertexAttribute, 1> VertexP::layout() {
  return {{0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), offsetof(VertexP, position)}};
}

std::array<VertexAttribute, 2> VertexPN::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)}}};
}

std::array<VertexAttribute, 3> VertexPNT::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)}}};
}
