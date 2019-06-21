
#ifndef PLAYER_H
#define PLAYER_H

#include "states.h"

class Player {
public:
  PlayerState* state_;
  Player();
  void HandleInput(Input input);
  void Update();

  void LongIdel();
  void MoveForward();
};

#endif // !PLAYER_H

