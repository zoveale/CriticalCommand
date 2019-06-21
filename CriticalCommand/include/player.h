#include "states.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
  PlayerState* state_;
  Player();
  void HandleInput(Input input) { state_->HandleInput(*this, input); }
  void Update() { state_->Update(*this); }
  void LongIdel();
  void MoveForward();
};

#endif // !PLAYER_H

