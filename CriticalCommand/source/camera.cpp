#include "camera.h"
glm::vec3 PlayerCamera::position;
glm::vec3 PlayerCamera::front;
glm::vec3 PlayerCamera::right;
glm::vec3 PlayerCamera::up;
glm::vec3 PlayerCamera::worldUp;

inline void PlayerCamera::Update() {
  //calculate the new front vector
  //glm::vec3 direction;

  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  //calculate the right and up vector
  right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  up = glm::normalize(glm::cross(right, front));
}

PlayerCamera::PlayerCamera() {
  
  position = glm::vec3(0.0f, 0.0f, 3.0f);
  front = glm::vec3(0.0f, 0.0f, 0.0f);
  right = glm::vec3(0.0f, 1.0f, 0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  yaw = YAW;
  pitch = PITCH;
  sensitivity = SENSITIVITY;

  Update();
  
}


void PlayerCamera::SetView(Player& player) {
  //player.right = this->right;
  //player.front = this->front;
  //player.AttachCamera()

  this->position = player.position;
  this->front = player.front;
  this->right = player.right;
  
  this->mouseX = player.look.x;
  this->mouseY = player.look.y;

  this->mouseX *= sensitivity;
  this->mouseY *= sensitivity;

  yaw += this->mouseX;
  pitch -= this->mouseY;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  //if (constrainPitch) {
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  //}

  // Update Front, Right and Up Vectors using the updated Euler angles
  Update();
  player.front = this->front;
  player.right = this->right;
}

/*
void Player::AttachCamera(PlayerCamera camera,float xoffset, float yoffset) {

  //FIXME:: dont like this implementation
  front = camera.GetFront();
  right = camera.GetRight();
  camera.SetTarget(position, front, right, xoffset, yoffset);
}
*/
//FIXME:: dont like this implementation
glm::vec3 PlayerCamera::GetFront() {
  return front;
}
//FIXME:: dont like this implementation
glm::vec3 PlayerCamera::GetRight() {
  return right;
}
///
glm::mat4 PlayerCamera::View() {
  //keep camera on the xz plane
  position.y = 0.0f;
  ///
  return glm::lookAt(position, (position + front), up);
}
