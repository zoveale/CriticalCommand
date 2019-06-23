
#ifndef PLAYER_H
#define PLAYER_H

#include "states.h"
#include "camera.h"
#include "vec2.hpp"

class Camera;

class Player {
private:
  

  float dt;
  float speed;
  float acc;
public:
  

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right;

  glm::vec2 look;//mouse

  InputState* state;
  Camera* camera;

  Player();
  void HandleInput(Input input, float dt);
  void Update(float xoffset, float yoffset);
  
  //input componets
  void LongIdel();
  void MoveForward();
  void MoveBackward();
  void MoveLeft();
  void MoveRight();
  ///
};

#endif // !PLAYER_H

