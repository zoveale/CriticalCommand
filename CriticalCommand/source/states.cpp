#include "states.h"
#include "player.h"

InputState::InputState() {
  printf("InputState Called->");
}

void IdelState::HandleInput(Player& player, Input input) {
  
 

  if (input.KEY.W) {
    idelTime = 0;
    printf("W pressed->");
    printf("move to forward state->");
    player.state = &InputState::forward;
  }
  else if (input.KEY.S) {
    idelTime = 0;
    printf("S pressed->");
    printf("move to backward state->");
    player.state = &InputState::backward;
  }
  else if (input.KEY.A) {
    idelTime = 0;
    printf("A pressed->");
    printf("move to strafeLeft state->");
    player.state = &InputState::left;
  }
  else if (input.KEY.D) {
    idelTime = 0;
    printf("D pressed->");
    printf("move to strafeRight state->");
    player.state = &InputState::right;
  }
  

}

void IdelState::Update(Player& player) {
  idelTime++;
  //printf("UpdateIdleState->");
  if (idelTime > 100.0f) {
    idelTime = 0;
    player.LongIdel();
  }
  
}

void ForwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.W) {
    printf("W released\n");
    printf("move to idel state->");
    player.state = &InputState::idel;
  }
}

void ForwardState::Update(Player& player) {
  //printf("UpdateForwardState->");hey 
  player.MoveForward();
  
}

void BackwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.S) {
    printf("S released\n");
    printf("move to idel state->");
    player.state = &InputState::idel;
  }
}

void BackwardState::Update(Player& player) {
  player.MoveBackward();
}

void StrafeLeftState::HandleInput(Player& player, Input input) {
  if (!input.KEY.A) {
    printf("A released\n");
    printf("move to idel state->");
    player.state = &InputState::idel;
  }

}

void StrafeLeftState::Update(Player& player) {
  player.MoveLeft();
}

void StrafeRightState::HandleInput(Player& player, Input input) {
  if (!input.KEY.D) {
    printf("D released\n");
    printf("move to idel state->");
    player.state = &InputState::idel;
  }
  
}

void StrafeRightState::Update(Player& player) {
  player.MoveRight();
}
