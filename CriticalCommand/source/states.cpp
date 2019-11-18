#include "states.h"

#include "player.h"



InputState::InputState() {
  //printf("InputState Called->");
}

void IdelState::HandleInput(Player& player, Input input) {
  
 

  if (input.KEY.W) {
    idelTime = 0;
#ifdef PRINT_STATES
    printf("W pressed->");
    printf("move to forward state->");
#endif // PRINT_STATES
    player.state = &InputState::forward;
  }
  else if (input.KEY.S) {
    idelTime = 0;
#ifdef PRINT_STATES
    printf("S pressed->");
    printf("move to backward state->");
#endif // PRINT_STATES
    player.state = &InputState::backward;
  }
  else if (input.KEY.A) {
    idelTime = 0;
#ifdef PRINT_STATES
    printf("A pressed->");
    printf("move to strafeLeft state->");
#endif // PRINT_STATES
    player.state = &InputState::left;
  }
  else if (input.KEY.D) {
    idelTime = 0;
#ifdef PRINT_STATES
    printf("D pressed->");
    printf("move to strafeRight state->");
#endif // PRINT_STATES
    player.state = &InputState::right;
  }
  

}

void IdelState::Update(Player& player) {
  idelTime++;
  //printf("UpdateIdleState->");
  if (idelTime > 1000.0f) {
    idelTime = 0;
    player.LongIdel();
  }
  
}

void ForwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.W) {
#ifdef PRINT_STATES
    printf("W released\n");
    printf("move to idel state->");
#endif // PRINT_STATES
    player.state = &InputState::idel;
  }
}

void ForwardState::Update(Player& player) {
  player.MoveForward();
}

void BackwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.S) {
#ifdef PRINT_STATES
    printf("S released\n");
    printf("move to idel state->");
#endif // PRINT_STATES
    player.state = &InputState::idel;
  }
}

void BackwardState::Update(Player& player) {
  player.MoveBackward();
}

void StrafeLeftState::HandleInput(Player& player, Input input) {
  if (!input.KEY.A) {
#ifdef PRINT_STATES
    printf("A released\n");
    printf("move to idel state->");
#endif // PRINT_STATES
    player.state = &InputState::idel;
  }

}

void StrafeLeftState::Update(Player& player) {
  player.MoveLeft();
}

void StrafeRightState::HandleInput(Player& player, Input input) {
  if (!input.KEY.D) {
#ifdef PRINT_STATES
    printf("D released\n");
    printf("move to idel state->");
#endif // PRINT_STATES
    player.state = &InputState::idel;
  }
  
}

void StrafeRightState::Update(Player& player) {
  player.MoveRight();
}

