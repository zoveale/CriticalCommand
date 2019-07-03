#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>

#include "joint.h"

#define NUM_BONES_PER_VEREX 4
glm::mat4 ai4x4ToGlm4x4(const aiMatrix4x4& load);
glm::mat3 ai3x3ToGlm3x3(const aiMatrix3x3& load);

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};


struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

struct BoneData {
  glm::mat4 offsetTransform; //<-assimp mOffsetMatrix
  glm::mat4 finalTransform; //<-assimp 
  BoneData() {
    offsetTransform = glm::mat4(1.0);
    finalTransform = glm::mat4(1.0);
  }
  BoneData(const aiMatrix4x4 offset, const aiMatrix4x4 final) {
    this->offsetTransform = ai4x4ToGlm4x4(offset);
    this->finalTransform = ai4x4ToGlm4x4(final);
  }
};


struct VertexBoneData {
  // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
  unsigned int ids[NUM_BONES_PER_VEREX];   
  float weights[NUM_BONES_PER_VEREX];

  VertexBoneData() {
    weights[0] = 0.0f;
    weights[1] = 0.0f;
    weights[2] = 0.0f;
    weights[3] = 0.0f;
    ids[0] = 0;
    ids[1] = 0;
    ids[2] = 0;
    ids[3] = 0;
  }

  void addVertexBoneData(unsigned int bone_id, float weight) {
    for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++) {
      if (weights[i] == 0.0f) {
        ids[i] = bone_id;
        weights[i] = weight;
        return;
      }
    }
  }
};


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

glm::mat3 ai3x3ToGlm3x3(const aiMatrix3x3& load) {
  glm::mat3 store;
  //row major to column major
  store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3;
  store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3;
  store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3;
  return store;
}

glm::mat4 ai4x4ToGlm4x4(const aiMatrix4x4& load) {
  glm::mat4 store;
  //row major to column major
  store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3; store[3][0] = load.a4;
  store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3; store[3][1] = load.b4;
  store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3; store[3][2] = load.c4;
  store[0][3] = load.d1; store[1][3] = load.d2; store[2][3] = load.d3; store[3][3] = load.d4;
  return store;
}

glm::vec3 aiVec3ToGlmVec4(const aiVector3D& vec){
  /*glm::vec3 store;
  store = glm::vec3(vec.x, vec.y, vec.z);*/
  return glm::vec3(vec.x, vec.y, vec.z);
}
glm::quat aiQuatToGlmQuat(const aiQuaternion& quat) {
  /*glm::quat store;
  store = glm::quat(quat.x, quat.y, quat.z, quat.w);*/
  return glm::quat(quat.x, quat.y, quat.z, quat.w);
}
void PrintAnimationInfo(const aiScene* scene) {
  printf("animation amount : %i\n", scene->mNumAnimations);
  for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
    printf("animation channels : %i", scene->mAnimations[i]->mNumChannels);
    for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
      printf("\nanimation node : %s\n", scene->mAnimations[i]->mChannels[j]->mNodeName.data);
      printf("\t num position keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumPositionKeys);
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x);
        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y);
        printf(" z: %f\n", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z);
      }
      printf("\n\t num rotation keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumRotationKeys);
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
        //printf("\t keys: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k]);
        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x);
        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y);
        printf(" z: %f\n", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z);
      }
      printf("\n\t num scaleing keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumScalingKeys);
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
        //printf("\t keys: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k]);
        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x);
        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y);
        printf(" z: %f\n", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z);

      }
    }
  }
}
#endif // !MODEL_UTILITY_H