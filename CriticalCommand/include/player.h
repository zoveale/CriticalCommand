
#ifndef PLAYER_H
#define PLAYER_H

#include "states.h"
#include "camera.h"

class Player {
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right; //right angle axis
  float dt;
  float speed;

  
public:
  PlayerState* state_;
  Player();
  void HandleInput(Input input, float dt);
  void Update();
  void AttachCamera(PlayerCamera camera);
  void LongIdel();
  void MoveForward();
};

#endif // !PLAYER_H

