#include "shader.h"



Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath) {

  //get shaders file path
  std::string vertexShader;
  std::string fragmentShader;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  // ensure ifstream objects can throw exceptions:
  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    //open files
    vertexShaderFile.open(vertexShaderPath);
    fragmentShaderFile.open(fragmentShaderPath);
    //create string streams to load file data
    std::stringstream vertexShaderStream;
    std::stringstream fragmentShaderStream;
    //load data into streams
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    //close files
    vertexShaderFile.close();
    fragmentShaderFile.close();
    //Convert the shader stream into a string
    vertexShader = vertexShaderStream.str();
    fragmentShader = fragmentShaderStream.str();
  }
  catch (std::ifstream::failure e) {
    printf("Shader file not read succesfully\n");
  }
  //
  const char* vertexShaderCode = vertexShader.c_str();
  const char* fragmentShaderCode = fragmentShader.c_str();

  printf("Vertex Shader\n%s\n", vertexShaderCode);
  printf("Fragment Shader\n%s\n", fragmentShaderCode);

  unsigned int vertex, fragment;
  
  CreateShaders(vertex, fragment, vertexShaderCode, fragmentShaderCode);
  AttachShaderID(vertex, fragment);

  //detach and delete the shaders after memory allocation
  glDetachShader(this->ID, vertex);
  glDetachShader(this->ID, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::Use() {
  glUseProgram(this->ID);
}

void Shader::Shutdown() {
}


///private functions
void Shader::CreateShaders(unsigned int &vertex, unsigned int &fragment,
                           const char* &vertexShaderCode, const char* &fragmentShaderCode) {
  int success;
  char infoLog[512];

  // vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderCode, NULL);
  glCompileShader(vertex);
  // print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  CheckCompileErrors(ID, "VERTEX");

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragment);
  // print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  CheckCompileErrors(ID, "FRAGMENT");

  
}

void Shader::AttachShaderID(unsigned int& vertex, unsigned int& fragment) {
  // shader Program
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glLinkProgram(this->ID);
  CheckCompileErrors(ID, "PROGRAM");

}

void Shader::CheckCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}