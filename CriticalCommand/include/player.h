#ifndef PLAYER_H
#define PLAYER_H
#include "input.h"

class Player {

};
class PlayerState {
  virtual ~PlayerState(){}
  virtual void HandleInput(Player& player, Input input);
  virtual void update(Player& player);
};

class StandingState : public PlayerState {
public:
private:

};
#endif // !PLAYER_H
