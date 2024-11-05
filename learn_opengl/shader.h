#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int id;

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      v_shader_file.open(vertexPath);
      f_shader_file.open(fragmentPath);
      std::stringstream v_shader_stream, f_shader_stream;
      // read file's buffer contents into streams
      v_shader_stream << v_shader_file.rdbuf();
      f_shader_stream << f_shader_file.rdbuf();
      // close file handlers
      v_shader_file.close();
      f_shader_file.close();
      // convert stream into string
      vertex_code = v_shader_stream.str();
      fragment_code = f_shader_stream.str();
    } catch (std::ifstream::failure &e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char *v_shader_code = vertex_code.c_str();
    const char *f_shader_code = fragment_code.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");
    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use() { glUseProgram(id); }
  // utility uniform functions
  // ------------------------------------------------------------------------
  void set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void set_int(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
  // ------------------------------------------------------------------------
  void set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
  }
  void set_float(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
  }
  void set_mat4(const std::string &name, const glm::mat4 &v) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
  }

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void check_compile_errors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};

#endif // SHADER_H
