#include "modelUtility.h"

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

