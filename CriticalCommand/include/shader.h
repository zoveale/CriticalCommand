#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>



class Shaders {
private:
  unsigned int shader;

  struct ShaderSource {
    std::string vSource;
    std::string fSource;
  };
  ShaderSource sSource;
  static ShaderSource ParseShader(const std::string& filepath);
  static unsigned int ComplieShader(unsigned int type, const std::string& source);
  static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

public:
  Shaders();

  void SetShader(const std::string& filepath);
  void Shutdown();
};
#endif // !SHADER_H
