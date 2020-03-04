#include "camera.h"
#include "objects.h"

glm::vec3 ThirdPerson::position;
glm::vec3 ThirdPerson::front;
glm::vec3 ThirdPerson::right;
glm::vec3 ThirdPerson::up;
glm::vec3 ThirdPerson::worldUp;
glm::vec3 ThirdPerson::lookAtPosition;

//TODO::fix this stuff


ThirdPerson::ThirdPerson() {
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

void ThirdPerson::StartUp() {
  position = glm::vec3(0.0f);
  front = glm::vec3(0.0f);
  right = glm::vec3(0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  yaw = YAW;
  pitch = PITCH;
  sensitivity = SENSITIVITY;

  //CAMERA_FUNCTIONS::SET_FRONT_VECTOR(*this);
}


void ThirdPerson::Update(GameObject& object) {
  //this->front = object.front;
  this->mouseX = object.look.x;
  this->mouseY = object.look.y;

  float yawChange = this->mouseX * sensitivity;
  float pitchChange = this->mouseY * sensitivity;

  yaw -= yawChange;
  pitch -= pitchChange;


  
  float offsetX = distanceOffset * glm::sin(glm::radians(yaw));
  this->position.x = object.position.x - offsetX;

  float offsetZ = distanceOffset * glm::cos(glm::radians(yaw));
  this->position.z = object.position.z - offsetZ;

  //float offsetY = distanceOffset * glm::sin(glm::radians(pitch));
  this->position.y = object.position.y + distanceOffset;
  


  this->lookAtPosition = object.position - glm::vec3(0.0f, -1.0f, -1.0f);
  this->front = glm::vec3(object.front.x, 0.0f, object.front.z);
  this->right = glm::vec3(object.right.x, 0.0f, object.right.z);


  //cool fixed cam positon effect
  //this->position.x *= glm::sin(glm::radians(pitch));
  //this->position.z *= glm::sin(glm::radians(pitch));

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  //if (constrainPitch) {
 /* if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;*/
  //}

  // Update Front, Right and Up Vectors using the updated Euler angles
  CAMERA_FUNCTIONS::SET_FRONT_VECTOR(*this);
  object.front = glm::vec3(this->front.x, 0.0f, -this->front.z);
  object.right = glm::vec3(-this->right.x, 0.0f, this->right.z);
}

glm::mat4 ThirdPerson::View() {
  return glm::lookAt(position, (lookAtPosition), glm::vec3(0.0f, 1.0f, 0.0f));
}

void CAMERA_FUNCTIONS::SET_FRONT_VECTOR(ThirdPerson& camera) {
  camera.front = glm::vec3(0.0f);
  //calculate the new front vector
  camera.front.x = glm::cos(glm::radians(camera.yaw - 90.0f)) * glm::cos(glm::radians(camera.pitch));
  camera.front.y = glm::sin(glm::radians(camera.pitch));
  camera.front.z = glm::sin(glm::radians(camera.yaw - 90.0f)) * glm::cos(glm::radians(camera.pitch));
  camera.front = glm::normalize(camera.front);

  //calculate the right and up vector
  camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  //camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}
