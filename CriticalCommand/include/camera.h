#ifndef CAMERA_H
#define CAMERA_H



#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>
#include "geometric.hpp"

class Player;

//class GameObject;
//class Camera;
class FirstPerson;
//class OverviewCamera;

//namespace CAMERA_FUNCTIONS {
//  inline void SetView(FirstPerson &camera);
//}

class Camera {
public:
  //Camera();
  virtual void Update(Player& player) = 0;
  static FirstPerson firstPerson;
};

/*

*/
class FirstPerson : public Camera {
private:
  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;
  inline void SetView();
public:
  static glm::vec3 position;
  static glm::vec3 front;
  static glm::vec3 right; //right angle axis
  static glm::vec3 up;
  static glm::vec3 worldUp;

  float mouseX;
  float mouseY;
  //int* mouseScrollValue;
  //mouse stuff
  float yaw;
  float pitch;
  float sensitivity;
  ///

  //private functions
public:
  ///
  
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

