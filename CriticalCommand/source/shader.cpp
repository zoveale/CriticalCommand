#include "shader.h"



Shader::Shader() {
  ///
}

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath) {

  
  this->vertexShader = LoadShader(vertexShaderPath);
  this->fragmentShader = LoadShader(fragmentShaderPath);

  this->vertexShaderCode = vertexShader.c_str();
  this->fragmentShaderCode = fragmentShader.c_str();

  
  //Print();
  unsigned int vertex, fragment;
  
  CreateShaders(vertex, fragment, vertexShaderCode, fragmentShaderCode);
  AttachShaderID(vertex, fragment);

  //detach and delete the shaders after memory allocation
  glDetachShader(this->ID, vertex);
  glDetachShader(this->ID, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::SetInit(const std::string& name, int value) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1i(location, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetMat4(unsigned int name, const glm::mat4& mat) const {
  //int location = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(name, 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& vec3) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform3fv(location, 1, &vec3[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform3f(location, x, y, z);
}

void Shader::SetFloat(const std::string& name, float f) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1f(location, f);
}

unsigned int Shader::GetUniform(const std::string& name) {
  return glGetUniformLocation(this->ID, name.c_str());
}




void Shader::Use() {
  glUseProgram(this->ID);
}

void Shader::Print() {
  printf("Vertex Shader\n%s\n", vertexShaderCode);
  printf("Fragment Shader\n%s\n", fragmentShaderCode);
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
  //Attach shader Program
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
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
        << "\n" << infoLog << "\n -- --------------------------------------------------- -- " 
        << std::endl;
    }
  }
  else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type 
        << "\n" << infoLog << "\n -- --------------------------------------------------- -- " 
        << std::endl;
    }
  }
}

std::string Shader::LoadShader(const GLchar* shaderPath) {
  std::string shader;
  std::ifstream shaderFile;
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shaderFile.open(shaderPath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shader = shaderStream.str();
  }
  catch (std::ifstream::failure e) {
    printf("Shader file not read succesfully\n");
  }

  return  shader;
}