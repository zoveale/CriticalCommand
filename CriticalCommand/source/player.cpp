#include "player.h"

IdelState PlayerState::idelState;
ForwardState PlayerState::forwardState;

Player::Player() {
  state_ = &PlayerState::idelState;
  printf("move to idel state->");
}

void Player::HandleInput(Input input) {
  state_->HandleInput(*this, input);
}

void Player::Update() {
  state_->Update(*this);
}

void Player::LongIdel() {
  printf("\nPlayer Long Idel->");
}

void Player::MoveForward() {
  printf("\nPlayer move Forward->");
}

