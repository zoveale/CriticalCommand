#ifndef CAMERA_H
#define CAMERA_H


#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtc/matrix_transform.hpp>

class Camera {
public:
  virtual void SetTarget(glm::vec3* p, glm::vec3* f, glm::vec3* r) = 0;

};

class PlayerCamera : public Camera {
private:
  glm::vec3* position;
  glm::vec3* front;
  glm::vec3* right; //right angle axis
  int* mouseX;
  int* moustY;
  int* mouseScrollValue;

  glm::mat4x4 lookTest;
  inline void Update();
public:
  PlayerCamera();
  void SetTarget(glm::vec3* p, glm::vec3* f, glm::vec3* r);
  glm::mat4 CameraView();
};

class OverviewCamera : public Camera {
  //void SetTarget();
};

#endif //CAMERA_H