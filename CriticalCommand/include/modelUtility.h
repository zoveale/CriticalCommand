#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H

#include <string>
#include "vec2.hpp"
#include "vec3.hpp"
#include "joint.h"

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

struct Joints {
  Joint root;
  unsigned int jointCount;

};


#endif // !MODEL_UTILITY_H