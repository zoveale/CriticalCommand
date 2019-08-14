#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H



#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>

#include <string>
#include <vector>
#include <map>


#define NUM_BONES_PER_VEREX 4
glm::mat4 aiToGlm(const aiMatrix4x4& load);
glm::mat3 aiToGlm(const aiMatrix3x3& load);
glm::vec3 aiToGlm(const aiVector3D& vec);
glm::quat aiToGlm(const aiQuaternion& quat);

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

struct Lights {
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

struct BoneData {
  glm::mat4 offsetTransform; //<-assimp mOffsetMatrix
  glm::mat4 finalTransform; //
  BoneData();
  BoneData(const aiMatrix4x4 offset, const aiMatrix4x4 final);
};


struct VertexBoneData {
  // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
  unsigned int ids[NUM_BONES_PER_VEREX];   
  float weights[NUM_BONES_PER_VEREX];

 
  void addVertexBoneData(unsigned int bone_id, float weight);
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