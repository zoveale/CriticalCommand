#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "mat4x4.hpp"
#include "vec3.hpp"




class Shader {
private:
  void CreateShaders(unsigned int &vertex,
                     unsigned int &fragment,
                     const char* &vertexShaderCode,
                     const char* &fragmentShaderCode);
  void AttachShaderID(unsigned int& vertex,
                      unsigned int& fragment);
  void CheckCompileErrors(GLuint shader, std::string type);

  std::string LoadShader(const GLchar* shaderPath);

  std::string vertexShader;
  std::string fragmentShader;
  const char* vertexShaderCode;
  const char* fragmentShaderCode;
  
public:
  //Shader ID for glUseProgram
  unsigned int ID;
  
  Shader();

  //Load from Shader files
  Shader(const GLchar* vertexShaderPath,
         const GLchar* fragmentShaderPath);

  //Select Shader ID
  void Use();
  void Print();
  //Uniform datatype shader functions
  //void SetPVM(const glm::vec3& projection, const glm::vec3& view, const glm::vec3& model);
  void SetInit(const std::string& name, int value) const;

  void SetMat4(const std::string& name, const glm::mat4& mat) const;
  void SetMat4(unsigned int name, const glm::mat4& mat) const;

  void SetVec3(const std::string& name,
               const glm::vec3& vec3) const;
  void SetVec3(const std::string& name,
               float x, float y, float z) const;

  void SetFloat(const std::string& name, float f) const;

  unsigned int GetUniform(const std::string& name);
  ///
  void Shutdown();
};


#endif // !SHADER_H
