/*

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

Shaders::Shaders() {
}

void Shaders::SetShader(const std::string& filepath) {
  if (filepath == "default") {
    printf("Shader = %s\n", filepath.c_str());
    this->sSource = ParseShader("resources/shader/dShader.glsl");
  }
  else {
    printf("Shader = %s\n", filepath.c_str());
    this->sSource = ParseShader(filepath);
  }

  printf("Vertex Shader\n%s\n", this->sSource.vSource.c_str());
  printf("Fragment Shader\n%s\n", this->sSource.fSource.c_str());

  this->shader = CreateShader(this->sSource.vSource, this->sSource.fSource);
  glUseProgram(this->shader);
}

Shaders::ShaderSource Shaders::ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream shaderString[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        //set vertex
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos) {
        //set fragment
        type = ShaderType::FRAGMENT;
      }
    }
    else {
      shaderString[(int)type] << line << '\n';
    }
  }

  return{ shaderString[0].str(), shaderString[1].str() };
}

unsigned int Shaders::ComplieShader(unsigned int type, const std::string& source) {

  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  //TODO:: error handling
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    printf("ERROR: Shader failed to compile: %s\n", message);

    glDeleteShader(id);
    return 0;
  }


  return id;
}

unsigned int Shaders::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = ComplieShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);


  glDetachShader(program, vs);
  glDeleteShader(program);

  return program;
}

void Shaders::Shutdown() {

  glDeleteProgram(this->shader);
}




*/