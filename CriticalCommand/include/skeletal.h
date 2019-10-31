#ifndef SKELETAL_H
#define SKELETAL_H
//#include "modelUtility.h"
#include "mat4x4.hpp"
#include "assimp/matrix4x4.h"

#define NUM_BONES_PER_VEREX 4

extern glm::mat4 aiToGlm(const aiMatrix4x4& load);

struct VertexBoneData {
  // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
  unsigned int ids[NUM_BONES_PER_VEREX];
  float weights[NUM_BONES_PER_VEREX];


  void addVertexBoneData(unsigned int bone_id, float weight);
};

struct BoneData {
  glm::mat4 offsetTransform; //<-assimp mOffsetMatrix
  glm::mat4 finalTransform; //
  BoneData();
  BoneData(const aiMatrix4x4 offset, const aiMatrix4x4 final);
};

#endif // !SKELETAL_H

