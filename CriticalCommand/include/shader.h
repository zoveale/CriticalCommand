#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
  void CreateShaders(unsigned int &vertex,
                     unsigned int &fragment,
                     const char* &vertexShaderCode,
                     const char* &fragmentShaderCode);
  void AttachShaderID(unsigned int& vertex,
                      unsigned int& fragment);


public:
  //Shader ID for glUseProgram
  unsigned int ID;

  Shader(const GLchar* vertexShaderPath,
         const GLchar* fragmentShaderPath);

  //Select Shader ID
  void Use();

  //Uniform datatype shader functions
  
  //
  void Shutdown();
};


#endif // !SHADER_H
