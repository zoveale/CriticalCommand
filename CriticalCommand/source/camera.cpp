#include "camera.h"
glm::vec3 FirstPerson::position;
glm::vec3 FirstPerson::front;
glm::vec3 FirstPerson::right;
glm::vec3 FirstPerson::up;
glm::vec3 FirstPerson::worldUp;



inline void FirstPerson::SetView() {
  
  front = glm::vec3(0.0f);
  //calculate the new front vector
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
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

  SetView();
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
  //keep camera on the xz plane
 // position.y = 0.0f;
  ///
  return glm::lookAt(position, (position + front), up);
}
