#include "states.h"
#include "player.h"

PlayerState::PlayerState() {
  printf("PlayerState Called");
}

void IdelState::HandleInput(Player& player, Input input) {
  idelTime++;
  //printf("idelTime = %F", idelTime);

  if (input.KEY.W) {
    idelTime = 0;
    printf("W pressed->");
    printf("move to forward state->");
    player.state_ = &PlayerState::forwardState;
  }
  if (idelTime > 1000.0f) {
    idelTime = 0;
    player.LongIdel();
  }

}

void IdelState::Update(Player& player) {
  //printf("UpdateIdleState->");
}

void ForwardState::HandleInput(Player& player, Input input) {
  if (!input.KEY.W) {
    printf("W released\n");
    printf("move to idel state->");
    player.state_ = &PlayerState::idelState;
  }
}

void ForwardState::Update(Player& player) {
  player.MoveForward();
  printf("UpdateForwardState->");
}
