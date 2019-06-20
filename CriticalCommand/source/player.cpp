#include "player.h"

#include "playerState.h"

Player::Player() {
  
  state_ = &PlayerState::idel;
}

void Player::HandleInput(Input input) {
  //FIXME:: remove playerState*?
  PlayerState* state = state_->HandleInput(*this, input);

  if (this->state_ != NULL) {
    delete this->state_;
    this->state_ = state;
  }
}

void Player::Update() {
  state_->Update(*this);
}

void Player::MoveForward() {
  printf("(vW) ");
  /*
  FIXME::MOVE PLAYER CAMERA FORWARD
  */
}

PlayerState::PlayerState() {
  
}

PlayerState::~PlayerState() {}

PlayerState* PlayerState::HandleInput(Player& player, Input input) {
  return new IdelState();
}

void PlayerState::Update(Player& player) {
}


PlayerState* IdelState::HandleInput(Player& player, Input input) {

  idelTime++;
  if (input.KEY.W) {
    printf("move to forward state->\n");
    idelTime = 0;
    return &PlayerState::forward;
  }
  //idelTime++;
  //printf("idelTime = %d\n", idelTime);

  return &PlayerState::idel;
}

void IdelState::Update(Player& player) {

}

PlayerState* ForwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.W) {
    printf("move to idel state->");
    return &PlayerState::idel;
  }
  //printf("\n\tMADE IT HERE TO!\n");
  return &PlayerState::forward;
}

void ForwardState::Update(Player& player) {
  player.MoveForward();
}
