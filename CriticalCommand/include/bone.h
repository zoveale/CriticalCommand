#ifndef BONE_H
#define BONE_H

#include <vector>

#include "model.h"
#include "mat4x4.hpp"

class Bone {
private:
  glm::mat3 ai3x3ToGlmMat3x3(const aiMatrix3x3& load) {
    glm::mat3 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3;
    return store;
  }

  glm::mat4 ai4x4ToGlmMat4x4(const aiMatrix4x4& load) {
    glm::mat4 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3; store[3][0] = load.a4;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3; store[3][1] = load.b4;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3; store[3][2] = load.c4;
    store[0][3] = load.d1; store[1][3] = load.d2; store[2][3] = load.d3; store[3][3] = load.d4;
    return store;
  }

public:
  unsigned int index;
  const char* name;
  glm::mat4x4 offset;

  std::vector<Bone> children;

  glm::mat4x4 localBindTransform;
  glm::mat4x4 invserseBindTransform;

  Bone();
};
#endif // !BONE_H
