#ifndef CAMERA_H
#define CAMERA_H



#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>
#include "geometric.hpp"

class Player;
class GameObject;

//class Camera;
class FirstPerson;
class ThirdPerson;
class Overview;

namespace CAMERA_FUNCTIONS {
  inline void SET_FRONT_VECTOR(FirstPerson &camera);
  inline void SET_FRONT_VECTOR(ThirdPerson &camera);
  inline void SET_FRONT_VECTOR(Overview &camera);
}

class Camera {
public:
  //Camera() {}
  virtual void Update(Player& player) = 0;
  virtual void Update(GameObject& player) = 0;
  virtual void StartUp() = 0;
  virtual glm::mat4 View() = 0;

  static Overview overview;
  static ThirdPerson thirdPerson;
};

/*

*/
class FirstPerson : public Camera {

public:
  static glm::vec3 position;
  static glm::vec3 front;
  static glm::vec3 right; //right angle axis
  static glm::vec3 up;
  static glm::vec3 worldUp;

  float yaw;
  float pitch;
  float sensitivity;

  float mouseX;
  float mouseY;
 
  FirstPerson();
  void StartUp();
  void Update(Player& player);
  void Update(GameObject& player) {}
  glm::mat4 View();
private:
  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;

};

class ThirdPerson : public Camera {

public:
  static glm::vec3 position;
  static glm::vec3 look;
  static glm::vec3 front;
  static glm::vec3 right; //right angle axis
  static glm::vec3 up;
  static glm::vec3 worldUp;

  float yaw;
  float pitch;
  float sensitivity;

  float mouseX;
  float mouseY;

  ThirdPerson();
  void StartUp();
  void Update(Player& player) {}
  void Update(GameObject& player);
  glm::mat4 View();

private:
  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;

};

class Overview : public Camera {

public:
  Overview();

  static glm::vec3 position;
  static glm::vec3 front;
  static glm::vec3 right; //right angle axis
  static glm::vec3 up;
  static glm::vec3 worldUp;

  float yaw;
  float pitch;
  float sensitivity;

  float mouseX;
  float mouseY;

  void StartUp();
  void Update(Player& player);
  void Update(GameObject& player) {}
  glm::mat4 View();
private:
  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 2.5f;
  const float SENSITIVITY = 0.08f;
  const float ZOOM = 45.0f;

};
#endif //CAMERA_H

