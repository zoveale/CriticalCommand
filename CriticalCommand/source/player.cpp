#include "player.h"

/*possible player states*/
IdelState InputState::idel;
ForwardState InputState::forward;
BackwardState InputState::backward;
StrafeLeftState InputState::left;
StrafeRightState InputState::right;
///

PlayerCamera Camera::camera;



//FIXME:: dont like this implementation
//void Player::AttachCamera(PlayerCamera camera,float xoffset, float yoffset) {
//
//  //FIXME:: dont like this implementation
//  front = camera.GetFront();
//  right = camera.GetRight();
//  camera.SetTarget(position, front, right, xoffset, yoffset);
//}

Player::Player() {
  position = glm::vec3(0.0f, 0.0f, 3.0f);
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  right = glm::vec3(0.0f, 1.0f, 0.0f);
  speed = 2.0f;
  dt = 0.0f;
  acc = 1.0f;
  camera = &Camera::camera;
  state = &InputState::idel;
  //printf("move to idel state->");
}

void Player::HandleInput(Input input, float dt) {
  this->dt = dt;
  state->HandleInput(*this, input);
}

void Player::Update(float xoffset, float yoffset) {
  look.x = xoffset;
  look.y = yoffset;
  camera->SetView(*this);
  state->Update(*this);
}

void Player::LongIdel() {
  printf("\nPlayer Long Idel->");
}

void Player::MoveForward() {
  position += front * (dt * speed);
  //position += front * (dt * speed + (0.5f * acc * dt * dt));
  //printf("player front x, y, z : %f, %f, %f\n", front.x, front.y, front.z);
  //printf("\nPlayer move Forward->");
}

void Player::MoveBackward() {
  //position.z += this->dt * speed;
  position -= this->dt * front * speed;
}

/*
FIXME:: probably not going to work
will need to edit camera vec3?
*/
void Player::MoveLeft() {
  //position.x -= this->dt * speed;
  position -= this->dt * right * speed;
}

void Player::MoveRight() {
  //position.x += this->dt * speed;
  position += this->dt * right * speed;
}

