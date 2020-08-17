#include "modelUtility.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"


glm::mat3 aiToGlm(const aiMatrix3x3& load) {
  glm::mat3 store;
  //row major to column major
  store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3;
  store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3;
  store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3;
  return store;
}

glm::mat4 aiToGlm(const aiMatrix4x4& load) {
  glm::mat4 store;
  //row major to column major
  store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3; store[3][0] = load.a4;
  store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3; store[3][1] = load.b4;
  store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3; store[3][2] = load.c4;
  store[0][3] = load.d1; store[1][3] = load.d2; store[2][3] = load.d3; store[3][3] = load.d4;
  //FIXME:: change to transposed glm
  //store = glm::transpose(glm::make_mat4(&load));
  /*for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      store[i][j] = load[i][j];
    }
  }*/
  return store;
}

glm::vec3 aiToGlm(const aiVector3D& vec) {
  /*glm::vec3 store;
  store = glm::vec3(vec.x, vec.y, vec.z);*/
  return glm::vec3(vec.x, vec.y, vec.z);
}
glm::quat aiToGlm(const aiQuaternion& quat) {
  /*glm::quat store;
  store = glm::quat(quat.x, quat.y, quat.z, quat.w);*/
  return glm::quat(quat.w, quat.x, quat.y, quat.z);
}





static unsigned char* threadData[5];
int Texture::Load(const char* path, const std::string& directory, bool gamma) {
  std::string filename{ path };
  filename = directory + '/' + filename;
  
  
  data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  //TODO:: mutex lock
  if (data) {
    id = 1;
    return 1;
  }
  else {
    printf("Texture failed to load at path: %s \n", filename.c_str());
    stbi_image_free(data);
    return -1;
  }
}



unsigned int Texture::SendToGPU() {

  GLenum internalFormat = GL_RED;
  GLenum format = 0;
  if (nrComponents == 1)
    internalFormat = format = GL_RED;
  else if (nrComponents == 3) {
    internalFormat = GL_SRGB;
    format = GL_RGB;
  }
  else if (nrComponents == 4) {
    internalFormat = GL_SRGB_ALPHA;
    format = GL_RGBA;
  }

  glActiveTexture(GL_TEXTURE0);
  unsigned int textureID;
  glGenTextures(1, &textureID);


  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  return textureID;
}

unsigned int Texture::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrComponents;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
        width, height, 0, GL_RGB, GL_FLOAT, nullptr);
      stbi_image_free(data);
    }
    else {
      printf("Cubemap texture failed to load at path: %s\n", faces[i].c_str());
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return textureID;
}

unsigned int Texture::LoadHDR(const char* path) {
  int width, height, nrComponents;
  float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);
  unsigned int hdrTexture = 0;
  if (data) {
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else {
    printf("HDR texture failed to load at path: %s\n", path);
  }
  return hdrTexture;
}
