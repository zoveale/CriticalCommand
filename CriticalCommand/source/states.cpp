#include "states.h"
#include "player.h"

PlayerState::PlayerState() {
  printf("PlayerState Called");
}

void IdelState::HandleInput(Player& player, Input input) {
  
  //printf("idelTime = %F", idelTime);

  if (input.KEY.W) {
    idelTime = 0;
    printf("W pressed->");
    printf("move to forward state->");
    player.state_ = &PlayerState::forwardState;
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
    printf("W released\n");
    printf("move to idel state->");
    player.state_ = &PlayerState::idelState;
  }
}

void ForwardState::Update(Player& player) {
  //printf("UpdateForwardState->");
  player.MoveForward();
  
}
