#include "shader.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

std::string extract_string_beetween(const std::string &input, char start_symbol,
                                    char end_symbol) {
  size_t start_index = input.find(start_symbol);
  size_t end_index = input.find(end_symbol, start_index + 1);
  std::string extracted_string = "";
  if (start_index != std::string::npos && end_index != std::string::npos) {
    extracted_string =
        input.substr(start_index + 1, end_index - start_index - 1);
  } else {
    std::cout << "ERROR [ ShaderLoader::extract_first_between ]: Start '"
              << start_symbol << "' or end symbol '" << end_symbol
              << "' not found" << std::endl;
  }
  return extracted_string;
}

std::string ShaderLoader::GetShaderSrcFromFile(const std::string &path) {

  //  std::cout << path << std::endl;
  if (m_includes.empty()) {
    m_rootDir = path.substr(0, path.find_last_of('/'));
  }
  for (auto checkPath : m_includes) {
    if (checkPath == path) {
      std::cout << "ERROR::Shader include path already included! --> " << path
                << std::endl;
      return "";
    }
  }
  m_includes.push_back(path);
  std::ifstream shaderFile(path);
  std::string shader_src = "";
  if (shaderFile.is_open()) {
    std::string line;
    std::string includeKey = "#include";
    while (getline(shaderFile, line)) {
      if (line.find(includeKey) == 0) {
        std::string includePath =
            m_rootDir + '/' +
            extract_string_beetween(line.substr(includeKey.length()), '"', '"');
        shader_src += GetShaderSrcFromFile(includePath);
      } else {
        shader_src += line + "\n";
      }
    }
    shaderFile.close();
    return shader_src;
  } else {
    std::cerr << "Error wrong path to the shaders!!!" << std::endl;
    shaderFile.close();
    return "";
  }
}

Shader::Shader(const unsigned int id) { ID = id; }

Shader::Shader(const std::string &VertexPath, const std::string &FragmentPath) {
  ID = ShaderLoader().GenerateShaderProgramID(VertexPath, FragmentPath);
}

void Shader::HotReload(const std::string &VertexPath,
                       const std::string &FragmentPath) {
  ID = ShaderLoader().GenerateShaderProgramID(VertexPath, FragmentPath);
}

void Shader::setUniformInt(const std::string name, int a) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), a);
}

void Shader::setUniformBool(const std::string name, bool a) {
  int num = static_cast<int>(a);
  glUniform1i(glGetUniformLocation(ID, name.c_str()), num);
}

void Shader::setUniformFloat(const std::string name, float a) {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), a);
}

void Shader::setUniformFloat3(const std::string name, float a, float b,
                              float c) {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), a, b, c);
}

void Shader::setUniformFloat3(const std::string name, float n[3]) {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), n[0], n[1], n[2]);
}

void Shader::setUniformFloat4(const std::string name, float a, float b, float c,
                              float d) {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), a, b, c, d);
}

void Shader::setUniformVec3f(const std::string name, glm::vec3 vector) {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,
               glm::value_ptr(vector));
}

void Shader::setUniformVec4f(const std::string name, glm::vec4 vector) {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1,
               glm::value_ptr(vector));
}
void Shader::setUniformMatrix4f(const std::string name, glm::mat4 mtrx) {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mtrx));
}

void Shader::Activate() { glUseProgram(ID); }

void Shader::Delete() { glDeleteProgram(ID); }

// SHADER_LOADER
Shader ShaderLoader::loadShaderProgram(const std::string &VertexPath,
                                       const std::string &FragmentPath) {
  unsigned int ID = glCreateProgram();
  unsigned int VertexSh =
      CompileShader(GL_VERTEX_SHADER, GetShaderSrcFromFile(VertexPath));
  unsigned int FragmentSh =
      CompileShader(GL_FRAGMENT_SHADER, GetShaderSrcFromFile(FragmentPath));

  glAttachShader(ID, VertexSh);
  glAttachShader(ID, FragmentSh);
  glLinkProgram(ID);

  int success;
  glGetShaderiv(ID, GL_LINK_STATUS, &success);

  if (!success) {
    int length;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
    char infoLog[length];
    glGetShaderInfoLog(ID, length, NULL, infoLog);
    std::cerr << "ERROR::LINKING PROGRAM FAILED!\n" << infoLog << std::endl;
  }

  glDeleteShader(VertexSh);
  glDeleteShader(FragmentSh);

  return Shader(ID);
}

unsigned int
ShaderLoader::GenerateShaderProgramID(const std::string &VertexPath,
                                      const std::string &FragmentPath) {
  unsigned int ID = glCreateProgram();
  unsigned int VertexSh =
      CompileShader(GL_VERTEX_SHADER, GetShaderSrcFromFile(VertexPath));
  unsigned int FragmentSh =
      CompileShader(GL_FRAGMENT_SHADER, GetShaderSrcFromFile(FragmentPath));

  glAttachShader(ID, VertexSh);
  glAttachShader(ID, FragmentSh);
  glLinkProgram(ID);

  int success;
  glGetShaderiv(ID, GL_LINK_STATUS, &success);

  if (!success) {
    int length;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
    char infoLog[length];
    glGetShaderInfoLog(ID, length, NULL, infoLog);
    std::cerr << "ERROR::LINKING PROGRAM FAILED!\n" << infoLog << std::endl;
  }

  glDeleteShader(VertexSh);
  glDeleteShader(FragmentSh);

  return ID;
}

unsigned int ShaderLoader::CompileShader(const GLenum type,
                                         const std::string &source) {
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char infoLog[length];
    glGetShaderInfoLog(id, length, NULL, infoLog);
    std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return id;
}
