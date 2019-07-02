#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <string>
#include "mat4x4.hpp"

class Joint {
private:
  
public:
  void InverseBindMatrix(glm::mat4 parentBindTransform) {
    glm::mat4 bindTransform = glm::matrixCompMult(parentBindTransform, localBindTransform);
    invserseBindTransform = glm::inverse(bindTransform);

    for (unsigned int i = 0; i < children.size(); i++) {
      InverseBindMatrix(children[i].localBindTransform);
    }

  }
  unsigned int index;
  std::string name;
  glm::mat4 offset;

  std::vector<Joint> children;

  glm::mat4 animateTransform;

  glm::mat4 localBindTransform;
  glm::mat4 invserseBindTransform;

  Joint() :index(0), name(""), offset(glm::mat4(1.0)), children(NULL),
           animateTransform(glm::mat4(1.0)),
           localBindTransform(glm::mat4(1.0)), 
           invserseBindTransform(glm::mat4(1.0)) {}

  Joint(unsigned int index, std::string name, glm::mat4 offset) {
    this->index = index;
    this->name = name;
    this->offset = offset;
  }
  
};
#endif // !JOINT_H
