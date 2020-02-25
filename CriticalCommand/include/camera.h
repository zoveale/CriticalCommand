#ifndef CAMERA_H
#define CAMERA_H



#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>
#include "geometric.hpp"

class Player;

class GameObject;
class Camera;
class FirstPerson;
class OverviewCamera;

namespace CAMERA_FUNCTIONS {
  inline void SetView(Camera& camera);
}

class Camera {
public:
  Camera();
  virtual void Update(Player& player) = 0;
  static FirstPerson firstPerson;

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right; //right angle axis
  glm::vec3 up;
  glm::vec3 worldUp;

  float mouseX;
  float mouseY;
  //int* mouseScrollValue;

  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;

  //mouse stuff
  float yaw;
  float pitch;
  float sensitivity;
  
};

/*

*/
class FirstPerson : public Camera {
private:
  
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right; //right angle axis

  glm::vec3 worldUp;

  float mouseX;
  float mouseY;
  //int* mouseScrollValue;

  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;

  //mouse stuff
  float yaw;
  float pitch;
  float sensitivity;
  ///

  //private functions

  ///
  glm::vec3 up;
  FirstPerson();
  void SetUp();
  void Update(Player& player);
  glm::mat4 View();
};

//class ThirdPerson : public Camera {
//public:
//  glm::vec3 up;
//  ThirdPerson();
//  void SetUp();
//  void Update(GameObject& object);
//  glm::mat4 View();
//};
//
//class OverviewCamera : public Camera {
//  /*
//  
//  */
//};

#endif //CAMERA_H

