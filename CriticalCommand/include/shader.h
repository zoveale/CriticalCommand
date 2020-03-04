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
#include "gtc/type_ptr.hpp"



class Shader {
public:
  
  
  Shader();

  //Load from Shader files
  Shader(const GLchar* vertexShaderPath,
         const GLchar* fragmentShaderPath, 
         const GLchar* geometryShaderPath = nullptr);
  void Load(const GLchar* vertexShaderPath,
    const GLchar* fragmentShaderPath,
    const GLchar* geometryShaderPath = nullptr);

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
  void SetVec2(const std::string& name,
               float x, float y) const;

  void SetFloat(const std::string& name, float f) const;
  void SetUnsignedInt(const std::string& name, unsigned int ui) const;
  void SetInt(const std::string& name, int i) const;
  
  unsigned int GetUniform(const std::string& name);
  ///
  void Shutdown();

  //Shader ID for glUseProgram
  unsigned int ID;
private:


  void CreateShaders(unsigned int& vertex,
    unsigned int &fragment,
    unsigned int &geometry,
    const char*  &vertexShaderCode,
    const char*  &fragmentShaderCode,
    const char*  &geometryShaderCode);

  void AttachShaderID(unsigned int& vertex,
    unsigned int& fragment,
    unsigned int& geometry);
  void CheckCompileErrors(GLuint shader, std::string type);

  std::string LoadShader(const GLchar* shaderPath);

  std::string vertexShader;
  std::string fragmentShader;
  std::string geometryShader;
  const char* vertexShaderCode;
  const char* fragmentShaderCode;
  const char* geometryShaderCode;

  bool geometryShaderBit;
};


#endif // !SHADER_H
