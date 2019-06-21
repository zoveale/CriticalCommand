#include "player.h"

IdelState PlayerState::idelState;
ForwardState PlayerState::forwardState;

Player::Player() {
  state_ = &PlayerState::idelState;
  printf("move to idel state->");
}

