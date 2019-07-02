#ifndef BONE_H
#define BONE_H

#include <vector>

#include "mat4x4.hpp"

class Joint {
private:
  void InverseBindMatrix(glm::mat4 parentBindTransform) {
    glm::mat4 bindTransform = glm::matrixCompMult(parentBindTransform, bindTransform);
    invserseBindTransform = glm::inverse(bindTransform);

    for (int i = 0; i < children.size(); i++) {
      InverseBindMatrix(children[i].localBindTransform);
    }

  }

public:
  unsigned int index;
  const char* name;
  glm::mat4 offset;

  std::vector<Joint> children;

  glm::mat4 animateTransform;

  glm::mat4 localBindTransform;
  glm::mat4 invserseBindTransform;

  Joint() :index(0), name(""), offset(glm::mat4(1.0)), children(NULL),
    localBindTransform(glm::mat4(1.0)), invserseBindTransform(glm::mat4(1.0)) {}

  Joint(unsigned int index, const char* name, glm::mat4 offset) {
    this->index = index;
    this->name = name;
    this->offset = offset;
  }
  
};
#endif // !BONE_H
