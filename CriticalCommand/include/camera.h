#ifndef CAMERA_H
#define CAMERA_H


#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>
#include "geometric.hpp"

#include "player.h"
class PlayerCamera;


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
  const float SENSITIVITY = 0.001f;
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



  //FIXME:: dont like this implementation
  glm::vec3 GetFront();
  glm::vec3 GetRight();
  ///
  glm::mat4 View();
};

class OverviewCamera : public Camera {
  //void SetTarget();
};

#endif //CAMERA_H

/*
  glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0),
  glm::vec3(0.0f, 0.0f, 0.0),
  glm::vec3(0.0f, 1.0f, 0.0));
*/

/*
glm::lookAt(
      glm::vec3(position->x, position->y, position->z),
      glm::vec3(position->x +front->x, position->y + front->y, position->z + front->z),
      glm::vec3(right->x, right->y, right->z)
    )
    glm::lookAt(position, position + front, right)
*/

/*printf("camera position x, y, z : %f, %f, %f\n",
    this->position->x, this->position->y, this->position->z);*/

/*
 printf("camera position x, y, z : %f, %f, %f\n",
    this->position.x, this->position.y, this->position.z);
  printf("camera front x, y, z : %f, %f, %f\n",
    this->front.x, this->front.y, this->front.z);
  printf("camera right x, y, z : %f, %f, %f\n",
    this->right.x, this->right.y, this->right.z);
*/