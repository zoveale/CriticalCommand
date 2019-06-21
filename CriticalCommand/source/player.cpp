#include "player.h"

IdelState PlayerState::idelState;
ForwardState PlayerState::forwardState;

void Player::AttachCamera(PlayerCamera camera) {
  camera.SetTarget(&position, &front, &right);
}

Player::Player() {
  this->position = glm::vec3(0.0f, 0.0f, 3.0f);
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->right = glm::vec3(0.0f, 1.0f, 0.0f);
  this->speed = 2.0f;
  this->dt = 0;
  state_ = &PlayerState::idelState;
  //printf("move to idel state->");
}

void Player::HandleInput(Input input, float dt) {
  this->dt = dt;
  state_->HandleInput(*this, input);
}

void Player::Update() {
  state_->Update(*this);
}

void Player::LongIdel() {
  printf("\nPlayer Long Idel->");
}

void Player::MoveForward() {
  this->position.x += this->dt * this->speed;
  printf("player position x, y, z : %f, %f, %f\n", position.x, position.y, position.z);
  //printf("\nPlayer move Forward->");
}

