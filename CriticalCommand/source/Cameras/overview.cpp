#include "camera.h"
#include "player.h"

glm::vec3 Overview::position;
glm::vec3 Overview::front;
glm::vec3 Overview::right;
glm::vec3 Overview::up;
glm::vec3 Overview::worldUp;


Overview::Overview() {
  position = glm::vec3(0.0f);
  front = glm::vec3(0.0f);
  right = glm::vec3(0.0f);
  up = glm::vec3(0.0f);
  worldUp = glm::vec3(0.0f);
  yaw = 0.0f;
  pitch = 0.0f;
  sensitivity = 0.0f;
  mouseX = 0;
  mouseY = 0;
}

void Overview::StartUp() {
  position = glm::vec3(0.0f);
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  right = glm::vec3(0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  yaw = YAW - 150.0f;
  pitch = PITCH;
  sensitivity = SENSITIVITY;

  CAMERA_FUNCTIONS::SET_FRONT_VECTOR(*this);

}


void Overview::Update(Player& player) {
  this->position = player.position;
  this->front = player.front;
  this->right = player.right;

  this->mouseX = player.look.x;
  this->mouseY = player.look.y;

  this->mouseX *= sensitivity;
  this->mouseY *= sensitivity;

  yaw += this->mouseX;
  pitch += this->mouseY;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  //if (constrainPitch) {
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  //}

  // Update Front, Right and Up Vectors using the updated Euler angles
  CAMERA_FUNCTIONS::SET_FRONT_VECTOR(*this);
  player.front = this->front;
  player.right = this->right;
}

glm::mat4 Overview::View() {
  //keep camera on the xz plane
 // position.y = 0.0f;
  ///
  return glm::lookAt(position, (position + front), up);
}

void CAMERA_FUNCTIONS::SET_FRONT_VECTOR(Overview& camera) {
  camera.front = glm::vec3(0.0f);
  //calculate the new front vector
  camera.front.x = glm::cos(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
  camera.front.y = glm::sin(glm::radians(camera.pitch));
  camera.front.z = glm::sin(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
  camera.front = glm::normalize(camera.front);

  //calculate the right and up vector
  camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}
