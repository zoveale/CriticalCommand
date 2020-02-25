#include "camera.h"
#include "player.h"

inline void FirstPerson::SetView() {

  front = glm::vec3(0.0f);
  //calculate the new front vector
  front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  front.y = glm::sin(glm::radians(pitch));
  front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  front = glm::normalize(front);

  //calculate the right and up vector
  right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  up = glm::normalize(glm::cross(right, front));
}

FirstPerson::FirstPerson() {
  position = glm::vec3(0.0f);
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  right = glm::vec3(0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  yaw = YAW - 150.0f;
  pitch = PITCH;
  sensitivity = SENSITIVITY;

  //SetView();
}



void FirstPerson::Update(Player& player) {
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
  SetView();
  player.front = this->front;
  player.right = this->right;
}

glm::mat4 FirstPerson::View() {
  return glm::lookAt(position, (position + front), up);
}

//void CAMERA_FUNCTIONS::SetView(FirstPerson &camera) {
//  
//  camera.front = glm::vec3(0.0f);
//  //calculate the new front vector
//  camera.front.x = glm::cos(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
//  camera.front.y = glm::sin(glm::radians(camera.pitch));
//  camera.front.z = glm::sin(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
//  camera.front = glm::normalize(camera.front);
//
//  //calculate the right and up vector
//  camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//  camera.up = glm::normalize(glm::cross(camera.right, camera.front));
//}

//Camera::Camera() {
//
//}
