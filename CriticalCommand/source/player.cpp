#include "player.h"
//#include "input.h"

/*possible player states*/
IdelState InputState::idel;
ForwardState InputState::forward;
BackwardState InputState::backward;
StrafeLeftState InputState::left;
StrafeRightState InputState::right;
///
//player camera, FIXME:: add more, "topleft" "topright" etc...
PlayerCamera Camera::camera;
///

//Input playerInput;


Player::Player() {
  position = glm::vec3(12.5f, 1.0f, -15.5f);
  front = glm::vec3(1.0f, 0.0f, 0.0f);
  right = glm::vec3(0.0f, 1.0f, 0.0f);
  speed = 10.0f;
  dt = 0.0f;
  acc = 1.0f;
  //input = &playerInput;
  camera = &Camera::camera;
  state = &InputState::idel;
  //printf("move to idel state->");
  
}

void Player::StartUp() {
/*

*/
}



void Player::HandleInput(Input input, float dt) {


  //input.MouseInput(*this);
  look.x = (float)input.xoffset;
  look.y = (float)input.yoffset;
  ///
  
  
  //FIXME:: pull delta time from physics based class
  this->dt = dt;  
  ///
  state->HandleInput(*this, input);
}

void Player::Update() {
  //future update(*this) stuff
  //phyics->update(*this);
  //graphics->update(*this);
  ///

  camera->SetView(*this);
  state->Update(*this);
}

void Player::Draw() {
  //FIXME::
  //player->draw(*this)...
}

void Player::LongIdel() {
  printf("\nPlayer Long Idel->");
}

void Player::MoveForward() {
  position += front * (dt * speed);
  //FIXME:: add possible modes of moving using kinematic equations
  //position += front * (dt * speed + (0.5f * acc * dt * dt));
  ///
  //printf("player position x, y, z : %f, %f, %f\n", position.x, position.y, position.z);
  //printf("\nPlayer move Forward->");
}

void Player::MoveBackward() {
  position -= dt * front * speed;
}

void Player::MoveLeft() {
  position -= dt * right * speed;
}

void Player::MoveRight() {
  position += dt * right * speed;
}

