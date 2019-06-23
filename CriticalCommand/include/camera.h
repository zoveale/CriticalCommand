#ifndef CAMERA_H
#define CAMERA_H


#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>
#include "geometric.hpp"

#include "player.h"
class PlayerCamera;
//class Player;

class Camera {
public:
  /*virtual void SetTarget(glm::vec3 p, glm::vec3 f, glm::vec3 r,
    float xoffset, float yoffset) = 0;*/

  virtual void SetView(Player& player) = 0;

  static PlayerCamera camera;
};

class PlayerCamera : public Camera {
private:
  static glm::vec3 position;
  static glm::vec3 front;
  static glm::vec3 right; //right angle axis
  static glm::vec3 up;
  static glm::vec3 worldUp;

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
  inline void Update();
  ///
public:
  PlayerCamera();
  /*void SetTarget(glm::vec3 p, glm::vec3 f, glm::vec3 r, 
                 float xoffset, float yoffset);*/

  void SetView(Player& player);



  ////FIXME:: dont like this implementation
  //glm::vec3 GetFront();
  //glm::vec3 GetRight();
  /////
  glm::mat4 View();
};

class OverviewCamera : public Camera {
  //void SetTarget();
};

#endif //CAMERA_H

