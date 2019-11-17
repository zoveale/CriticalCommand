#include "shader.h"



Shader::Shader() {
  geometryShaderBit = false;
}

Shader::Shader(const GLchar* vertexShaderPath,
               const GLchar* fragmentShaderPath,
               const GLchar* geometryShaderPath) {

  
  this->vertexShader = LoadShader(vertexShaderPath);
  this->vertexShaderCode = vertexShader.c_str();

  this->fragmentShader = LoadShader(fragmentShaderPath);
  this->fragmentShaderCode = fragmentShader.c_str();

  if (geometryShaderPath != nullptr) {
    this->geometryShader = LoadShader(geometryShaderPath);
    this->geometryShaderCode = geometryShader.c_str();
    geometryShaderBit = true;
  }
  
  
  //Print();
  
  unsigned int vertex, fragment, geometry;
  CreateShaders(vertex, fragment, geometry, vertexShaderCode, fragmentShaderCode, geometryShaderCode);
  AttachShaderID(vertex, fragment, geometry);

  //detach and delete the shaders after memory allocation
  glDetachShader(this->ID, vertex);
  glDetachShader(this->ID, fragment);
  glDetachShader(this->ID, geometry);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  glDeleteShader(geometry);
}

void Shader::SetInit(const std::string& name, int value) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1i(location, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetMat4(unsigned int name, const glm::mat4& mat) const {
  //int location = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(name, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetVec3(const std::string& name, const glm::vec3& vec3) const {
  //int location = glGetUniformLocation(ID, name.c_str());
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec3));
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
  //int location = glGetUniformLocation(ID, name.c_str());
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetFloat(const std::string& name, float f) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1f(location, f);
}

void Shader::SetUnsignedInt(const std::string& name, unsigned int ui) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1ui(location, ui);
}

void Shader::SetInt(const std::string& name, int i) const {
  int location = glGetUniformLocation(ID, name.c_str());
  glUniform1i(location, i);
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
void Shader::CreateShaders(unsigned int &vertex,
                           unsigned int &fragment,
                           unsigned int &geometry,
                           const char* &vertexShaderCode, 
                           const char* &fragmentShaderCode, 
                           const char* &geometryShaderCode) {
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

  // geometry Shader
  if (geometryShaderBit) {
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometryShaderCode, NULL);
    glCompileShader(geometry);
    // print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    CheckCompileErrors(ID, "GEOMETRY");
  }
}

void Shader::AttachShaderID(unsigned int& vertex, unsigned int& fragment, unsigned int& geometry) {
  //Attach shader Program
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glAttachShader(this->ID, geometry);
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