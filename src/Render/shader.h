#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
  Shader(const std::string &VertexPath, const std::string &FragmentPath);
  Shader(const unsigned int ID);
  ~Shader() { Delete(); }

  unsigned int GetID() { return ID; }

  void setUniformInt(const std::string name, int a);
  void setUniformBool(const std::string name, bool a);
  void setUniformFloat(const std::string name, float a);
  void setUniformFloat3(const std::string name, float a, float b, float c);
  void setUniformFloat3(const std::string name, float n[3]);
  void setUniformFloat4(const std::string name, float a, float b, float c,
                        float d);
  void setUniformFloat4(const std::string name, float n[4]);
  void setUniformVec3f(const std::string name, glm::vec3 vector);
  void setUniformVec4f(const std::string name, glm::vec4 vector);
  void setUniformMatrix4f(const std::string name, glm::mat4 mtrx);
  void setUniformMatrix3f(const std::string name, glm::mat3 mtrx);

  void HotReload();
  void Activate();
  void Delete();

private:
  unsigned int ID;
  std::string m_VertexSrcPath;
  std::string m_FragmentSrcPath;
};

class ShaderLoader {
public:
  ShaderLoader() = default;

  Shader loadShaderProgram(const std::string &VertexPath,
                           const std::string &FragmentPath);
  unsigned int GenerateShaderProgramID(const std::string &VertexPath,
                                       const std::string &FragmentPath);

private:
  std::vector<std::string> m_includes;
  std::string m_rootDir;

  std::string GetShaderSrcFromFile(const std::string &path);
  unsigned int CompileShader(const GLenum type, const std::string &source);
};
