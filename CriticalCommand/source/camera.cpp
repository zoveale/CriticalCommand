#include "camera.h"

inline void PlayerCamera::Update() {

}

PlayerCamera::PlayerCamera() {
  this->position = &glm::vec3(0.0f, 0.0f, 3.0f);
  this->front = &glm::vec3(0.0f, 0.0f, -1.0f);
  this->right = &glm::vec3(0.0f, 1.0f, 0.0f);
  
  
}

void PlayerCamera::SetTarget(glm::vec3* p, glm::vec3* f, glm::vec3* r) {
  this->position = p;
  this->front = f;
  this->right = r;
  printf("camera position x, y, z : %f, %f, %f\n", 
    this->position->x, this->position->y, this->position->z);

}

glm::mat4 PlayerCamera::CameraView() {
  return 
    glm::lookAt(
      &position,
      &position + front,
      &right
    );
}
