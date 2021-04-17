#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H



#include <GL/glew.h>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/matrix_decompose.hpp>

#include <string>
#include <vector>
#include <map>


extern glm::mat3 aiToGlm(const aiMatrix3x3& load);
extern glm::mat4 aiToGlm(const aiMatrix4x4& load);
extern glm::vec3 aiToGlm(const aiVector3D& vec);
extern glm::quat aiToGlm(const aiQuaternion& quat);




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

  Vertex():Position(glm::vec3(0.0f)), 
    Normal(glm::vec3(0.0f)),
    TexCoords(glm::vec2(0.0f)),
    Tangent(glm::vec3(0.0f)),
    Bitangent(glm::vec3(0.0f)){ }
};

struct Lights {
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
};



struct Texture {
  int id;
  std::string type;
  std::string path;
  unsigned char* data;
  int width;
  int height;
  int nrComponents;
  GLenum format;

  int Load(const char* path,
                      const std::string& directory,
                      bool gamma = false);
  
  unsigned int SendToGPU();

  static unsigned int loadCubemap(std::vector<std::string> faces);
  static unsigned int LoadHDR(const char* path);

  
};







//void PrintAnimationInfo(const aiScene* scene) {
//  
//  printf("animation amount : %i\n", scene->mNumAnimations);
//  for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
//    printf("mTicksPerSecond[%i]: %f\t", i, scene->mAnimations[i]->mTicksPerSecond);
//    printf("mDuration[%i]: %f\n", i, scene->mAnimations[i]->mDuration);
//  }
//  
//  for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
//    printf("animation name : %s\n", scene->mAnimations[i]->mName.data);
//    printf("animation channels : %i", scene->mAnimations[i]->mNumChannels);
//    for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
//      printf("\nanimation node : %s\n", scene->mAnimations[i]->mChannels[j]->mNodeName.data);
//      printf("\t num position keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumPositionKeys);
//      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
//        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x);
//        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y);
//        printf(" z: %f\n", scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z);
//      }
//      printf("\n\t num rotation keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumRotationKeys);
//      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
//        //printf("\t keys: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k]);
//        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x);
//        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y);
//        printf(" z: %f", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z);
//        printf(" w: %f\n", scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w);
//      }
//      printf("\n\t num scaleing keys: %i\n", scene->mAnimations[i]->mChannels[j]->mNumScalingKeys);
//      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
//        //printf("\t keys: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k]);
//        printf("\t\t x: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x);
//        printf(" y: %f", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y);
//        printf(" z: %f\n", scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z);
//
//      }
//    }
//  }
//}
#endif // !MODEL_UTILITY_H