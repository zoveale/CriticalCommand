#include "skeletal.h"

void VertexBoneData::addVertexBoneData(unsigned int bone_id, float weight) {
  for (unsigned int i = 0; i < sizeof(ids) / sizeof(unsigned int); i++) {

    if (weights[i] == 0.0f) {

      ids[i] = bone_id;
      weights[i] = weight;
      return;
    }
  }
  // should never get here - more bones than we have space for
  /*count++;
  printf("assert(0) %i times\n", count);*/
}

BoneData::BoneData() {
  offsetTransform = glm::mat4(0.0f);
  finalTransform = glm::mat4(0.0f);
}

BoneData::BoneData(const aiMatrix4x4 offset, const aiMatrix4x4 final) {
  this->offsetTransform = aiToGlm(offset);
  this->finalTransform = aiToGlm(final);
}

