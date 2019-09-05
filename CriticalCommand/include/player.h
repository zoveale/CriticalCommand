#ifndef PLAYER_H
#define PLAYER_H

#include "states.h"
#include "camera.h"
#include "physics.h"

class Camera;
//class PlayerCamera;
//class InputState;
//class Input;
#include "vec2.hpp"

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

  //Add pointer to input class
  //Input* input;
  ///
  physx::Physics* physics;
  InputState* state;
  Camera* camera;
  //Graphics* graphic
  //model* draw

  Player();
  void StartUp();
  void HandleInput(Input input, float dt);
  void Update();
  void Draw();

  //input componets
  void LongIdel();
  void MoveForward();
  void MoveBackward();
  void MoveLeft();
  void MoveRight();
  void ShootBall();
  ///
};

#endif // !PLAYER_H

